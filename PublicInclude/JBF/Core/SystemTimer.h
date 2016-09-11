#pragma once

#include<chrono>
#include"JBF/Definitions.h"

namespace JBF{
    namespace Core{
        namespace SystemTimer{
            extern INLINE std::chrono::steady_clock::time_point GetCurrentTick();

            extern void Init();
            extern void Update();

            extern INLINE float GetTimeDelta();
        };
    };
};