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
    void DrawBase(const Matrix* matVP);
    void DrawLightMask(const Matrix* matVP);
};