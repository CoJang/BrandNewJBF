#include"pch.h"

#include"Public/Public.h"
#include"Object.h"

ObjBackground::ObjBackground() : ins_texBase(nullptr), ins_texLightMask(nullptr), ins_sprFrame(nullptr){}
ObjBackground::~ObjBackground(){}

ObjBackground* ObjBackground::Create(ARCHIVE_HASHSIZE texBase, ARCHIVE_HASHSIZE texLightMask){
    auto _new = new ObjBackground();

    if (!(_new->ins_texBase = Object::ExternalTexture::Read(&arcSprites, texBase)))goto FAILED_FUNC;

    {
        Vector2 planSize;

        planSize.x = _new->ins_texBase->GetInfo()->Width;
        planSize.y = _new->ins_texBase->GetInfo()->Height;

        if(!(_new->ins_sprFrame = BasePlane::Create(&planSize)))goto FAILED_FUNC;
    }

    if (!(_new->ins_texLightMask = Object::ExternalTexture::Read(&arcSprites, texLightMask)))goto FAILED_FUNC;

    return _new;

FAILED_FUNC:
    RELEASE(_new);
    return nullptr;
}
void ObjBackground::Release(){
    if(ins_texBase)RELEASE(ins_texBase);
    if(ins_texLightMask)RELEASE(ins_texLightMask);

    if(ins_sprFrame)RELEASE(ins_sprFrame);

    delete this;
}