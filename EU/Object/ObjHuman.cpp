﻿#include"pch.h"
#include"Object.h"

#include"ArchiveTable.h"
#define SHADER_NAME _T("Basic_wrap.fxo")
#define FILE_NAME _T("dummyHuman.png")

ObjHuman::ObjHuman(){
    ins_shader = Object::Shader::Read(&arcShaders, Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(SHADER_NAME, tstrlen(SHADER_NAME)));

    {
        Vector2 planSize;

        ins_texture = Object::ExternalTexture::Read(&arcSprites, Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(FILE_NAME, tstrlen(FILE_NAME)));
        planSize.x = ins_texture->GetInfo()->Width;
        planSize.y = ins_texture->GetInfo()->Height;

        ins_sprite = BasePlane::Create(&planSize);
    }

    ins_matWorld = Matrix::constIdentity;
}
ObjHuman::~ObjHuman(){
    RELEASE(ins_shader);
    RELEASE(ins_texture);
    RELEASE(ins_sprite);
}

ObjHuman* ObjHuman::Create(){
    auto _new = Global::Alloc::NewCustomAligned<ObjHuman>(32);
    return _new;
}
void ObjHuman::Release(){ Global::Alloc::DeleteCustomAligned(this); }

static HRESULT _draw_callback(void* rawObj){
    auto obj = (BasePlane*)rawObj;

    return obj->Draw();
}
bool ObjHuman::Draw(const Matrix* matVP){
    Matrix matWVP;

    matWVP = ins_matWorld * (*matVP);

    Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    ins_shader->SetMatrix("matWVP", &matWVP);
    ins_shader->SetTexture("texMain", ins_texture->GetTexture());

    ins_sprite->SendFaceInfo();

    ins_shader->IteratePass(0, _draw_callback, this->ins_sprite);

    return true;
}