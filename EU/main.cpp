#include"pch.h"
#include"JBF/JBFramework.h"
#include"ArchiveTable.h"

using namespace JBF;

#include"Stage/StageTest.h"
extern StageTest stgTest;

static const Global::Math::Point<USHORT> clientSize = { 1366, 768 };
static void GameBegin(){
    arcModels.OpenFile(_T(".\\Content\\Models.jba"));
    arcTextures.OpenFile(_T(".\\Content\\Textures.jba"));

    Manager::Stage::ChangeStage(&stgTest);
}
static void GameEnd(){
    arcModels.CloseFile();
    arcTextures.CloseFile();
}
CREATE_MAIN(_T("EU"), GameBegin, GameEnd, nullptr, &clientSize);