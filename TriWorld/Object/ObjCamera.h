#pragma once

#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjCamera{
private:
    Vector3 ins_vEye;
    Vector3 ins_vLookAt;
    Vector3 ins_vUp;
private:
    float ins_fFov;
    float ins_fAspect;
    float ins_fZNear;
    float ins_fZFar;
private:
    Matrix ins_mProj;
    Matrix ins_mView;

private:
    ObjCamera();
    ~ObjCamera();

public:
    static ObjCamera* Create();
    void Release();

public:
    void Init();
    void Update(const Vector3* vTarget);

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};