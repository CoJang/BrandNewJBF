#pragma once

#include"JBF/JBFramework.h"
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjLight : public BaseObject{
private:
    Matrix ins_matWorld;

private:
    ObjLight();
    virtual ~ObjLight();

public:
    static ObjLight* Create(ARCHIVE_HASHSIZE texBase);

public:
    virtual void Update(float delta, const Matrix* matVP);

public: //temp
    void SetPosition(const Vector3* pos){
        *(Vector3*)&ins_matWorld._41 = *pos;
        ins_matWorld._44 = 1;
    }

public:
    virtual void DrawBase();

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
};