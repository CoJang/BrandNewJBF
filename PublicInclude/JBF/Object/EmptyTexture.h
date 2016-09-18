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

        private:
            EmptyTexture(INFO* inf);
            virtual ~EmptyTexture();

        public:
            static EmptyTexture* Create(INFO* inf);
            void Release();

        public:
            virtual HRESULT Validate();
            virtual void Invalidate();

        public:
            HRESULT CopySurface(UINT level, EmptyTexture* src);

        public:
            INLINE IDirect3DSurface9* GetSurface(UINT level){ return ins_surfaceTable[level]; }
            INLINE const INFO* GetInfo()const{ return &ins_info; }
        };
    };
};