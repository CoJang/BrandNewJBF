﻿#include"pch.h"
#include"StageTest.h"

#include"ArchiveTable.h"

#define _SPRITE_BACKGROUND _T("dummyBackground.png")
#define SPRITE_BACKGROUND JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SPRITE_BACKGROUND, tstrlen(_SPRITE_BACKGROUND))

#define _SPRITE_OBJECT _T("dummyHuman.png")
#define SPRITE_OBJECT JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SPRITE_OBJECT, tstrlen(_SPRITE_OBJECT))

#define _SHADER_BASIC _T("Basic_clamp.fxo")
#define SHADER_BASIC JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_BASIC, tstrlen(_SHADER_BASIC))

#define _SHADER_BRIGHT _T("Bright.fxo")
#define SHADER_BRIGHT JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_BRIGHT, tstrlen(_SHADER_BRIGHT))

#define _SHADER_BLUR _T("Blur.fxo")
#define SHADER_BLUR JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_BLUR, tstrlen(_SHADER_BLUR))

#define _SHADER_COMBINE _T("Combine.fxo")
#define SHADER_COMBINE JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_COMBINE, tstrlen(_SHADER_COMBINE))

using namespace JBF;
using namespace JBF::Global::Alloc;
using namespace JBF::Core;

StageTest stgTest;

void StageTest::Init(){
    ins_initFace();
    ins_initShader();
    ins_initFrame();
    ins_initObject();

    cfgBrightPassLevel = 0.04f;

    Graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    Graphic::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

    Core::Graphic::SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, FALSE);
}
void StageTest::ins_initFace(){
    size_t i;
    Object::EmptyTexture::INFO inf;

    {
        inf.pool = D3DPOOL_DEFAULT;
        inf.usage = D3DUSAGE_RENDERTARGET;
        inf.format = D3DFMT_A8B8G8R8;
        inf.mipLevels = 1;
        inf.width = Core::Graphic::GetDisplayInfo()->Width;
        inf.height = Core::Graphic::GetDisplayInfo()->Height;

        faceGame = Object::EmptyTexture::Create(&inf);
        for (i = 0; i < _countof(faceRenderPass); ++i)faceRenderPass[i] = Object::EmptyTexture::Create(&inf);
    }
}
void StageTest::ins_initShader(){
    shadBasic = Object::Shader::Read(&arcShaders, SHADER_BASIC);
    shadBright = Object::Shader::Read(&arcShaders, SHADER_BRIGHT);
    shadBlur = Object::Shader::Read(&arcShaders, SHADER_BLUR);
    shadCombine = Object::Shader::Read(&arcShaders, SHADER_COMBINE);
}
void StageTest::ins_initFrame(){
    Vector2 size = Vector2(Core::Graphic::GetDisplayInfo()->Width, Core::Graphic::GetDisplayInfo()->Height);

    matFrame = Matrix(
        2 / size.x, 0, 0, 0,
        0, 2 / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );
    matFrameDown2X = Matrix(
        1.f / size.x, 0, 0, 0,
        0, 1.f / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );
    matFrameUp2X = Matrix(
        4 / size.x, 0, 0, 0,
        0, 4 / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );

    sprFrame = BasePlane::Create(&size);
}
void StageTest::ins_initObject(){
    objCamera = ObjCamera::Create();

    objBackground = ObjTest::Create(SPRITE_BACKGROUND);
    objHuman = ObjTest::Create(SPRITE_OBJECT);
}

void StageTest::Cleanup(){
    ins_releaseObject();
    ins_releaseFrame();
    ins_releaseShader();
    ins_releaseFace();
}
void StageTest::ins_releaseFace(){
    size_t i;

    RELEASE(faceGame);
    for (i = 0; i < _countof(faceRenderPass); ++i)RELEASE(faceRenderPass[i]);
}
void StageTest::ins_releaseShader(){
    RELEASE(shadBasic);
    RELEASE(shadBright);
    RELEASE(shadBlur);
    RELEASE(shadCombine);
}
void StageTest::ins_releaseFrame(){
    RELEASE(sprFrame);
}
void StageTest::ins_releaseObject(){
    RELEASE(objCamera);

    RELEASE(objBackground);
    RELEASE(objHuman);
}

void StageTest::Update(float delta){
    static const float speed = 100.f;
    Vector3 vCamDir(0.f, 0.f, 0.f);

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

    if (Core::Input::KeyDown(Core::Input::DK_2))cfgBrightPassLevel += delta * 0.1f;
    else if (Core::Input::KeyDown(Core::Input::DK_1))cfgBrightPassLevel -= delta * 0.1f;

    if (Core::Input::KeyDown(Core::Input::DK_W))vCamDir.y += speed * delta;
    else if (Core::Input::KeyDown(Core::Input::DK_S))vCamDir.y -= speed * delta;
    if (Core::Input::KeyDown(Core::Input::DK_A))vCamDir.x -= speed * delta;
    else if (Core::Input::KeyDown(Core::Input::DK_D))vCamDir.x += speed * delta;

    objHuman->SetPosition(&vCamDir);
}

void StageTest::Draw(){
    const Matrix* matVP = objCamera->GetVPMatrix();

    IDirect3DSurface9* surfOrg;

    if (FAILED(Graphic::GetDevice()->BeginScene()))return;

    Core::Graphic::GetRenderTarget(0, &surfOrg);
    {
        Core::Graphic::SetRenderTarget(0, faceGame->GetSurface(0));
        ins_drawGame(matVP);
    }

    Graphic::GetDevice()->EndScene();

    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    if (FAILED(Graphic::GetDevice()->BeginScene()))return;

    { // Downfiltering
        Core::Graphic::SetRenderTarget(0, faceRenderPass[0]->GetSurface(0));
        ins_drawTextureOriginal(&matFrameDown2X, faceGame);
    }

    { // Extrace bright region
        Core::Graphic::SetRenderTarget(0, faceRenderPass[1]->GetSurface(0));
        ins_drawTextureBrighRegion(&matFrame, &cfgBrightPassLevel, faceRenderPass[0]);
    }

    { // apply horz blur
        Core::Graphic::SetRenderTarget(0, faceRenderPass[0]->GetSurface(0));
        ins_drawTextureBlurHorz(&matFrame, faceRenderPass[1]);
    }

    { // apply vert blur
        Core::Graphic::SetRenderTarget(0, faceRenderPass[1]->GetSurface(0));
        ins_drawTextureBlurVert(&matFrame, faceRenderPass[0]);
    }

    { // Upfiltering
        Core::Graphic::SetRenderTarget(0, faceRenderPass[0]->GetSurface(0));
        ins_drawTextureOriginal(&matFrameUp2X, faceRenderPass[1]);
    }

    { // combine base and bright
        Core::Graphic::SetRenderTarget(0, surfOrg);
        ins_drawTextureCombine(&matFrame, faceGame, faceRenderPass[0]);
    }

    Graphic::GetDevice()->EndScene();
}

static HRESULT _drawCallback(void* rawObj){
    auto obj = (BasePlane*)rawObj;

    return obj->Draw();
}

void StageTest::ins_drawGame(const Matrix* matVP){
    Graphic::GetDevice()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f),
        1.f,
        0
    );

    objBackground->Draw(matVP);
    objHuman->Draw(matVP);
}

void StageTest::ins_drawTextureOriginal(const Matrix* matWMP, const Object::EmptyTexture* texture){
    shadBasic->SetMatrix("matWVP", matWMP);
    Core::Graphic::SetTexture(0, texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _drawCallback, sprFrame);
}
void StageTest::ins_drawTextureBrighRegion(const Matrix* matWMP, const float* fBrightPassLevel, const Object::EmptyTexture* texture){
    shadBright->SetMatrix("matWVP", matWMP);
    shadBright->SetFloat("fLevel", *fBrightPassLevel);
    Core::Graphic::SetTexture(0, texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadBright->IteratePass(0, _drawCallback, sprFrame);
}
void StageTest::ins_drawTextureBlurHorz(const Matrix* matWMP, const Object::EmptyTexture* texture){
    shadBlur->SetMatrix("matWVP", matWMP);
    Core::Graphic::SetTexture(0, texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadBlur->IteratePass("main_h", 0, _drawCallback, sprFrame);
}
void StageTest::ins_drawTextureBlurVert(const Matrix* matWMP, const Object::EmptyTexture* texture){
    shadBlur->SetMatrix("matWVP", matWMP);
    Core::Graphic::SetTexture(0, texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadBlur->IteratePass("main_v", 0, _drawCallback, sprFrame);
}
void StageTest::ins_drawTextureCombine(const Matrix* matWMP, const Object::EmptyTexture* texBase, const Object::EmptyTexture* texBloom){
    shadCombine->SetMatrix("matWVP", matWMP);

    Core::Graphic::SetTexture(0, texBase->GetTexture());
    Core::Graphic::SetTexture(1, texBloom->GetTexture());

    sprFrame->SendFaceInfo();

    shadCombine->IteratePass(0, _drawCallback, sprFrame);
}