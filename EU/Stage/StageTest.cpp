#include"pch.h"
#include"StageTest.h"

#include"ArchiveTable.h"

#define _SHADER_BASIC _T("Basic_clamp.fxo")
#define SHADER_BASIC JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_BASIC, tstrlen(_SHADER_BASIC))

#define _SHADER_DOWNCAST4X _T("Downcast4X.fxo")
#define SHADER_DOWNCAST4X JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_DOWNCAST4X, tstrlen(_SHADER_DOWNCAST4X))

#define _SHADER_BRIGHT _T("Bright.fxo")
#define SHADER_BRIGHT JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_BRIGHT, tstrlen(_SHADER_BRIGHT))

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
    Object::EmptyTexture::INFO inf;

    inf.pool = D3DPOOL_DEFAULT;
    inf.usage = D3DUSAGE_RENDERTARGET;
    inf.format = D3DFMT_A16B16G16R16F;
    inf.mipLevels = 1;
    inf.width = Core::Graphic::GetDisplayInfo()->Width;
    inf.height = Core::Graphic::GetDisplayInfo()->Height;

    faceGame = Object::EmptyTexture::Create(&inf);
    faceBrigh = Object::EmptyTexture::Create(&inf);
}
void StageTest::ins_initShader(){
    shadBasic = Object::Shader::Read(&arcShaders, SHADER_BASIC);
    shadDowncast4X = Object::Shader::Read(&arcShaders, SHADER_DOWNCAST4X);
    shadBright = Object::Shader::Read(&arcShaders, SHADER_BRIGHT);
}
void StageTest::ins_initFrame(){
    Vector2 size = Vector2(Core::Graphic::GetDisplayInfo()->Width, Core::Graphic::GetDisplayInfo()->Height);

    matFrame = Matrix(
        2 / size.x, 0, 0, 0,
        0, 2 / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );
    matFrameDown4X = Matrix(
        0.5f / size.x, 0, 0, 0,
        0, 0.5f / size.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );

    sprFrame = BasePlane::Create(&size);
}
void StageTest::ins_initObject(){
    objCamera = ObjCamera::Create();

    objHuman = ObjHuman::Create();
}

void StageTest::Cleanup(){
    ins_releaseObject();
    ins_releaseFrame();
    ins_releaseShader();
    ins_releaseFace();
}
void StageTest::ins_releaseFace(){
    RELEASE(faceGame);
    RELEASE(faceBrigh);
}
void StageTest::ins_releaseShader(){
    RELEASE(shadBasic);
    RELEASE(shadDowncast4X);
    RELEASE(shadBright);
}
void StageTest::ins_releaseFrame(){
    RELEASE(sprFrame);
}
void StageTest::ins_releaseObject(){
    RELEASE(objCamera);

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

    if (Core::Input::KeyDown(Core::Input::DK_W))vCamDir.y += speed * delta;
    else if (Core::Input::KeyDown(Core::Input::DK_S))vCamDir.y -= speed * delta;
    if (Core::Input::KeyDown(Core::Input::DK_A))vCamDir.x -= speed * delta;
    else if (Core::Input::KeyDown(Core::Input::DK_D))vCamDir.x += speed * delta;

    objCamera->MoveDirection(&vCamDir);
}

void StageTest::Draw(){
    const Matrix* matVP = objCamera->GetVPMatrix();

    IDirect3DSurface9* surfOrg;

    Core::Graphic::GetRenderTarget(0, &surfOrg);
    Core::Graphic::SetRenderTarget(0, faceGame->GetSurface(0));
    ins_drawGame(matVP);

    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    Core::Graphic::SetRenderTarget(0, faceBrigh->GetSurface(0));
    ins_drawTextureDownCast4X(&matFrameDown4X, faceGame);

    Core::Graphic::SetRenderTarget(0, surfOrg);
    ins_drawTextureBrighRegion(&matFrame, faceBrigh);

    //ins_drawTextureOriginal(&matFrame, faceBrigh);
    
}

static HRESULT _drawCallback(void* rawObj){
    auto obj = (BasePlane*)rawObj;

    return obj->Draw();
}

void StageTest::ins_drawGame(const Matrix* matVP){
    if (FAILED(Graphic::GetDevice()->BeginScene()))return;
    Graphic::GetDevice()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
        1.f,
        0
    );

    objHuman->Draw(matVP);

    Graphic::GetDevice()->EndScene();
}

void StageTest::ins_drawTextureOriginal(const Matrix* matWMP, const Object::EmptyTexture* texture){
    shadBasic->SetMatrix("matWVP", matWMP);
    shadBasic->SetTexture("texMain", texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadBasic->IteratePass(0, _drawCallback, sprFrame);
}
void StageTest::ins_drawTextureDownCast4X(const Matrix* matWMP, const Object::EmptyTexture* texture){
    shadDowncast4X->SetMatrix("matWVP", matWMP);
    shadDowncast4X->SetTexture("texMain", texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadDowncast4X->IteratePass(0, _drawCallback, sprFrame);
}
void StageTest::ins_drawTextureBrighRegion(const Matrix* matWMP, const Object::EmptyTexture* texture){
    shadBright->SetMatrix("matWVP", matWMP);
    shadBright->SetFloat("fLevel", cfgBrightPassLevel);
    shadBright->SetTexture("texMain", texture->GetTexture());

    sprFrame->SendFaceInfo();

    shadBright->IteratePass(0, _drawCallback, sprFrame);
}