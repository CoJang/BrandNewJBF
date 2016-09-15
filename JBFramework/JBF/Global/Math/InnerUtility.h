#pragma once

#ifdef USE_INTRINSIC
#include<intrin.h>
#endif

#include"JBF/Definitions.h"

namespace JBF{
    namespace Global{
        namespace Math{
#ifdef USE_INTRINSIC
            extern __m128 MATH_CALL xmm_abs(__m128 v);

            extern __m128 MATH_CALL xmm_sin(__m128 v);
            extern __m128 MATH_CALL xmm_cos(__m128 v);
            extern __m128 MATH_CALL xmm_tan(__m128 v);
#endif
        };
    };
};