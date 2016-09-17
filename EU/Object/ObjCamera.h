#pragma once

#pragma once

#include"JBF/JBFramework.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjCamera{
private:
    Matrix ins_matView;
    Matrix ins_matProj;

    Matrix ins_matVP;

public:
    void Init();

private:
    INLINE void ins_calVPMatrix(){ ins_matVP = ins_matView; ins_matVP *= ins_matProj; }

public:
    void MoveDirection(const Vector3* dir);
    void Zoom(const Vector2* vec);

public:
    INLINE const Vector3* GetPosition()const{ return (const Vector3*)&ins_matView._41; }

public:
    INLINE const Matrix* GetViewMatrix()const{ return &ins_matView; }
    INLINE const Matrix* GetProjectionMatrix()const{ return &ins_matProj; }
    INLINE const Matrix* GetVPMatrix(){ ins_calVPMatrix(); return &ins_matVP; }
};