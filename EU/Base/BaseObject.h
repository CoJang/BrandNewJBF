#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;
using namespace JBF::Global::Math;

class BasePlane;

class BaseObject{
protected:
    BasePlane* ins_sprFrame;

    Object::ExternalTexture* ins_texBase;

protected:
    BaseObject();
    virtual ~BaseObject();

public:
    virtual void Release();

public:
    virtual void DrawBase(const Matrix* matVP) = 0;
};