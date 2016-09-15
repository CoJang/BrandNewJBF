#include"pch.h"

#include"InnerUtility.h"
#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
#ifdef USE_INTRINSIC
            __m128 MATH_CALL xmm_abs(__m128 v){
                return _mm_andnot_ps(_mm_set1_ps(-0.f), v);
            }

            __m128 MATH_CALL xmm_sin(__m128 v){
                static const float B = 4.f / PIf;
                static const float C = -4.f / (PIf * PIf);
                static const float P = 0.225f;

                __m128 m_pi = _mm_set1_ps(PIf);
                __m128 m_mpi = _mm_set1_ps(-PIf);
                __m128 m_2pi = _mm_set1_ps(PIf * 2);
                __m128 m_B = _mm_set1_ps(B);
                __m128 m_C = _mm_set1_ps(C);
                __m128 m_P = _mm_set1_ps(P);
                __m128 m1 = _mm_cmpnlt_ps(v, m_pi);

                m1 = _mm_and_ps(m1, m_2pi);
                v = _mm_sub_ps(v, m1);
                m1 = _mm_cmpngt_ps(v, m_mpi);
                m1 = _mm_and_ps(m1, m_2pi);
                v = _mm_add_ps(v, m1);

                __m128 m_abs = xmm_abs(v);

                m1 = _mm_mul_ps(m_abs, m_C);
                m1 = _mm_add_ps(m1, m_B);

                __m128 m_y = _mm_mul_ps(m1, v);

                m_abs = xmm_abs(m_y);
                m1 = _mm_mul_ps(m_abs, m_y);
                m1 = _mm_sub_ps(m1, m_y);
                m1 = _mm_mul_ps(m1, m_P);
                m_y = _mm_add_ps(m1, m_y);

                return m_y;
            }
            __m128 MATH_CALL xmm_cos(__m128 v){
                v = _mm_add_ps(v, _mm_set1_ps(PIf / 2.f));
                return xmm_sin(v);
            }

            __m128 MATH_CALL xmm_tan(__m128 v){
                __m128 b = xmm_cos(v);

                v = xmm_sin(v);
                v = _mm_div_ps(v, b);

                return v;
            }
#endif
        };
    };
};