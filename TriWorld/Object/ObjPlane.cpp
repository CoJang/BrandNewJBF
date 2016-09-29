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
    HRESULT hr;

    hr = BaseXFormatMesh::Validate();
    if (FAILED(hr))return hr;

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

    MatrixRotationX(&mTmp0, angleX * DEG2RADf);

    MatrixRotationY(&mTmp1, angleY * DEG2RADf);
    mTmp0 *= mTmp1;

    MatrixRotationZ(&mTmp1, _this->ins_fPropRotate);
    mTmp0 *= mTmp1;

    MatrixRotationY(&mTmp1, -angleY * DEG2RADf);
    mTmp0 *= mTmp1;

    MatrixRotationX(&mTmp1, -angleX * DEG2RADf);
    mTmp0 *= mTmp1;

    mTmp0 *= _this->ins_mWorld;

    Core::Graphic::SetTransform(D3DTS_WORLD, &mTmp0);
}

void ObjPlane::TurnAround(const Vector3* pCenter, const float fRad, const float fSpeed){
    Vector3 vPos;
    Matrix mTmp;

    MatrixRotationZ(&ins_mWorld, 0.3f);

    vPos = *pCenter;
    vPos.x += fRad;
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

struct _CALLBACK_ARG{
    ID3DXMesh* mesh;
    DWORD index;
};
static HRESULT _draw_callback(void* rawObj){
    auto obj = (_CALLBACK_ARG*)rawObj;

    return obj->mesh->DrawSubset(obj->index);
}
void ObjPlane::Draw(Object::Shader* shader){
    Matrix matTmp, matTmp2;
    auto drawPartFunc = [=](DWORD i)->void{
        _CALLBACK_ARG callbackArg = { ins_mesh, i };
        auto curTex = ins_texTable[i];

        Core::Graphic::SetMaterial(&ins_mtrlTable[i]);

        {
            shader->SetVector("k_d", (const Vector4*)&ins_mtrlTable[i].Diffuse);
            shader->SetVector("k_a", (const Vector4*)&ins_mtrlTable[i].Ambient);
            shader->SetVector("k_s", (const Vector4*)&ins_mtrlTable[i].Specular);
            shader->SetFloat("k_p", ins_mtrlTable[i].Power);
        }

        Core::Graphic::SetTexture(0, curTex ? curTex->GetTexture() : ins_texWhite->GetTexture());

        shader->IteratePass(0, _draw_callback, &callbackArg);
    };

    Core::Graphic::SetTransform(D3DTS_WORLD, &ins_mWorld);

    Core::Graphic::SetTexture(1, ins_texWhite->GetTexture());

    Core::Graphic::SetVertexDeclaration(ins_vertDecl);

    for (auto i = decltype(ins_partConut){0}; i < ins_partConut; ++i){
        ins_transTable[i](this);

        {
            Core::Graphic::GetTransform(D3DTS_WORLD, &matTmp);
            Core::Graphic::GetTransform(D3DTS_VIEW, &matTmp2);

            matTmp = matTmp * matTmp2;
            shader->SetMatrix("matWV", &matTmp);
        }
        {
            Core::Graphic::GetTransform(D3DTS_PROJECTION, &matTmp2);

            matTmp *= matTmp2;
            shader->SetMatrix("matWVP", &matTmp);
        }

        drawPartFunc(i);
    }
}