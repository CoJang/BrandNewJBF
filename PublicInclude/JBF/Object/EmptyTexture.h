#pragma once

#include<vector>
#include"JBF/Definitions.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Object{
        class EmptyTexture : public Base::Texture{
        public:
            struct INFO{
                UINT width, height;
                UINT mipLevels;
                DWORD usage;
                D3DFORMAT format;
                D3DPOOL pool;
            };

        private:
            std::vector<IDirect3DSurface9*> ins_surfaceTable;
            INFO ins_info;

            void(*ins_validateCallback)(void* arg);
            void* ins_validateCallbackArg;

        private:
            EmptyTexture(const INFO* inf);
            virtual ~EmptyTexture();

        public:
            static EmptyTexture* Create(const INFO* inf);
            void Release();

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();

        public:
            HRESULT CopySurface(UINT level, EmptyTexture* src);

        public:
            INLINE IDirect3DSurface9* GetSurface(UINT level){ return ins_surfaceTable[level]; }
            INLINE INFO* GetInfo(){ return &ins_info; }
        public:
            INLINE void SetValidateCallback(void(*callback)(void* arg), void* arg){ ins_validateCallback = callback; ins_validateCallbackArg = arg; }
        };
    };
};