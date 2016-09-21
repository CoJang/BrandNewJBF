#include"pch.h"
#include"StageTest.h"
#include"ArchiveTable.h"

using namespace JBF;
using namespace JBF::Global::Alloc;
using namespace JBF::Core;

StageTest stgTest;

#define _NAME_PLANE _T("JN-4.x")
#define NAME_PLANE JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(_NAME_PLANE, tstrlen(_NAME_PLANE))

void StageTest::Init(){
    {
        Graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        Graphic::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    {
        bGridSwitch = true;
        bAxisSwitch = true;
    }

    {
        objCamera = ObjCamera::Create();
        objFont = ObjFont::GetInstance();

        objGrid = ObjGrid::Create(100, 5.f);
        objAxis = ObjAxis::Create(5.f);
    }

    {
        objPlane = ObjPlane::Create(&arcModels, &arcTextures, NAME_PLANE);
    }

    {
        objFont->Init();
    }
}
void StageTest::Cleanup(){
    {
        RELEASE(objCamera);

        RELEASE(objGrid);
        RELEASE(objAxis);

        ObjFont::Release();
    }

    {
        RELEASE(objPlane);
    }
}

void StageTest::Update(float delta){
    static const float speed = 10.f;
    static Vector3 vCamTarget(0.f, 0.f, 0.f);

    static const Vector3 vPlaneCenter(0.f, 0.f, 0.f);
    static const float fPlaneTurnRad = 30.f;
    static const float fPlaneSpeed = 1.f;

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

    if (Core::Input::KeyPressed(Core::Input::DK_F2))bGridSwitch ^= true;
    if (Core::Input::KeyPressed(Core::Input::DK_F3))bAxisSwitch ^= true;

    if (Core::Input::KeyDown(Core::Input::DK_W))vCamTarget.z += speed * delta;
    else if (Core::Input::KeyDown(Core::Input::DK_S))vCamTarget.z -= speed * delta;
    if (Core::Input::KeyDown(Core::Input::DK_A))vCamTarget.x -= speed * delta;
    else if (Core::Input::KeyDown(Core::Input::DK_D))vCamTarget.x += speed * delta;

    //objCamera->Update(&vCamTarget);

    objCamera->Update(objPlane->GetPosition());
    if(bAxisSwitch)objAxis->Update(&Vector3(0.f, 0.f, 0.f));

    objPlane->TurnAround(&vPlaneCenter, fPlaneTurnRad, fPlaneSpeed);
    objPlane->Update(delta);
}
void StageTest::Draw(){
    if (FAILED(Graphic::GetDevice()->BeginScene()))return;
    Graphic::GetDevice()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.f),
        1.f,
        0
    );

    {
        objPlane->Draw();
    }

    {
        if(bGridSwitch)objGrid->Draw();
        if(bAxisSwitch)objAxis->Draw();
    }

    Graphic::GetDevice()->EndScene();
}