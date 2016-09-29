#include"pch.h"
#include"StageTest.h"
#include"Public/Public.h"

#define RES_FILENAME(str) JBF::Global::Hash::X65599Generator<ARCHIVE_HASHSIZE, TCHAR>(str, tstrlen(str))

using namespace JBF;
using namespace JBF::Global::Alloc;
using namespace JBF::Core;

StageTest stgTest;

void StageTest::Init(){
    Global::Math::Point<WORD> clientSize;

    ARCHIVE_HASHSIZE namePlane;
    ARCHIVE_HASHSIZE nameDwarf;

    { // Read configuration
        CfgParseINI::Value val;

        if (cfgINIReader.Read(_T("./tri_setting.ini"))){
            if (cfgINIReader.Get(_T("ScreenX"), &val)){
                if (val.type == CfgParseINI::NUMBER){
                    clientSize.x = val.interger;
                }
            }
            if (cfgINIReader.Get(_T("ScreenY"), &val)){
                if (val.type == CfgParseINI::NUMBER){
                    clientSize.y = val.interger;
                }
            }

            if (cfgINIReader.Get(_T("PlaneFile"), &val)){
                if (val.type == CfgParseINI::STRING){
                    namePlane = RES_FILENAME(val.string);
                }
            }
            if (cfgINIReader.Get(_T("DwarfFile"), &val)){
                if (val.type == CfgParseINI::STRING){
                    nameDwarf = RES_FILENAME(val.string);
                }
            }
        }
    }

    { // Graphic setting
        Graphic::Resize(&clientSize);

        Graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        Graphic::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    { // Environment init
        ZeroMemory(&envLight, sizeof envLight);

        envLight.Type = D3DLIGHT_DIRECTIONAL;

        envLight.Diffuse = D3DXCOLOR(0.8, 0.8, 0.8, 1);
        envLight.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
        envLight.Specular = D3DXCOLOR(1, 1, 1, 1);

        envLight.Range = 1000.f;
    }

    { // Variable init
        bGridSwitch = true;
        bAxisSwitch = true;
    }

    { // Object create
        objCamera = ObjCamera::Create();
        objFont = ObjFont::GetInstance();

        objGrid = ObjGrid::Create(100, 5.f);
        objAxis = ObjAxis::Create(5.f);

        objPlane = ObjPlane::Create(&arcModels, &arcTextures, namePlane);
        objDwarf = ObjDwarf::Create(&arcModels, &arcTextures, nameDwarf);
    }

    { // Object init
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
        RELEASE(objDwarf);
    }
}

void StageTest::Update(float delta){
    static const float fHeroSpeed = 5.f;
    static Vector3 vHeroPos = { 0, 0, 0 };

    { // Control update
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

        if (Core::Input::KeyDown(Core::Input::DK_W))vHeroPos.z += delta * fHeroSpeed;
        else if (Core::Input::KeyDown(Core::Input::DK_S))vHeroPos.z -= delta * fHeroSpeed;
        if (Core::Input::KeyDown(Core::Input::DK_D))vHeroPos.x += delta * fHeroSpeed;
        else if (Core::Input::KeyDown(Core::Input::DK_A))vHeroPos.x -= delta * fHeroSpeed;

        if (Core::Input::KeyPressed(Core::Input::DK_F2))bGridSwitch ^= true;
        if (Core::Input::KeyPressed(Core::Input::DK_F3))bAxisSwitch ^= true;
    }

    { // Camera update
        Vector3 vCamLookAt(vHeroPos.x, 5.f, vHeroPos.z);

        objCamera->Update(&vCamLookAt);
    }

    { // Environment update
        static float fLightRotation = PIf * 2.f;
        Vector3 vLight = Vector3(Sin(fLightRotation), -1, Cos(fLightRotation));

        fLightRotation -= delta * 1.f;
        if (fLightRotation < 0.f)fLightRotation = PIf * 2.f;

        vLight.normalize();
        //envLight.Direction = { vLight.x, vLight.y, vLight.z };

        {
            shadLight->SetVector("l_d", (const Vector4*)&envLight.Diffuse);
            shadLight->SetVector("l_a", (const Vector4*)&envLight.Ambient);
            shadLight->SetVector("l_s", (const Vector4*)&envLight.Specular);
            shadLight->SetVector("l_r", (const Vector4*)&vLight);
        }
    }

    { // Guide object update
        if (bAxisSwitch)objAxis->Update(&Vector3(0.f, 0.f, 0.f));
    }

    { // Object update
        static const Vector3 vPlaneCenter(0.f, 5.f, 0.f);
        static const float fPlaneTurnRad = 30.f;
        static const float fPlaneSpeed = 1.f;

        objPlane->TurnAround(&vPlaneCenter, fPlaneTurnRad, fPlaneSpeed);
        objPlane->Update(delta);

        objDwarf->SetPosition(&vHeroPos);
        objDwarf->Update(delta);
    }
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
        objPlane->Draw(shadLight);
        objDwarf->Draw(shadLight);
    }

    if (bAxisSwitch)objAxis->Draw();

    Graphic::GetDevice()->EndScene();
}