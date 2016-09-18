#include"pch.h"
#include"StageTest.h"

#include"ArchiveTable.h"

using namespace JBF;
using namespace JBF::Global::Alloc;
using namespace JBF::Core;

StageTest stgTest;

void StageTest::Init(){
    {
        Graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        Graphic::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    {
        Object::EmptyTexture::INFO inf;
        inf.pool = D3DPOOL_MANAGED;
        inf.usage = D3DUSAGE_RENDERTARGET;
        inf.format = Core::Graphic::GetDisplayInfo()->Format;
        inf.mipLevels = 1;
        inf.width = Core::Graphic::GetDisplayInfo()->Width;
        inf.height = Core::Graphic::GetDisplayInfo()->Height;
        faceGame = Object::EmptyTexture::Create(&inf);
    }

    {
        objCamera = NewCustomAligned<ObjCamera>(32);

        objHuman = NewCustomAligned<ObjHuman>(32);
    }

    {
        objCamera->Init();
    }
}
void StageTest::Cleanup(){
    RELEASE(faceGame);

    DeleteCustomAligned(objCamera);

    DeleteCustomAligned(objHuman);
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


    Core::Graphic::SetRenderTarget(0, surfOrg);
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