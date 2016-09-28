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

    Core::Graphic::SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    Core::Graphic::SetRenderState(D3DRS_LIGHTING, FALSE);

    Core::Graphic::SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, FALSE);

    return S_OK;
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
        {
            Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

            ins_drawTextureOriginal(&ins_matFrame, ins_faceObject);
        }
        ins_drawForeground(matVP);
    }

    { // Background light mask draw
        Core::Graphic::SetRenderTarget(0, ins_faceLightMask[0]->GetSurface(0));

        ins_drawBackgroundLightMask(matVP);
        ins_drawTextureRGBZero(&ins_matFrame, ins_faceObject);
    }
    { // Foreground light mask draw
        Core::Graphic::SetRenderTarget(0, ins_faceLightMask[1]->GetSurface(0));

        ins_drawForegroundLightMask(matVP);
    }

    { // Background light draw
        Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));

        ins_drawBackgroundLight(matVP);
    }
    { // Foreground light draw
        Core::Graphic::SetRenderTarget(0, ins_faceLight[1]->GetSurface(0));

        ins_drawForegroundLight(matVP);
    }

    { // Extract lighted background region
        Core::Graphic::SetRenderTarget(0, ins_faceLight[2]->GetSurface(0));

        ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[0], ins_faceLightMask[0]);
    }
    { // Extract lighted foreground region
        Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));

        ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[1], ins_faceLightMask[1]);
    }
    {
        // lighted background region was moved to index 0
        // lighted foreground region was moved to index 1

        std::swap(ins_faceLight[0], ins_faceLight[1]);
        std::swap(ins_faceLight[2], ins_faceLight[0]);
    }

    { // Apply light on game scene
        //Core::Graphic::SetRenderTarget(0, ins_faceTemp[0]->GetSurface(0));
        Core::Graphic::SetRenderTarget(0, surfOrg);

        ins_drawTextureLightCombine(&ins_matFrame, ins_faceGame, ins_faceLight[0], ins_faceLight[1]);
    }

    Core::Graphic::GetDevice()->EndScene();
}