﻿#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjBackground : public BaseObject{
private:
    Object::ExternalTexture* ins_texLightMask;

private:
    ObjBackground();
    virtual ~ObjBackground();

public:
    static ObjBackground* Create(ARCHIVE_HASHSIZE texBase, ARCHIVE_HASHSIZE texLightMask);
    virtual void Release();

public:
    void DrawBase();
    void DrawLightMask();

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
};