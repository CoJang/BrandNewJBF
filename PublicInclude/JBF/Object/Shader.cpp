#include"pch.h"

#include"JBF/JBFramework.h"
#include"JBF/Object/Object.h"

namespace JBF{
    namespace Object{
        Shader::Shader() : DXResource(RESTYPE_MANAGE){}
        Shader::~Shader(){ Invalidate(); }

        Shader* Shader::Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file){
            return ins_read<Shader>(arc, file, [](Shader* obj)->bool{ return SUCCEEDED(obj->Validate()); });
        }

        HRESULT Shader::Validate(){
            HRESULT hr;

            void* bufData;
            DWORD sizeData;

            ID3DXBuffer* msg;
            DWORD flag = 0;
#ifdef _DEBUG
            flag |= D3DXSHADER_DEBUG;
#endif

            if (ins_file.arc->GetDataLock(ins_file.file, &bufData, &sizeData)){
                hr = D3DXCreateEffect(
                    Core::Graphic::GetDevice(),
                    bufData,
                    sizeData,
                    nullptr,
                    nullptr,
                    flag,
                    nullptr,
                    &ins_effect,
                    &msg
                );
                ins_file.arc->Unlock();
                if (FAILED(hr)){
                    ASSERT_HRESULT(hr, _T("Failed to load effect.\nArchive: \"%s\"\nHash key: %u"), ins_file.arc->GetFilePath(), ins_file.file);

                    Global::MsgBox(nullptr, "Shader Error", MB_ICONERROR | MB_OK, (char*)msg->GetBufferPointer());
                    RELEASE(msg);
                }
            }
            else{
                ASSERT(false, _T("Failed to read data from archive.\nArchive: \"%s\"\nHash key: %u"), ins_file.arc->GetFilePath(), ins_file.file);
                return E_FAIL;
            }

            return S_OK;
        }
        void Shader::Invalidate(){

        }
    };
};