﻿#include"pch.h"

#include"JBF/JBFramework.h"
#include"JBF/Object/Object.h"

namespace JBF{
    namespace Object{
        EmptyTexture::EmptyTexture(INFO* inf) : 
            Base::Texture(inf->pool == D3DPOOL_MANAGED ? RESTYPE_MANAGE : RESTYPE_VRAM),
            ins_surfaceTable(inf->mipLevels)
        {
            memcpy_s(&ins_info, sizeof(*inf), inf, sizeof(*inf));
        }
        EmptyTexture::~EmptyTexture(){ Invalidate(); }

        EmptyTexture* EmptyTexture::Create(INFO* inf){
            HRESULT hr;
            auto _new = new EmptyTexture(inf);

            hr = _new->Validate();
            if (FAILED(hr)){
                delete _new;
                return nullptr;
            }

            return _new;
        }
        void EmptyTexture::Release(){ delete this; }

        HRESULT EmptyTexture::Validate(){
            HRESULT hr;

            {
                hr = D3DXCreateTexture(
                    Core::Graphic::GetDevice(),
                    ins_info.width,
                    ins_info.height,
                    ins_info.mipLevels,
                    ins_info.usage,
                    ins_info.format,
                    ins_info.pool,
                    &ins_texture
                );
                if (FAILED(hr)){
                    ASSERT_HRESULT(hr, _T("Failed to create empty texture.\nError code: %x"), hr);
                    return hr;
                }
            }

            {
                for (auto i = decltype(ins_info.mipLevels){0}; i < ins_info.mipLevels; ++i){
                    hr = ins_texture->GetSurfaceLevel(i, &ins_surfaceTable[i]);
                    ASSERT_HRESULT(hr, _T("Failed to get surface index %u from texture.\nError code: %x"), i, hr);
                    return hr;
                }
            }

            return S_OK;
        }
        void EmptyTexture::Invalidate(){
            if (ins_texture)RELEASE(ins_texture);
        }
    };
};