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

    Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
    Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _drawCallback, ins_sprFrame);
}
void StageTitle::ins_drawTextureRGBZero(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadRGBZero->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texture->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
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

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    {
        Core::Graphic::SetTexture(1, texLight->GetTexture());

        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadLightExtract->IteratePass(0, _drawCallback, ins_sprFrame);
}
void StageTitle::ins_drawTextureLightCombine(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBackLight, const Object::EmptyTexture* texFrontLight){
    shadLightCombine->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texBase->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    {
        Core::Graphic::SetTexture(1, texBackLight->GetTexture());

        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    {
        Core::Graphic::SetTexture(2, texFrontLight->GetTexture());

        Core::Graphic::SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadLightCombine->IteratePass(0, _drawCallback, ins_sprFrame);
}

void StageTitle::ins_drawTextureUpDownFilter(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadBasic->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texture->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _drawCallback, ins_sprFrame);
}

void StageTitle::ins_drawTextureBrighRegion(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBackLight, const Object::EmptyTexture* texFrontLight){
    shadBright->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texBase->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    {
        Core::Graphic::SetTexture(1, texBackLight->GetTexture());

        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    {
        Core::Graphic::SetTexture(2, texFrontLight->GetTexture());

        Core::Graphic::SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBright->IteratePass(0, _drawCallback, ins_sprFrame);
}

void StageTitle::ins_drawTextureBlurHorz(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadBlur->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texture->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBlur->IteratePass("main_h", 0, _drawCallback, ins_sprFrame);
}
void StageTitle::ins_drawTextureBlurVert(const Matrix* matWVP, const Object::EmptyTexture* texture){
    shadBlur->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texture->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBlur->IteratePass("main_v", 0, _drawCallback, ins_sprFrame);
}

void StageTitle::ins_drawTextureBloomCombine(const Matrix* matWVP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBloom){
    shadBloomCombine->SetMatrix("matWVP", matWVP);

    {
        Core::Graphic::SetTexture(0, texBase->GetTexture());

        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    {
        Core::Graphic::SetTexture(1, texBloom->GetTexture());

        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Core::Graphic::SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        Core::Graphic::SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        Core::Graphic::SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }

    ins_sprFrame->SendFaceInfo();

    shadBloomCombine->IteratePass(0, _drawCallback, ins_sprFrame);
}