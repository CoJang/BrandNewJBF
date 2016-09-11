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

public:
    ObjCamera();

public:
    void Init();
    void Update(Vector3* vTarget);
};