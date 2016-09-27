#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjBackground{
private:
    BasePlane* ins_sprFrame;

    Object::ExternalTexture* ins_texBase;
    Object::ExternalTexture* ins_texLightMask;

private:
    ObjBackground();
    ~ObjBackground();

public:
    static ObjBackground* Create(ARCHIVE_HASHSIZE texBase, ARCHIVE_HASHSIZE texLightMask);
    void Release();

public:
    INLINE BasePlane* GetFrame()const{ return ins_sprFrame; }
    INLINE Object::ExternalTexture* GetBaseTexture()const{ return ins_texBase; }
    INLINE Object::ExternalTexture* GetLightMaskTexture()const{ return ins_texLightMask; }
};