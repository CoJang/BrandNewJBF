#include"pch.h"
#include"JBF/Core/Core.h"

namespace JBF{
    namespace Core{
        namespace SystemTimer{
            using namespace std;
            using namespace std::chrono;

            // Inner variable(s) definition
            ///////////////////////////////////////////
            static steady_clock::time_point ins_tickTable[2];
            static steady_clock::time_point *ins_oldTick, *ins_curTick;

            static double ins_timeDelta;
            ///////////////////////////////////////////

            void Init(){
                ins_oldTick = &ins_tickTable[0];
                ins_curTick = &ins_tickTable[1];

                *ins_oldTick = steady_clock::now();
            }

            void Update(){
                *ins_curTick = steady_clock::now();
                ins_timeDelta = duration<decltype(ins_timeDelta), micro>((*ins_curTick) - (*ins_oldTick)).count() * 0.000001;
                std::swap(ins_curTick, ins_oldTick);
            }

            INLINE float GetTimeDelta(){ return ins_timeDelta; }
        };
    };
};