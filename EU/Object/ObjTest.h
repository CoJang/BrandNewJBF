#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjTest{
private:
    BasePlane* ins_sprite;
    Object::ExternalTexture* ins_texture;

private:
    Object::Shader* ins_shader;

private:
    Matrix ins_matWorld;

private:
    ObjTest(ARCHIVE_HASHSIZE sprite);
    virtual ~ObjTest();

public:
    static ObjTest* Create(ARCHIVE_HASHSIZE sprite);
    void Release();

public:
    bool Draw(const Matrix* matVP);

public:
    void SetPosition(Vector3* pos);

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};