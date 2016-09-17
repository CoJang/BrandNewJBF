#pragma once

#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Object{
        class Shader : public Base::DXResource, public Base::ArchiveResource<Shader>{
        private:
            ID3DXEffect* ins_effect;

        private:
            Shader();
            virtual ~Shader();

        public:
            static Shader* Read(Global::Archive::Decrypter* arc, ARCHIVE_HASHSIZE file);

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();

        public:
            HRESULT IteratePass(DWORD flag, HRESULT(*func)(void*), void* funcArg);

        public:
            INLINE HRESULT GetTexture(D3DXHANDLE hParameter, IDirect3DBaseTexture9** ppTexture){ return ins_effect->GetTexture(hParameter, ppTexture); }
            INLINE HRESULT SetTexture(D3DXHANDLE hParameter, IDirect3DBaseTexture9* pTexture){ return ins_effect->SetTexture(hParameter, pTexture); }

            INLINE HRESULT GetValue(D3DXHANDLE hParameter, void* pData, UINT Bytes){ return ins_effect->GetValue(hParameter, pData, Bytes); }
            INLINE HRESULT SetValue(D3DXHANDLE hParameter, const void* pData, UINT Bytes){ return ins_effect->SetValue(hParameter, pData, Bytes); }

            INLINE HRESULT GetBool(D3DXHANDLE hParameter, BOOL* pb){ return ins_effect->GetBool(hParameter, pb); }
            INLINE HRESULT SetBool(D3DXHANDLE hParameter, BOOL b){ return ins_effect->SetBool(hParameter, b); }
            INLINE HRESULT GetBoolArray(D3DXHANDLE hParameter, BOOL* pb, UINT Count){ return ins_effect->GetBoolArray(hParameter, pb, Count); }
            INLINE HRESULT SetBoolArray(D3DXHANDLE hParameter, const BOOL* pb, UINT Count){ return ins_effect->SetBoolArray(hParameter, pb, Count); }

            INLINE HRESULT GetInt(D3DXHANDLE hParameter, int* pn){ return ins_effect->GetInt(hParameter, pn); }
            INLINE HRESULT SetInt(D3DXHANDLE hParameter, int n){ return ins_effect->SetInt(hParameter, n); }
            INLINE HRESULT GetIntArray(D3DXHANDLE hParameter, int* pn, UINT Count){ return ins_effect->GetIntArray(hParameter, pn, Count); }
            INLINE HRESULT SetIntArray(D3DXHANDLE hParameter, const int* pn, UINT Count){ return ins_effect->SetIntArray(hParameter, pn, Count); }

            INLINE HRESULT GetFloat(D3DXHANDLE hParameter, float* pf){ return ins_effect->GetFloat(hParameter, pf); }
            INLINE HRESULT SetFloat(D3DXHANDLE hParameter, float f){ return ins_effect->SetFloat(hParameter, f); }
            INLINE HRESULT GetFloatArray(D3DXHANDLE hParameter, float* pf, UINT Count){ return ins_effect->GetFloatArray(hParameter, pf, Count); }
            INLINE HRESULT SetFloatArray(D3DXHANDLE hParameter, const float* pf, UINT Count){ return ins_effect->SetFloatArray(hParameter, pf, Count); }

            INLINE HRESULT GetVector(D3DXHANDLE hParameter, Global::Math::Vector4* pVector){ return ins_effect->GetVector(hParameter, (D3DXVECTOR4*)pVector); }
            INLINE HRESULT SetVector(D3DXHANDLE hParameter, const Global::Math::Vector4* pVector){ return ins_effect->SetVector(hParameter, (const D3DXVECTOR4*)pVector); }
            INLINE HRESULT GetVectorArray(D3DXHANDLE hParameter, Global::Math::Vector4* pVector, UINT Count){ return ins_effect->GetVectorArray(hParameter, (D3DXVECTOR4*)pVector, Count); }
            INLINE HRESULT SetVectorArray(D3DXHANDLE hParameter, const Global::Math::Vector4* pVector, UINT Count){ return ins_effect->SetVectorArray(hParameter, (const D3DXVECTOR4*)pVector, Count); }

            INLINE HRESULT GetMatrix(D3DXHANDLE hParameter, Global::Math::Matrix* pVector){ return ins_effect->GetMatrix(hParameter, (D3DXMATRIXA16*)pVector); }
            INLINE HRESULT SetMatrix(D3DXHANDLE hParameter, const Global::Math::Matrix* pVector){ return ins_effect->SetMatrix(hParameter, (const D3DXMATRIXA16*)pVector); }
            INLINE HRESULT GetMatrixArray(D3DXHANDLE hParameter, Global::Math::Matrix* pVector, UINT Count){ return ins_effect->GetMatrixArray(hParameter, (D3DXMATRIXA16*)pVector, Count); }
            INLINE HRESULT SetMatrixArray(D3DXHANDLE hParameter, const Global::Math::Matrix* pVector, UINT Count){ return ins_effect->SetMatrixArray(hParameter, (const D3DXMATRIXA16*)pVector, Count); }

        public:
            friend class Base::ArchiveResource<Shader>;
        };
    };
};