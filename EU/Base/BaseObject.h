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
    Matrix ins_matWVP;

protected:
    BaseObject();
    virtual ~BaseObject();

public:
    virtual void Release();

public:
    virtual void Update(float delta, const Matrix* matVP);

public:
    virtual void DrawBase() = 0;

public:
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};