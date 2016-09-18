#include"pch.h"
#include"ObjCamera.h"

ObjCamera::ObjCamera(){}
ObjCamera::~ObjCamera(){}

ObjCamera* ObjCamera::Create(){
    auto _new = Global::Alloc::NewCustomAligned<ObjCamera>(32);
    _new->Init();

    return _new;
}
void ObjCamera::Release(){ Global::Alloc::DeleteCustomAligned(this); }

void ObjCamera::Init(){
    MatrixOrthoLH(
        &ins_matProj,
        Core::Graphic::GetDisplayInfo()->Width,
        Core::Graphic::GetDisplayInfo()->Height,
        0.f,
        1000.f
    );

    ins_matView = Matrix::constIdentity;

    ins_calVPMatrix();
}

void ObjCamera::MoveDirection(const Vector3* dir){
    Vector3* pos = (decltype(pos))&ins_matView._41;
    *pos -= *dir;
    ins_matView._44 = 1.f;
}
void ObjCamera::Zoom(const Vector2* vec){
    ins_matView._11 = vec->x;
    ins_matView._22 = vec->y;
}