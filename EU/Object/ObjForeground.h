#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjForeground : public BaseObject{
private:
    Object::ExternalTexture* ins_texLightMask;

private:
    Matrix ins_matWorld;

private:
    ObjForeground();
    virtual ~ObjForeground();

public:
    static ObjForeground* Create(ARCHIVE_HASHSIZE texBase, ARCHIVE_HASHSIZE texLightMask);
    virtual void Release();

public:
    void SetPosition(const Vector3* pos);

public:
    virtual void Update(float delta, const Matrix* matVP);

public:
    void DrawBase();
    void DrawLightMask();

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
};