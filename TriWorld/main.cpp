#include"pch.h"
#include"JBF/JBFramework.h"
#include"Public/Public.h"

using namespace JBF;

#include"Stage/StageTest.h"
extern StageTest stgTest;

static const Global::Math::Point<WORD> clientSize = { 320, 240 };
static void GameBegin(){
    ArchiveLoad();

    Manager::Stage::ChangeStage(&stgTest);
}
static void GameEnd(){
    ArchiveCleanup();
}
CREATE_MAIN(_T("TriWorld"), GameBegin, GameEnd, nullptr, &clientSize);