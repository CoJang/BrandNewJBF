#include"pch.h"

#include"JBF/JBFramework.h"
#include"JBF/Object/Object.h"

namespace JBF{
    namespace Object{
        std::unordered_map<ExternalTexture::DATA_KEY, std::pair<ExternalTexture*, DWORD>, ExternalTexture::DATA_HASHER> ExternalTexture::ins_table;

        size_t ExternalTexture::DATA_HASHER::operator()(const DATA_KEY& key)const{
            return (reinterpret_cast<size_t>(key.arc) ^ key.file);
        }

        void ExternalTexture::InitTable(){
            ins_table.rehash(1 << 20);
        }
        void ExternalTexture::ReleaseTable(){
            for (auto i = ins_table.begin(), e = ins_table.end(); i != e; ++i){
                delete i->second.first;
            }
        }

        ExternalTexture::ExternalTexture() : Base::Texture(RESTYPE_MANAGE){}
        ExternalTexture::~ExternalTexture(){ Invalidate(); }

        ExternalTexture* ExternalTexture::Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file){
            DATA_KEY key = { arc, file };
            auto itr = ins_table.find(key);
            ExternalTexture* _new;

            if (itr == ins_table.end()){
                _new = new ExternalTexture();

                _new->ins_file.arc = arc;
                _new->ins_file.file = file;

                if (!_new->Validate()){
                    delete _new;
                    return nullptr;
                }

                ins_table.emplace(key, std::make_pair(_new, (DWORD)0));
            }
            else{
                _new = itr->second.first;
                ++itr->second.second;
            }

            return _new;
        }
        void ExternalTexture::Release(ExternalTexture* obj){
            auto itr = ins_table.find(obj->ins_file);

            if (itr == ins_table.end())return;

            if (itr->second.second)--itr->second.second;
            else{
                delete itr->second.first;
                ins_table.erase(itr);
            }
        }

        HRESULT ExternalTexture::Validate(){
            HRESULT hr;

            void* bufData;
            DWORD sizeData;

            if (ins_file.arc->GetDataLock(ins_file.file, &bufData, &sizeData)){
                hr = D3DXCreateTextureFromFileInMemoryEx(
                    Core::Graphic::GetDevice(),
                    bufData,
                    sizeData,
                    D3DX_DEFAULT, D3DX_DEFAULT,
                    D3DX_DEFAULT,
                    D3DUSAGE_DYNAMIC,
                    D3DFMT_UNKNOWN,
                    D3DPOOL_MANAGED,
                    D3DX_DEFAULT,
                    D3DX_DEFAULT,
                    NULL,
                    &ins_info,
                    nullptr,
                    &ins_texture
                );
                ins_file.arc->Unlock();
                ASSERT_HRESULT(hr, _T("Failed to load texture."));
            }
            else return E_FAIL;

            return S_OK;
        }
        void ExternalTexture::Invalidate(){
            if (ins_texture)RELEASE(ins_texture);
        }
    };
};