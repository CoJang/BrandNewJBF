#include"pch.h"

#include"Public/Public.h"
#include"Object.h"

ObjBackground::ObjBackground() : ins_texBase(nullptr), ins_texLightMask(nullptr), ins_sprFrame(nullptr){}
ObjBackground::~ObjBackground(){}

ObjBackground* ObjBackground::Create(ARCHIVE_HASHSIZE texBase, ARCHIVE_HASHSIZE texLightMask){
    auto _new = new ObjBackground();

    if (!(_new->ins_texBase = Object::ExternalTexture::Read(&arcSprites, texBase)))goto FAILED_FUNC;

    {
        Vector2 planSize;

        planSize.x = _new->ins_texBase->GetInfo()->Width;
        planSize.y = _new->ins_texBase->GetInfo()->Height;

        if(!(_new->ins_sprFrame = BasePlane::Create(&planSize)))goto FAILED_FUNC;
    }

    if (!(_new->ins_texLightMask = Object::ExternalTexture::Read(&arcSprites, texLightMask)))goto FAILED_FUNC;

    return _new;

FAILED_FUNC:
    RELEASE(_new);
    return nullptr;
}
void ObjBackground::Release(){
    if(ins_texBase)RELEASE(ins_texBase);
    if(ins_texLightMask)RELEASE(ins_texLightMask);

    if(ins_sprFrame)RELEASE(ins_sprFrame);

    delete this;
}

static HRESULT _draw_callback(void* rawObj){
    auto obj = (BasePlane*)rawObj;
    return obj->Draw();
}
void ObjBackground::DrawBase(const Matrix* matVP){
    shadBasic->SetMatrix("matWVP", matVP);

    {
        Core::Graphic::SetTexture(0, ins_texBase->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _draw_callback, ins_sprFrame);
}
void ObjBackground::DrawLightMask(const Matrix* matVP){
    shadBasic->SetMatrix("matWVP", matVP);

    {
        Core::Graphic::SetTexture(0, ins_texLightMask->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _draw_callback, ins_sprFrame);
}