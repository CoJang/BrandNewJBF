#include"pch.h"
#include"JBF/JBFramework.h"

#include"Public/Public.h"
#include"Stage/Stage.h"

using namespace JBF;

static const Global::Math::Point<WORD> clientSize = { 720, 480 };
static void GameBegin(){
    ArchiveLoad();
    ShaderLoad();

    GlobalObjectInit();

    Manager::Stage::ChangeStage(&stgTitle);
}
static void GameEnd(){
    GlobalObjectCleanup();

    ShaderCleanup();
    ArchiveCleanup();
}
CREATE_MAIN(_T("EU"), GameBegin, GameEnd, nullptr, &clientSize);