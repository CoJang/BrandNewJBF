#include"pch.h"

#include<string>
#include"Object.h"

#define RES_FILENAME(str) JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(str, tstrlen(str))

ObjDwarf::ObjDwarf(){}
ObjDwarf::~ObjDwarf(){}

HRESULT ObjDwarf::Validate(){
    HRESULT hr;

    void* bufData;
    DWORD sizeData;

    ID3DXBuffer* bufMtrl;
    D3DXMATERIAL* convMtrl;

    std::string tmpStr(1024, 0);
    std::string tmpName(1024, 0);
    std::string tmpExt(16, 0);

    ins_texWhite = Object::ExternalTexture::Read(ins_arcTexture, RES_FILENAME(_T("Texture_white.dds")));
    if (!ins_texWhite)return E_FAIL;

    if (ins_arcModel->GetDataLock(ins_fileName, &bufData, &sizeData)){
        hr = D3DXLoadMeshFromXInMemory(
            bufData, sizeData,
            D3DXMESH_MANAGED,
            Core::Graphic::GetDevice(),
            nullptr,
            &bufMtrl,
            nullptr,
            &ins_partConut,
            &ins_mesh
        );
        ins_arcModel->Unlock();
        ASSERT_HRESULT(hr, _T("Failed to load XFile mesh.\nArchive: %s\nHash key: %u\nError code: %x"), ins_arcModel->GetFilePath(), ins_fileName, hr);

        {
            D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];

            hr = ins_mesh->GetDeclaration(decl);
            ASSERT_HRESULT(hr, _T("Failed to read vertex declaration.\nArchive: %s\nHash key: %u\nError code: %x"), ins_arcModel->GetFilePath(), ins_fileName, hr);

            hr = Core::Graphic::CreateVertexDeclaration(decl, &ins_vertDecl);
            ASSERT_HRESULT(hr, _T("Failed to create vertex declaration.\nArchive: %s\nHash key: %u\nError code: %x"), ins_arcModel->GetFilePath(), ins_fileName, hr);
        }

        {
            ins_mtrlTable.resize(ins_partConut);
            ins_texTable.resize(ins_partConut);
            ins_texBumpTable.resize(ins_partConut);
            ins_texSpecTable.resize(ins_partConut);
        }

        convMtrl = (decltype(convMtrl))bufMtrl->GetBufferPointer();

        for (auto i = decltype(ins_partConut){0}; i < ins_partConut; ++i){
            if (convMtrl[i].pTextureFilename){
                ARCHIVE_HASHSIZE key;
                size_t tmpLen;

                tmpStr.clear();
                tmpName.clear();
                tmpExt.clear();

                tmpName = convMtrl[i].pTextureFilename;

                key = Global::Hash::X65599Generator<decltype(key), char>(tmpName.c_str(), tmpName.length());
                ins_texTable[i] = Object::ExternalTexture::Read(ins_arcTexture, key);
                if (!ins_texTable[i])return E_FAIL;

                tmpLen = tmpName.rfind('.');
                tmpExt = tmpName.substr(tmpLen);
                tmpName = tmpName.substr(0, tmpLen);

                tmpStr = tmpName + "_bumpmap" + tmpExt;
                key = Global::Hash::X65599Generator<decltype(key), char>(tmpStr.c_str(), tmpStr.length());
                ins_texBumpTable[i] = Object::ExternalTexture::Read(ins_arcTexture, key);
                if (!ins_texBumpTable[i])return E_FAIL;

                tmpStr = tmpName + "_spec" + tmpExt;
                key = Global::Hash::X65599Generator<decltype(key), char>(tmpStr.c_str(), tmpStr.length());
                ins_texSpecTable[i] = Object::ExternalTexture::Read(ins_arcTexture, key);
                if (!ins_texSpecTable[i])return E_FAIL;
            }

            ins_mtrlTable[i] = convMtrl[i].MatD3D;
            {
                ins_mtrlTable[i].Diffuse.a = ins_mtrlTable[i].Diffuse.a <= 0.f ? 1.f : ins_mtrlTable[i].Diffuse.a;
                ins_mtrlTable[i].Ambient = ins_mtrlTable[i].Diffuse;

                ins_mtrlTable[i].Specular = D3DXCOLOR(1, 1, 1, 1);
                ins_mtrlTable[i].Power = 30;
            }
        }

        RELEASE(bufMtrl);
    }
    else{
        ASSERT(false, _T("Failed to read data from archive.\nArchive: %s\nHash key: %u"), ins_arcModel->GetFilePath(), ins_fileName);
        return E_FAIL;
    }

    return S_OK;
}
void ObjDwarf::Invalidate(){
    BaseXFormatMesh::Invalidate();

    for (auto i = ins_texBumpTable.begin(), e = ins_texBumpTable.end(); i != e; ++i){
        auto cur = *i;
        if (cur){
            RELEASE(cur);
            *i = nullptr;
        }
    }
    for (auto i = ins_texSpecTable.begin(), e = ins_texSpecTable.end(); i != e; ++i){
        auto cur = *i;
        if (cur){
            RELEASE(cur);
            *i = nullptr;
        }
    }
}

ObjDwarf* ObjDwarf::Create(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName){
    auto _new = Global::Alloc::NewCustomAligned<ObjDwarf>(32);

    _new->Init(arcModel, arcTexture, fileName);
    return _new;
}
void ObjDwarf::Release(){ Global::Alloc::DeleteCustomAligned(this); }

void ObjDwarf::Init(Global::Archive::Decrypter* arcModel, Global::Archive::Decrypter* arcTexture, ARCHIVE_HASHSIZE fileName){
    ins_arcModel = arcModel;
    ins_arcTexture = arcTexture;
    ins_fileName = fileName;

    ins_mWorld = Matrix::constIdentity;
    ins_mWorld._11 = ins_mWorld._22 = ins_mWorld._33 = 5.f;

    Validate();
}

void ObjDwarf::Update(float delta){
    static const float fMoveSpeed = 5;
    static const float fRotateSpeed = 1;

    static const Matrix mScale = Matrix(
        5, 0, 0, 0,
        0, 5, 0, 0,
        0, 0, 5, 0,
        0, 0, 0, 1
    );

    static Vector3 vPos = Vector3(0, 0, 0);
    Matrix mTrans;

    static float fAngle = 0;
    Matrix mRotate;

    if (Core::Input::KeyDown(Core::Input::DK_E)){
        fAngle += delta * fRotateSpeed;
        if (fAngle > PIf * 2)fAngle = 0;
    }
    else if (Core::Input::KeyDown(Core::Input::DK_Q)){
        fAngle -= delta * fRotateSpeed;
        if (fAngle < 0)fAngle = PIf * 2;
    }

    if (Core::Input::KeyDown(Core::Input::DK_S))vPos.z += delta * fMoveSpeed;
    else if (Core::Input::KeyDown(Core::Input::DK_W))vPos.z -= delta * fMoveSpeed;
    if (Core::Input::KeyDown(Core::Input::DK_D))vPos.x += delta * fMoveSpeed;
    else if (Core::Input::KeyDown(Core::Input::DK_A))vPos.x -= delta * fMoveSpeed;

    MatrixRotationY(&mRotate, fAngle);
    MatrixTranslation(&mTrans, &vPos);

    ins_mWorld = mScale * mRotate * mTrans;
}

struct _CALLBACK_ARG{
    ID3DXMesh* mesh;
    DWORD index;
};
static HRESULT _draw_callback(void* rawObj){
    auto obj = (_CALLBACK_ARG*)rawObj;

    return obj->mesh->DrawSubset(obj->index);
}
void ObjDwarf::Draw(Object::Shader* shader){
    Matrix matTmp, matTmp2;
    auto drawPartFunc = [=](DWORD i)->void{
        _CALLBACK_ARG callbackArg = { ins_mesh, i };
        auto curTex = ins_texTable[i];
        auto curSpec = ins_texSpecTable[i];

        Core::Graphic::SetMaterial(&ins_mtrlTable[i]);

        {
            shader->SetVector("k_d", (const Vector4*)&ins_mtrlTable[i].Diffuse);
            shader->SetVector("k_a", (const Vector4*)&ins_mtrlTable[i].Ambient);
            shader->SetVector("k_s", (const Vector4*)&ins_mtrlTable[i].Specular);
            shader->SetFloat("k_p", ins_mtrlTable[i].Power);
        }


        Core::Graphic::SetTexture(0, curTex ? curTex->GetTexture() : ins_texWhite->GetTexture());
        Core::Graphic::SetTexture(1, curSpec ? curSpec->GetTexture() : ins_texWhite->GetTexture());

        shader->IteratePass(0, _draw_callback, &callbackArg);
    };

    Core::Graphic::SetTransform(D3DTS_WORLD, &ins_mWorld);

    {
        Core::Graphic::GetTransform(D3DTS_VIEW, &matTmp2);

        matTmp = ins_mWorld * matTmp2;
        shader->SetMatrix("matWV", &matTmp);
    }
    {
        Core::Graphic::GetTransform(D3DTS_PROJECTION, &matTmp2);

        matTmp *= matTmp2;
        shader->SetMatrix("matWVP", &matTmp);
    }

    Core::Graphic::SetVertexDeclaration(ins_vertDecl);

    for (auto i = decltype(ins_partConut){0}; i < ins_partConut; ++i){
        drawPartFunc(i);
    }
}