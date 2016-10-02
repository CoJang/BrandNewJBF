#include"pch.h"

#include"Public/Public.h"
#include"Stage.h"


void StageTitle::Init(){
    //BOOL windowed = FALSE;
    //Core::Graphic::Resize(nullptr, &windowed);

    { // test
        RECT rc = { 0, 0, 1366, 768 };
        {
            AdjustWindowRect(&rc, GetWindowLong(GetHandle(), GWL_STYLE), FALSE);
            {
                rc.right -= rc.left;
                rc.left = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) >> 1;
                rc.bottom -= rc.top;
                rc.top = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) >> 1;
            }

            MoveWindow(GetHandle(), rc.left, rc.top, rc.right, rc.bottom, TRUE);
        }
    }

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
    static Vector3 vPos = Vector3(0, -365, 0);

#ifdef _DEBUG
    {
        static float timeElasped = 0.f;
        timeElasped += delta;
        if (timeElasped > 1.f){
            timeElasped = 0.f;

            LOGGING_NORMAL(_T("FPS: %.1f"), 1 / delta);
        }
    }
#endif

    if (Core::Input::KeyDown(Core::Input::DK_A))vPos.x -= delta * 150.f;
    else if (Core::Input::KeyDown(Core::Input::DK_D))vPos.x += delta * 150.f;
    ins_objPlayer->SetPosition(&vPos);

    for (auto i : ins_objTable)i->Update(delta, objCamera->GetVPMatrix());
}

void StageTitle::Draw(){
    HRESULT hr;
    IDirect3DSurface9* surfOrg;
    const Matrix* matVP = objCamera->GetVPMatrix();

    Object::EmptyTexture* faceCombinedGame;
    Object::EmptyTexture* faceTemp[2];

    hr = Core::Graphic::GetDevice()->BeginScene();
    if (FAILED(hr))return;

    hr = Core::Graphic::GetDevice()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        0x00000000,
        1.f,
        0
    );
    if (FAILED(hr))return;

    hr = Core::Graphic::GetRenderTarget(0, &surfOrg);
    if (FAILED(hr))return;

    { // Combine game scene with masked light
        { // Object draw
            hr = Core::Graphic::GetDevice()->ColorFill(ins_faceObject->GetSurface(0), nullptr, 0);
            if (FAILED(hr))return;

            hr = Core::Graphic::SetRenderTarget(0, ins_faceObject->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

                ins_drawObject(matVP);
                ins_drawForeground(matVP);
            }
        }

        { // Ingame draw
            hr = Core::Graphic::SetRenderTarget(0, ins_faceGame->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawBackground(matVP);
            }
            {
                Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

                ins_drawTextureOriginal(&ins_matFrame, ins_faceObject);
            }
        }

        { // Background light mask draw
            hr = Core::Graphic::SetRenderTarget(0, ins_faceLightMask[0]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawBackgroundLightMask(matVP);
            }
        }
        { // Foreground light mask draw
            hr = Core::Graphic::SetRenderTarget(0, ins_faceLightMask[1]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawForegroundLightMask(matVP);
            }
        }

        { // Background light draw
            hr = Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawBackgroundLight(matVP);
            }
            { // Extract upper layer drew object
                Core::Graphic::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

                ins_drawTextureRGBZero(&ins_matFrame, ins_faceObject);
            }
        }
        { // Foreground light draw
            hr = Core::Graphic::SetRenderTarget(0, ins_faceLight[1]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawForegroundLight(matVP);
            }
        }

        { // Extract lighted background region
            hr = Core::Graphic::SetRenderTarget(0, ins_faceLight[2]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[0], ins_faceLightMask[0]);
            }
        }
        { // Extract lighted foreground region
            hr = Core::Graphic::SetRenderTarget(0, ins_faceLight[0]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureLightExtract(&ins_matFrame, ins_faceLight[1], ins_faceLightMask[1]);
            }
        }

        { // Apply light on game scene
            hr = Core::Graphic::SetRenderTarget(0, ins_faceObject->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureLightCombine(&ins_matFrame, ins_faceGame, ins_faceLight[2], ins_faceLight[0]);
            }
        }
    }

    { // temp
        hr = Core::Graphic::SetRenderTarget(0, surfOrg);
        if (FAILED(hr))return;

        {
            Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

            ins_drawTextureOriginal(&ins_matFrame, ins_faceObject);
        }

        hr = Core::Graphic::GetDevice()->EndScene();
        if (FAILED(hr))return;

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
            hr = Core::Graphic::SetRenderTarget(0, faceTemp[0]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBrighRegion(&ins_matFrame, faceCombinedGame, ins_faceLightMask[0], ins_faceLightMask[1]);
            }
        }

        { // Downfiltering
            hr = Core::Graphic::GetDevice()->ColorFill(faceTemp[1]->GetSurface(0), nullptr, 0);
            if (FAILED(hr))return;

            hr = Core::Graphic::SetRenderTarget(0, faceTemp[1]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureUpDownFilter(&ins_matFrameDown2X, faceTemp[0]);
            }
        }

        { // Apply horz blur
            hr = Core::Graphic::SetRenderTarget(0, faceTemp[0]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBlurHorz(&ins_matFrame, faceTemp[1]);
            }
        }
        { // Apply vert blur
            hr = Core::Graphic::SetRenderTarget(0, faceTemp[1]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBlurVert(&ins_matFrame, faceTemp[0]);
            }
        }

        { // Upfiltering
            hr = Core::Graphic::SetRenderTarget(0, faceTemp[0]->GetSurface(0));
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureUpDownFilter(&ins_matFrameUp2X, faceTemp[1]);
            }
        }

        { // combine base and bright
            hr = Core::Graphic::SetRenderTarget(0, surfOrg);
            if (FAILED(hr))return;

            {
                Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

                ins_drawTextureBloomCombine(&ins_matFrame, faceCombinedGame, faceTemp[0]);
            }
        }
    }

    hr = Core::Graphic::GetDevice()->EndScene();
    if (FAILED(hr))return;
}