#pragma once

#include"JBF/Definitions.h"

namespace JBF{
    namespace Debug{
        namespace Dump{
            extern void Init();
            extern void Release();

            extern INLINE void MakeDump();
        };
    };
};