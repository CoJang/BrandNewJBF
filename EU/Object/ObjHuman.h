#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjHuman{
private:
    BasePlane* ins_sprite;
    Object::ExternalTexture* ins_texture;

private:
    Object::Shader* ins_shader;

private:
    Matrix ins_matWorld;

private:
    ObjHuman();
    virtual ~ObjHuman();

public:
    static ObjHuman* Create();
    void Release();

public:
    bool Draw(const Matrix* matVP);

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};