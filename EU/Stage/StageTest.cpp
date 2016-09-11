#include"pch.h"
#include"StageTest.h"

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
        bGridSwitch = true;
        bAxisSwitch = true;
    }

    {
        objCamera = NewCustomAligned<ObjCamera>(32);
        objFont = ObjFont::GetInstance();

        objGrid = new ObjGrid();
        objAxis = NewCustomAligned<ObjAxis>(32);
    }

    {
        objCamera->Init();
        objFont->Init();

        objGrid->Init(100, 5.f);
        objAxis->Init(5.f);
    }
}
void StageTest::Cleanup(){
    DeleteCustomAligned(objCamera);
    ObjFont::Release(objFont);

    delete objGrid;
    DeleteCustomAligned(objAxis);
}

void StageTest::Update(float delta){
    static const float speed = 10.f;
    static Vector3 vCamTarget(0.f, 0.f, 0.f);

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

    objCamera->Update(&vCamTarget);
    if(bAxisSwitch)objAxis->Update(&Vector3(0.f, 0.f, 0.f));
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
        if(bGridSwitch)objGrid->Draw();
        if(bAxisSwitch)objAxis->Draw();
    }

    Graphic::GetDevice()->EndScene();
}