#include"pch.h"
#include"StageTest.h"

#include"ArchiveTable.h"

#define _SHADER_FRAME _T("Basic_clamp.fxo")
#define SHADER_FRAME JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_SHADER_FRAME, tstrlen(_SHADER_FRAME))

using namespace JBF;
using namespace JBF::Global::Alloc;
using namespace JBF::Core;

StageTest stgTest;

void StageTest::Init(){
    ins_initFace();
    ins_initFrame();
    ins_initObject();

    Graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    Graphic::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

    Core::Graphic::SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Core::Graphic::SetRenderState(D3DRS_ZENABLE, FALSE);
}
void StageTest::ins_initFace(){
    Object::EmptyTexture::INFO inf;

    inf.pool = D3DPOOL_DEFAULT;
    inf.usage = D3DUSAGE_RENDERTARGET;
    inf.format = Core::Graphic::GetDisplayInfo()->Format;
    inf.mipLevels = 1;
    inf.width = Core::Graphic::GetDisplayInfo()->Width;
    inf.height = Core::Graphic::GetDisplayInfo()->Height;

    faceGame = Object::EmptyTexture::Create(&inf);
}
void StageTest::ins_initFrame(){
    Vector2 size = Vector2(Core::Graphic::GetDisplayInfo()->Width, Core::Graphic::GetDisplayInfo()->Height);

    sprShader = Object::Shader::Read(&arcShaders, SHADER_FRAME);

    matFrame = Matrix(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -size.x * 0.5f, size.y * 0.5f, 0, 1
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
    ins_releaseFace();
}
void StageTest::ins_releaseFace(){
    RELEASE(faceGame);
}
void StageTest::ins_releaseFrame(){
    RELEASE(sprShader);

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
    const Matrix matFP = matFrame * (*objCamera->GetProjectionMatrix());

    IDirect3DSurface9* surfOrg;

    Core::Graphic::GetRenderTarget(0, &surfOrg);
    Core::Graphic::SetRenderTarget(0, faceGame->GetSurface(0));
    ins_drawGame(matVP);

    Core::Graphic::SetRenderTarget(0, surfOrg);
    ins_drawScene(&matFP);
    
}
void StageTest::ins_drawGame(const Matrix* matVP){
    if (FAILED(Graphic::GetDevice()->BeginScene()))return;
    Graphic::GetDevice()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.f),
        1.f,
        0
    );

    objHuman->Draw(matVP);

    Graphic::GetDevice()->EndScene();
}
void StageTest::ins_drawScene(const Matrix* matP){
    Core::Graphic::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    sprShader->SetMatrix("matWVP", matP);
    sprShader->SetTexture("texMain", faceGame->GetTexture());

    sprFrame->SendFaceInfo();

    sprShader->IteratePass(
        0,
        [](void* rawObj)->HRESULT{
            BasePlane* obj = (decltype(obj))rawObj;
            return obj->Draw();
        },
        sprFrame
        );
}