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

    Object::EmptyTexture* faceCombinedGame;
    Object::EmptyTexture* faceTemp[2];

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

    { // Combine game scene with masked light
        { // Object draw
            Core::Graphic::GetDevice()->ColorFill(ins_faceObject->GetSurface(0), nullptr, 0);
            Core::Graphic::SetRenderTarget(0, ins_faceObject->GetSurface(0));

            {
Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

ins_drawObject(matVP);
            }
        }

        { // Ingame draw
        Core::Graphic::SetRenderTarget(0, ins_faceGame->GetSurface(0));

        {
            Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

            ins_drawBackground(matVP);
        }
        {
            Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

            ins_drawTextureOriginal(&ins_matFrame, ins_faceObject);
            ins_drawForeground(matVP);
        }
        }

        { // Background light mask draw
            Core::Graphic::SetRenderTarget(0, ins_faceLightMask[0]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawBackgroundLightMask(matVP);
            }
            {
                Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

                ins_drawTextureRGBZero(&ins_matFrame, ins_faceObject);
            }
        }
        { // Foreground light mask draw
            Core::Graphic::SetRenderTarget(0, ins_faceLightMask[1]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawForegroundLightMask(matVP);
            }
        }

        { // Background light draw
            Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawBackgroundLight(matVP);
            }
        }
        { // Foreground light draw
            Core::Graphic::SetRenderTarget(0, ins_faceLight[1]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawForegroundLight(matVP);
            }
        }

        { // Extract lighted background region
            Core::Graphic::SetRenderTarget(0, ins_faceLight[2]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[0], ins_faceLightMask[0]);
            }
        }
        { // Extract lighted foreground region
            Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[1], ins_faceLightMask[1]);
            }
        }

        { // Apply light on game scene
            Core::Graphic::SetRenderTarget(0, ins_faceObject->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureLightCombine(&ins_matFrame, ins_faceGame, ins_faceLight[2], ins_faceLight[0]);
            }
        }
    }

    { // temp
        Core::Graphic::SetRenderTarget(0, surfOrg);

        {
            Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

            ins_drawTextureOriginal(&ins_matFrame, ins_faceObject);
        }

        Core::Graphic::GetDevice()->EndScene();
        return;
    }

    { // Apply bloom
        {
            // Face light[2] was used as game scene
            faceCombinedGame = ins_faceObject;

            // Face light[0] and [1] was used as temporary surface
            faceTemp[0] = ins_faceLight[0];
            faceTemp[1] = ins_faceLight[2];
        }

        { // Extrace bright region
            Core::Graphic::SetRenderTarget(0, faceTemp[0]->GetSurface(0));
            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBrighRegion(&ins_matFrame, faceCombinedGame, ins_faceLightMask[0], ins_faceLightMask[1]);
            }
        }

        { // Downfiltering
            Core::Graphic::GetDevice()->ColorFill(faceTemp[1]->GetSurface(0), nullptr, 0);
            Core::Graphic::SetRenderTarget(0, faceTemp[1]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureUpDownFilter(&ins_matFrameDown2X, faceTemp[0]);
            }
        }

        { // Apply horz blur
            Core::Graphic::SetRenderTarget(0, faceTemp[0]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBlurHorz(&ins_matFrame, faceTemp[1]);
            }
        }
        { // Apply vert blur
            Core::Graphic::SetRenderTarget(0, faceTemp[1]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBlurVert(&ins_matFrame, faceTemp[0]);
            }
        }

        { // Upfiltering
            Core::Graphic::SetRenderTarget(0, faceTemp[0]->GetSurface(0));

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureUpDownFilter(&ins_matFrameUp2X, faceTemp[1]);
            }
        }

        { // combine base and bright
            Core::Graphic::SetRenderTarget(0, surfOrg);

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBloomCombine(&ins_matFrame, faceCombinedGame, faceTemp[0]);
            }
        }
    }

    Core::Graphic::GetDevice()->EndScene();
}