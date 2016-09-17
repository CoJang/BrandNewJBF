#include"pch.h"

#include"JBF/JBFramework.h"
#include"JBF/Object/Object.h"

namespace JBF{
    namespace Object{
        ExternalTexture::ExternalTexture() : Base::Texture(RESTYPE_MANAGE){}
        ExternalTexture::~ExternalTexture(){ Invalidate(); }

        ExternalTexture* ExternalTexture::Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file){
            return ins_read<ExternalTexture>(arc, file, [](ExternalTexture* obj)->bool{return SUCCEEDED(obj->Validate()); });
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
                    0,
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
                ASSERT_HRESULT(hr, _T("Failed to load texture.\nArchive: %s\nHash key: %u\nError code: %x"), ins_file.arc->GetFilePath(), ins_file.file, hr);
            }
            else{
                ASSERT(false, _T("Failed to read data from archive.\nArchive: %s\nHash key: %u"), ins_file.arc->GetFilePath(), ins_file.file);
                return E_FAIL;
            }

            return S_OK;
        }
        void ExternalTexture::Invalidate(){
            if (ins_texture)RELEASE(ins_texture);
        }
    };
};