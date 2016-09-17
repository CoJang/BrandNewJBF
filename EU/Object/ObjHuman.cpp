#include"pch.h"
#include"Object.h"

#include"ArchiveTable.h"
#define SHADER_NAME _T("Basic.fxo")
#define FILE_NAME _T("dummyHuman.bmp")

ObjHuman::ObjHuman(){
    ins_shader = Object::Shader::Read(&arcShaders, Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(SHADER_NAME, tstrlen(SHADER_NAME)));

    {
        D3DXVECTOR2 planSize;

        ins_texture = Object::ExternalTexture::Read(&arcSprites, Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(FILE_NAME, tstrlen(FILE_NAME)));
        planSize.x = ins_texture->GetInfo()->Width;
        planSize.y = ins_texture->GetInfo()->Height;

        ins_sprite = BasePlane::Create(&planSize);
    }

    ins_world = Matrix::constIdentity;
}
ObjHuman::~ObjHuman(){
    RELEASE(ins_shader);
    RELEASE(ins_texture);
    RELEASE(ins_sprite);
}

static HRESULT _draw_callback(void* rawObj){
    auto obj = (BasePlane*)rawObj;

    return obj->Draw();
}
bool ObjHuman::Draw(){
    Matrix matWVP, matTmp;

    Core::Graphic::GetTransform(D3DTS_VIEW, &matTmp);
    Core::Graphic::GetTransform(D3DTS_PROJECTION, &matWVP);
    matTmp *= matWVP;
    matWVP = ins_world * matTmp;

    ins_shader->SetMatrix("matWVP", &matWVP);
    ins_shader->SetTexture("texMain", ins_texture->GetTexture());

    ins_sprite->SendFaceInfo();

    ins_shader->IteratePass(0, _draw_callback, this->ins_sprite);

    return true;
}