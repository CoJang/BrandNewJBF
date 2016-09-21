#pragma once

#include<vector>
#include"Base/Base.h"

using namespace JBF;
using namespace JBF::Global::Math;

class ObjPlane : public BaseXFormatMesh{
private:
    std::vector<void(*)(ObjPlane*)> ins_transTable;

private:
    Matrix ins_mWorld;

private:
    float ins_fTurnPos;
    float ins_fTurnSpeed;
    float ins_fPropRotate;

private:
    ObjPlane();
    virtual ~ObjPlane();

public:
    virtual HRESULT Validate();

private:
    static void ins_trsBasic(ObjPlane* _this);
    static void ins_trsRotatePropeller(ObjPlane* _this);

public:
    static ObjPlane* Create(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName);
    void Release();

public:
    void Init(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName);

public:
    void Update(float delta);
    void Draw();

public:
    void TurnAround(const Vector3* pCenter, const float fRad, const float fSpeed);

public:
    INLINE const Vector3* GetPosition()const{ return (const Vector3*)&ins_mWorld._41; }

public:
    template<class T, typename... ARGS> friend T* Global::Alloc::NewCustomAligned(size_t, ARGS&&...);
    template<class T> friend void Global::Alloc::DeleteCustomAligned(T*);
};