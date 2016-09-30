#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjLight : BaseObject{
private:
    ObjLight();
    virtual ~ObjLight();

private:
    Matrix ins_matWorld;

public:
    static ObjLight* Create(ARCHIVE_HASHSIZE texBase);
    virtual void Release();

public:
    virtual void DrawBase(const Matrix* matVP);

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};