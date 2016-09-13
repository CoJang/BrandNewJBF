#include"pch.h"
#include"JBF/JBFramework.h"
#include"ArchiveTable.h"

using namespace JBF;

#include"Stage/StageTest.h"
extern StageTest stgTest;

static const Global::Math::Point<USHORT> clientSize = { 720, 480 };
static void GameBegin(){
    arcSprites.OpenFile(_T("./Content/EU_Sprites.jba"));
    arcShaders.OpenFile(_T("./Content/EU_Shaders.jba"));

    Manager::Stage::ChangeStage(&stgTest);
}
static void GameEnd(){
    arcSprites.CloseFile();
    arcShaders.CloseFile();
}
CREATE_MAIN(_T("EU"), GameBegin, GameEnd, nullptr, &clientSize);