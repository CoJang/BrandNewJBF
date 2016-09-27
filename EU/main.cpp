#include"pch.h"
#include"JBF/JBFramework.h"

#include"ArchiveTable.h"
#include"ShaderTable.h"

#include"Stage/Stage.h"

using namespace JBF;

static const Global::Math::Point<WORD> clientSize = { 720, 480 };
static void GameBegin(){
    ArchiveLoad();
    ShaderLoad();

    Manager::Stage::ChangeStage(&stgTest);
}
static void GameEnd(){
    ShaderCleanup();
    ArchiveCleanup();
}
CREATE_MAIN(_T("EU"), GameBegin, GameEnd, nullptr, &clientSize);