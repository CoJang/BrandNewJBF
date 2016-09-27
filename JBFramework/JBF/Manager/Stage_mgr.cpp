#include"pch.h"
#include<forward_list>

#include"JBF/Manager/Manager.h"

namespace JBF{
    namespace Manager{
        namespace Stage{
            static std::forward_list<Base::Stage*> ins_table;

            void Stage::Init(){}
            void Stage::Release(){
                while (!ins_table.empty()){
                    (*ins_table.begin())->Cleanup();
                    ins_table.pop_front();
                }
            }

            INLINE void Update(float delta){
                if (!ins_table.empty())(*ins_table.begin())->Update(delta);
            }
            INLINE void Draw(){
                if (!ins_table.empty())(*ins_table.begin())->Draw();
            }

            void ChangeStage(Base::Stage* stage){
                if (!ins_table.empty()){
                    (*ins_table.begin())->Cleanup();
                    ins_table.pop_front();
                }

                stage->Init();
                ins_table.emplace_front(stage);
            }
            void PushStage(Base::Stage* stage){
                if (!ins_table.empty())(*ins_table.begin())->Pause();

                stage->Init();
                ins_table.emplace_front(stage);
            }
            void PopStage(){
                if (!ins_table.empty())(*ins_table.begin())->Cleanup();
                if (!ins_table.empty())(*ins_table.begin())->Resume();
            }

            void Validate(){
                for (auto i : ins_table)i->Validate();
            }
            void Invalidate(){
                for (auto i : ins_table)i->Invalidate();
            }
        };
    };
};