#pragma once

#include"JBF/DXHeaders.h"
#include"JBF/Definitions.h"

namespace JBF{
    namespace Base{
        class DXResource;

        class Texture : public DXResource{
        protected:
            IDirect3DTexture9* ins_texture;
            D3DXIMAGE_INFO ins_info;

        protected:
            Texture(RES_TYPE type);
            virtual ~Texture();

        public:
            INLINE IDirect3DTexture9* GetTexture()const{ return ins_texture; }
        };
    };
};