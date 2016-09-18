#include"pch.h"
#include"Object.h"

#include"ArchiveTable.h"

#define _SHADER_NAME _T("Basic_wrap.fxo")
#define SHADER_NAME Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_NAME, tstrlen(_SHADER_NAME)

ObjHuman::ObjHuman(ARCHIVE_HASHSIZE sprite){
    ins_shader = Object::Shader::Read(&arcShaders, SHADER_NAME));

    {
        Vector2 planSize;

        ins_texture = Object::ExternalTexture::Read(&arcSprites, sprite);
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

ObjHuman* ObjHuman::Create(ARCHIVE_HASHSIZE sprite){
    auto _new = Global::Alloc::NewCustomAligned<ObjHuman>(32, sprite);
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

void ObjHuman::SetPosition(Vector3* pos){
    Vector3* _pos = (decltype(pos))&ins_matWorld._41;
    *_pos += *pos;
    ins_matWorld._44 = 1.f;
}