#include"pch.h"
#include"JBF/Core/Core.h"

namespace JBF{
    namespace Core{
        namespace SystemTimer{
            // Inner variable(s) definition
            ///////////////////////////////////////////
            static decltype(GetCurrentTick()) ins_tickTable[2];
            static decltype(&(*ins_tickTable)) ins_oldTick, ins_curTick;

            static float ins_timeDelta;
            ///////////////////////////////////////////

            INLINE std::chrono::steady_clock::time_point GetCurrentTick(){ return std::chrono::steady_clock::now(); }

            void Init(){
                ins_oldTick = &ins_tickTable[0];
                ins_curTick = &ins_tickTable[1];

                *ins_oldTick = GetCurrentTick();
            }
            void Update(){
                *ins_curTick = GetCurrentTick();
                ins_timeDelta = std::chrono::duration<decltype(ins_timeDelta), std::milli>((*ins_curTick) - (*ins_oldTick)).count() * 0.001f;
                std::swap(ins_curTick, ins_oldTick);
            }

            INLINE float GetTimeDelta(){ return ins_timeDelta; }
        };
    };
};