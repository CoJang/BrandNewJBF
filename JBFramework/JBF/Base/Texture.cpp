#include"pch.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Base{
        Texture::Texture(RES_TYPE type) : DXResource(type), ins_texture(nullptr){}
        Texture::~Texture(){}
    };
};