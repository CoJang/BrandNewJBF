#include"pch.h"

#include"JBF/JBFramework.h"
#include"JBF/Object/Object.h"

#define ENTRY_NAME "main"

namespace JBF{
    namespace Object{
        Shader::Shader() : DXResource(RESTYPE_MANAGE), ins_effect(nullptr){}
        Shader::~Shader(){ Invalidate(); }

        Shader* Shader::Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file){
            return ins_read<Shader>(arc, file, [](Shader* obj)->bool{ return SUCCEEDED(obj->Validate()); });
        }

        HRESULT Shader::Validate(){
            HRESULT hr;

            void* bufData;
            DWORD sizeData;

            ID3DXBuffer* errMsg;

            if (ins_file.arc->GetDataLock(ins_file.file, &bufData, &sizeData)){
                hr = D3DXCreateEffect(
                    Core::Graphic::GetDevice(),
                    bufData,
                    sizeData,
                    nullptr,
                    nullptr,
                    D3DXSHADER_SKIPVALIDATION,
                    nullptr,
                    &ins_effect,
                    &errMsg
                );
                ins_file.arc->Unlock();
                if (FAILED(hr)){
                    Global::MsgBox(nullptr, "Shader load faiied", MB_ICONERROR | MB_OK, (char*)errMsg->GetBufferPointer());
                    RELEASE(errMsg);

                    ASSERT_HRESULT(hr, _T("Failed to load effect.\nArchive: \"%s\"\nHash key: %u"), ins_file.arc->GetFilePath(), ins_file.file);
                }
            }
            else{
                ASSERT(false, _T("Failed to read data from archive.\nArchive: \"%s\"\nHash key: %u"), ins_file.arc->GetFilePath(), ins_file.file);
                return E_FAIL;
            }

            return S_OK;
        }
        void Shader::Invalidate(){
            if (ins_effect)RELEASE(ins_effect);
        }

        HRESULT Shader::IteratePass(DWORD flag, HRESULT(*func)(void*), void* funcArg){
            HRESULT hr;
            UINT cnt, i;

            hr = ins_effect->SetTechnique(ENTRY_NAME);
            if (SUCCEEDED(hr)){

                hr = ins_effect->Begin(&cnt, flag);
                if (SUCCEEDED(hr)){
                    for (i = 0; i < cnt; ++i){

                        hr = ins_effect->BeginPass(i);
                        if (SUCCEEDED(hr)){

                            hr = func(funcArg);
                            if (SUCCEEDED(hr)){

                                hr = ins_effect->EndPass();
                                if (SUCCEEDED(hr))continue;
                                else goto END_FUNC;

                            }
                            else goto END_FUNC;

                        }
                        else goto END_FUNC;

                    }

                    hr = ins_effect->End();
                }

            }
            
        END_FUNC:
            return hr;
        }
    };
};