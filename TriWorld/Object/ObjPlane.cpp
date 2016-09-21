#include"pch.h"
#include"Object.h"

ObjPlane::ObjPlane(){}
ObjPlane::~ObjPlane(){}

ObjPlane* ObjPlane::Create(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName){
    auto _new = Global::Alloc::NewCustomAligned<ObjPlane>(32);

    _new->Init(arcModel, arcTexture, fileName);
    return _new;
}
void ObjPlane::Release(){ Global::Alloc::DeleteCustomAligned(this); }

HRESULT ObjPlane::Validate(){
    HRESULT hr = BaseXFormatMesh::Validate();
    if (SUCCEEDED(hr)){
        ins_transTable.resize(ins_partConut);
        ins_mWorld = Matrix::constIdentity;

        ins_transTable[0] = ins_trsBasic;
        ins_transTable[1] = ins_trsBasic;
        ins_transTable[2] = ins_trsBasic;
        ins_transTable[3] = ins_trsBasic;
        ins_transTable[4] = ins_trsBasic;
        ins_transTable[5] = ins_trsRotatePropeller;
        ins_transTable[6] = ins_trsBasic;
        ins_transTable[7] = ins_trsBasic;
    }
    return hr;
}

void ObjPlane::Init(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName){
    ins_arcModel = arcModel;
    ins_arcTexture = arcTexture;
    ins_fileName = fileName;

    ins_fTurnPos = 0;
    ins_fPropRotate = 0;

    Validate();
}

void ObjPlane::ins_trsBasic(ObjPlane* _this){
    Core::Graphic::SetTransform(D3DTS_WORLD, &_this->ins_mWorld);
}
void ObjPlane::ins_trsRotatePropeller(ObjPlane* _this){
    static const float angleX = -6.5f;
    static const float angleY = 0.5f;
    Matrix mTmp0, mTmp1;

    MatrixRotationX(&mTmp0, angleX * (PIf / 180.f));

    MatrixRotationY(&mTmp1, angleY * (PIf / 180.f));
    mTmp0 *= mTmp1;

    MatrixRotationZ(&mTmp1, _this->ins_fPropRotate);
    mTmp0 *= mTmp1;

    MatrixRotationY(&mTmp1, -angleY * (PIf / 180.f));
    mTmp0 *= mTmp1;

    MatrixRotationX(&mTmp1, -angleX * (PIf / 180.f));
    mTmp0 *= mTmp1;

    mTmp0 *= _this->ins_mWorld;

    Core::Graphic::SetTransform(D3DTS_WORLD, &mTmp0);
}

void ObjPlane::TurnAround(const Vector3* pCenter, const float fRad, const float fSpeed){
    Vector3 vPos;
    Matrix mTmp;

    MatrixRotationZ(&ins_mWorld, 0.3f);

    MatrixRotationY(&mTmp, -PIf * 0.5f);
    ins_mWorld *= mTmp;

    vPos = *pCenter;
    vPos.z += fRad;
    MatrixTranslation(&mTmp, &vPos);
    ins_mWorld *= mTmp;

    ins_fTurnSpeed = fSpeed;
    MatrixRotationY(&mTmp, ins_fTurnPos);
    ins_mWorld *= mTmp;
}

void ObjPlane::Update(float delta){
    ins_fTurnPos += ins_fTurnSpeed * delta;
    if (ins_fTurnPos > PIf * 2.f)ins_fTurnPos = 0.f;

    ins_fPropRotate += delta * 50.f;
    if (ins_fPropRotate > PIf * 2.f)ins_fPropRotate = 0.f;
}
void ObjPlane::Draw(){
    auto drawPartFunc = [=](DWORD i)->void{
        auto curTex = ins_texTable[i];

        Core::Graphic::SetMaterial(&ins_mtrlTable[i]);
        Core::Graphic::SetTexture(0, curTex ? curTex->GetTexture() : nullptr);

        ins_mesh->DrawSubset(i);
    };

    Core::Graphic::SetTransform(D3DTS_WORLD, &ins_mWorld);

    for (auto i = decltype(ins_partConut){0}; i < ins_partConut; ++i){
        ins_transTable[i](this);
        drawPartFunc(i);
    }
}