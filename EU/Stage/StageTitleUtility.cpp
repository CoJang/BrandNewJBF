#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


static HRESULT _drawCallback(void* rawObj){
    auto obj = (BasePlane*)rawObj;
    return obj->Draw();
}


void StageTitle::ins_drawTextureOriginal(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadBasic->SetMatrix("matWVP", matWVP);

    Core::Graphic::SetTexture(0, texture->GetTexture());

    Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
    Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _drawCallback, ins_sprFrame);
}
void StageTitle::ins_drawTextureRGBZero(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadRGBZero->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texture->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
    }

    ins_sprFrame->SendFaceInfo();

    shadRGBZero->IteratePass(0, _drawCallback, ins_sprFrame);
}

void StageTitle::ins_drawTextureLightExtract(const Matrix* matWVP, const Object::EmptyTexture* texLight, const Object::EmptyTexture* texMask){
    shadLightExtract->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texMask->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
    }
    {
        Core::Graphic::SetTexture(1, texLight->GetTexture());

        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
    }

    ins_sprFrame->SendFaceInfo();

    shadLightExtract->IteratePass(0, _drawCallback, ins_sprFrame);
}

void StageTitle::ins_drawTextureUpDownFilter(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadBasic->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texture->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _drawCallback, ins_sprFrame);
}