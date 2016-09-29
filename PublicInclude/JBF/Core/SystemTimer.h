#pragma once

#include<chrono>
#include"JBF/Definitions.h"

namespace JBF{
    namespace Core{
        namespace SystemTimer{
            extern void Init();
            extern void Update();

            extern INLINE float GetTimeDelta();
        };
    };
};