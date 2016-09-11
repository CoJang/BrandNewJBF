#include"pch.h"
#include"JBF/Base/Base.h"

namespace JBF{
    namespace Base{
        Stage::Stage(){}
        Stage::~Stage(){}

        void Stage::Init(){}
        void Stage::Cleanup(){}

        void Stage::Update(float delta){}
        void Stage::Draw(){}

        void Stage::Pause(){}
        void Stage::Resume(){}
    };
};