#include"pch.h"
#include"Object.h"

ObjCamera::ObjCamera(){
    ins_vEye = Vector3(0.f, 10.f, -40.f);
    ins_vLookAt = Vector3(0.f, 0.f, 0.f);
    ins_vUp = Vector3(0.f, 1.f, 0.f);

    ins_fFov = PIf * 0.25f;
    ins_fAspect = (float)Core::Graphic::GetDisplayInfo()->Width / Core::Graphic::GetDisplayInfo()->Height;
    ins_fZNear = 1.f;
    ins_fZFar = 300.f;
}
ObjCamera::~ObjCamera(){}

ObjCamera* ObjCamera::Create(){
    auto _new = Global::Alloc::NewCustomAligned<ObjCamera>(32);

    _new->Init();
    return _new;
}
void ObjCamera::Release(){ Global::Alloc::DeleteCustomAligned(this); }

void ObjCamera::Init(){
    MatrixLookAtLH(&ins_mView, &ins_vEye, &ins_vLookAt, &ins_vUp);
    Core::Graphic::SetTransform(D3DTS_VIEW, &ins_mView);

    ins_fAspect = (float)Core::Graphic::GetDisplayInfo()->Width / Core::Graphic::GetDisplayInfo()->Height;
    MatrixPerspectiveFovLH(&ins_mProj, ins_fFov, ins_fAspect, ins_fZNear, ins_fZFar);
    Core::Graphic::SetTransform(D3DTS_PROJECTION, &ins_mProj);
}
void ObjCamera::Update(const Vector3* vTarget){
    MatrixLookAtLH(&ins_mView, &ins_vEye, vTarget, &ins_vUp);
    Core::Graphic::SetTransform(D3DTS_VIEW, &ins_mView);
}