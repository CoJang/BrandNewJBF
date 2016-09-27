#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


void StageTitle::Init(){
    ins_initFace();
    ins_initFrame();
    ins_initObject();

    Validate();
}
HRESULT StageTitle::Validate(){
    ins_initMatrix();
}

void StageTitle::Cleanup(){
    Invalidate();

    ins_releaseObject();
    ins_releaseFrame();
    ins_releaseFace();
}
void StageTitle::Invalidate(){
    ins_releaseMatrix();
}

void StageTitle::Update(float delta){

}
void StageTitle::Draw(){
    IDirect3DSurface9* surfOrg;
    const Matrix* matVP = objCamera->GetVPMatrix();

    if (FAILED(Core::Graphic::GetDevice()->BeginScene()))return;

    Core::Graphic::GetDevice()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        0x00000000,
        1.f,
        0
    );

    Core::Graphic::GetRenderTarget(0, &surfOrg);

    { // Object draw
        Core::Graphic::SetRenderTarget(0, ins_faceObject->GetSurface(0));

        ins_drawObject(matVP);
    }

    { // Ingame draw
        Core::Graphic::SetRenderTarget(0, ins_faceGame->GetSurface(0));

        ins_drawBackground(matVP);
        ins_drawTextureOriginal(&ins_matFrame, ins_faceObject);
        ins_drawForeground(matVP);
    }

    { // Light mask draw
        Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));

        ins_drawBackgroundLightMask(matVP);
        ins_drawTextureRGBZero(&ins_matFrame, ins_faceObject);
        ins_drawForegroundLightMask(matVP);
    }

    { // Light draw
        Core::Graphic::SetRenderTarget(0, ins_faceLight[1]->GetSurface(0));

        ins_drawLight(matVP);
    }

    { // Extract lighted region
        Core::Graphic::SetRenderTarget(0, ins_faceLight[2]->GetSurface(0));

        ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[1], ins_faceLight[0]);
    }

    { // Apply light on game scene
        Core::Graphic::SetRenderTarget(0, ins_faceTemp[0]->GetSurface(0));


    }

    Core::Graphic::GetDevice()->EndScene();
}