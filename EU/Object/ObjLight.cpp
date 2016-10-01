#include"pch.h"
#include"pch.h"

#include"Public/Public.h"
#include"Object.h"

ObjLight::ObjLight() : ins_matWorld(Matrix::constIdentity){}
ObjLight::~ObjLight(){}

ObjLight* ObjLight::Create(ARCHIVE_HASHSIZE texBase){
    auto _new = Global::Alloc::NewCustomAligned<ObjLight>(32);

    if (!(_new->ins_texBase = Object::ExternalTexture::Read(&arcSprites, texBase)))goto FAILED_FUNC;

    {
        Vector2 planSize;

        planSize.x = _new->ins_texBase->GetInfo()->Width;
        planSize.y = _new->ins_texBase->GetInfo()->Height;

        if (!(_new->ins_sprFrame = BasePlane::Create(&planSize, &Vector2(-0.5f, -0.5f))))goto FAILED_FUNC;
    }

    return _new;

FAILED_FUNC:
    RELEASE(_new);
    return nullptr;
}

void ObjLight::Update(float delta, const Matrix* matVP){
    ins_matWVP = ins_matWorld * (*matVP);
}

static HRESULT _draw_callback(void* rawObj){
    auto obj = (BasePlane*)rawObj;
    return obj->Draw();
}
void ObjLight::DrawBase(){
    shadBasic->SetMatrix("matWVP", &ins_matWVP);

    {
        Core::Graphic::SetTexture(0, ins_texBase->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _draw_callback, ins_sprFrame);
}