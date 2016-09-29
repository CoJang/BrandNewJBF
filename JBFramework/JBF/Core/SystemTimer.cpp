#include"pch.h"
#include"JBF/Core/Core.h"

namespace JBF{
    namespace Core{
        namespace SystemTimer{
            // Inner variable(s) definition
            ///////////////////////////////////////////
            static double ins_frequency;

            static LARGE_INTEGER ins_tickTable[2];
            static decltype(&(*ins_tickTable)) ins_oldTick, ins_curTick;

            static double ins_timeDelta;
            ///////////////////////////////////////////

            void Init(){
                LARGE_INTEGER tmp;

                QueryPerformanceFrequency(&tmp);
                ins_frequency = 1. / tmp.QuadPart;

                ins_oldTick = &ins_tickTable[0];
                ins_curTick = &ins_tickTable[1];

                QueryPerformanceCounter(ins_oldTick);
            }
            void Update(){
                QueryPerformanceCounter(ins_curTick);

                ins_timeDelta = (ins_curTick->QuadPart - ins_oldTick->QuadPart);
                ins_timeDelta *= ins_frequency;

                std::swap(ins_curTick, ins_oldTick);
            }

            INLINE float GetTimeDelta(){ return (float)ins_timeDelta; }
        };
    };
};