#include"pch.h"
#include"StageTest.h"
#include"ArchiveTable.h"

using namespace JBF;
using namespace JBF::Global::Alloc;
using namespace JBF::Core;

StageTest stgTest;

void StageTest::Init(){
    ARCHIVE_HASHSIZE namePlane;

    {
        CfgParseINI::Value val;

        if (cfgINIReader.Read(_T("./tri_setting.ini"))){
            if (cfgINIReader.Get(_T("PlaneFile"), &val)){
                if(val.type == CfgParseINI::STRING)namePlane = Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(val.string, tstrlen(val.string));
            }
        }
    }

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
        objPlane = ObjPlane::Create(&arcModels, &arcTextures, namePlane);
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
    static const Vector3 vPlaneCenter(0.f, 5.f, 0.f);
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

    if (bGridSwitch)objGrid->Draw();

    {
        objPlane->Draw();
    }

    if (bAxisSwitch)objAxis->Draw();

    Graphic::GetDevice()->EndScene();
}