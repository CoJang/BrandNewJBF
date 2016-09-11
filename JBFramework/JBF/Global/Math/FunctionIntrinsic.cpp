#include"pch.h"

#ifdef USE_INTRINSIC
#include<intrin.h>

#include"InnerUtility.h"
#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
            INLINE float MATH_CALL Sin(float v){ return xmm_sin(_mm_set1_ps(v)).m128_f32[0]; }
            INLINE float MATH_CALL Cos(float v){ return xmm_cos(_mm_set1_ps(v)).m128_f32[0]; }

            Vector3* MATH_CALL Vec3TransformCoord(Vector3* pOut, const Vector3* pV, const Matrix* pM){
                __m128 i = _mm_set_ps(1.f, pV->z, pV->y, pV->x);
                Matrix m = pM->transposed();

                pOut->table_xmm = _mm_dp_ps(i, m.table_xmm[0], 0xf1);
                pOut->table_xmm = _mm_or_ps(pOut->table_xmm, _mm_dp_ps(i, m.table_xmm[1], 0xf2));
                pOut->table_xmm = _mm_or_ps(pOut->table_xmm, _mm_dp_ps(i, m.table_xmm[2], 0xf4));

                pOut->table_xmm = _mm_or_ps(pOut->table_xmm, _mm_dp_ps(i, m.table_xmm[3], 0xf8));
                pOut->table_xmm = _mm_div_ps(pOut->table_xmm, _mm_set1_ps(pOut->table_xmm.m128_f32[3]));

                return pOut;
            }
            Vector4* MATH_CALL Vec4TransformCoord(Vector4* pOut, const Vector4* pV, const Matrix* pM){
                __m128 i = pV->table_xmm;
                Matrix m = pM->transposed();

                pOut->table_xmm = _mm_dp_ps(i, m.table_xmm[0], 0xf1);
                pOut->table_xmm = _mm_or_ps(pOut->table_xmm, _mm_dp_ps(i, m.table_xmm[1], 0xf2));
                pOut->table_xmm = _mm_or_ps(pOut->table_xmm, _mm_dp_ps(i, m.table_xmm[2], 0xf4));
                pOut->table_xmm = _mm_or_ps(pOut->table_xmm, _mm_dp_ps(i, m.table_xmm[3], 0xf8));

                return pOut;
            }
        };
    };
};
#endif