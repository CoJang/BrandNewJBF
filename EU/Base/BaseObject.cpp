#include"pch.h"
#include"Base.h"

BaseObject::BaseObject() : ins_sprFrame(nullptr), ins_texBase(nullptr){}
BaseObject::~BaseObject(){}

void BaseObject::Release(){
    if (ins_texBase)RELEASE(ins_texBase);

    if (ins_sprFrame)RELEASE(ins_sprFrame);

    Global::Alloc::DeleteCustomAligned(this);
}

void BaseObject::Update(float delta, const Matrix* matVP){
    ins_matWVP = *matVP;
}