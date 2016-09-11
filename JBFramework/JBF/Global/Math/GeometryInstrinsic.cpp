#include"pch.h"

#ifdef USE_INTRINSIC
#include<cfloat>
#include<cmath>
#include<intrin.h>

#include"JBF/Global/Math/Math.h"

union __declspec(align(32)) __m256cmp{
    __m256 r;
    __declspec(align(32)) unsigned long long c[4];
};

// AVX에서 사용되는 __m256(YMM#)레지스터의 비교 명령은 
// _mm256_cmp_ps(...)함수로 제공되며 해당 조건은 3번째 인자로 구성 가능하다.
// 조건 설정 중, 플래그의 구분을 살펴보면 ..._OS, ..._OQ, ..._UQ 등으로 나타나는데 각각의 설명은 다음과 같다.
// O(ordered) <-> U(unordered)
// O: 피연산자에 대한 NaN 여부를 무시한다.
// U: 피연산자에 대한 NaN 여부를 체크한다.
// S(signaling) <-> Q(nonsignaling)
// S: NaN의 존재에 의한 예외를 발생시킨다.
// Q: NaN의 존재에 의한 예외를 무시한다.

namespace JBF{
    namespace Global{
        namespace Math{
            Vector3::Vector3(float fill){
                table_xmm = _mm_set1_ps(fill);
            }
            Vector3::Vector3(float _x, float _y, float _z){
                table_xmm = _mm_set_ps(1.f, _z, _y, _x);
            }
            Vector3::Vector3(const Vector3& rhs){
                table_xmm = rhs.table_xmm;
            }

            Vector3& Vector3::operator=(const Vector3& rhs){
                table_xmm = rhs.table_xmm;
                return *this;
            }

            bool MATH_CALL Vector3::operator!()const{
                __m128 a, b;

                a = table_xmm;
                b = _mm_setzero_ps();

                a = _mm_cmpeq_ps(a, b);
                return a.m128_u64[0] && a.m128_u32[2];
            }
            bool MATH_CALL Vector3::operator==(const Vector3& rhs)const{
                __m128 a, b;

                a = _mm_sub_ps(table_xmm, rhs.table_xmm);

                // -0.f = 1 << 31
                b = _mm_set1_ps(-0.f);

                b = _mm_andnot_ps(b, a);
                a = _mm_set1_ps(FLT_EPSILON);

                b = _mm_cmple_ps(b, a);
                return b.m128_u64[0] && b.m128_u32[2];
            }

            Vector3& MATH_CALL Vector3::operator+=(const Vector3& rhs){
                table_xmm = _mm_add_ps(table_xmm, rhs.table_xmm);
                return *this;
            }
            Vector3& MATH_CALL Vector3::operator-=(const Vector3& rhs){
                table_xmm = _mm_sub_ps(table_xmm, rhs.table_xmm);
                return *this;
            }
            Vector3& MATH_CALL Vector3::operator*=(const Vector3& rhs){
                table_xmm = _mm_mul_ps(table_xmm, rhs.table_xmm);
                return *this;
            }
            Vector3& MATH_CALL Vector3::operator/=(const Vector3& rhs){
                table_xmm = _mm_div_ps(table_xmm, rhs.table_xmm);
                return *this;
            }

            Vector3& MATH_CALL Vector3::operator*=(float rhs){
                table_xmm = _mm_mul_ps(table_xmm, _mm_set1_ps(rhs));
                return *this;
            }
            Vector3& MATH_CALL Vector3::operator/=(float rhs){
                table_xmm = _mm_div_ps(table_xmm, _mm_set1_ps(rhs));
                return *this;
            }

            Vector3& MATH_CALL Vector3::normalize(){
                __m128 b;

                if (table_xmm.m128_u64[0] || table_xmm.m128_u32[2]){
                    // mask0 = 1
                    // mask1 = 1
                    // mask2 = 1
                    // mask3 = 1
                    // 출력 레지스터의 상위 4개의 4bytes에 dot-production 결과를 기록

                    // mask4 = 1
                    // mask5 = 1
                    // mask6 = 1
                    // mask7 = 0
                    // 입력 레지스터의 상위 3개의 4bytes에 대해서만 연산을 진행
                    b = _mm_dp_ps(table_xmm, table_xmm, 0x7f);

                    b = _mm_sqrt_ps(b);
                    table_xmm = _mm_div_ps(table_xmm, b);
                }
                else table_xmm = _mm_setzero_ps();

                return *this;
            }
            Vector3& MATH_CALL Vector3::cross(const Vector3& rhs){
                __m128 b, c;

                b = rhs.table_xmm;

                // w, z, y, x -> w, x, z, y
                table_xmm = _mm_shuffle_ps(table_xmm, table_xmm, _MM_SHUFFLE(3, 0, 2, 1));

                // w, z, y, x -> w, y, x, z
                b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));

                c = _mm_mul_ps(table_xmm, b);

                // w, x, z, y -> w, y, x, z
                table_xmm = _mm_shuffle_ps(table_xmm, table_xmm, _MM_SHUFFLE(3, 0, 2, 1));

                // w, y, x, z -> w, x, z, y
                b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));

                table_xmm = _mm_mul_ps(table_xmm, b);
                table_xmm = _mm_sub_ps(c, table_xmm);

                return *this;
            }

            float MATH_CALL Vector3::dot(const Vector3& rhs)const{
                __m128 a;

                // mask0 = 1
                // mask1 = 0
                // mask2 = 0
                // mask3 = 0
                // 출력 레지스터의 상위 1개의 4bytes에 dot-production 결과를 기록

                // mask4 = 1
                // mask5 = 1
                // mask6 = 1
                // mask7 = 0
                // 입력 레지스터의 상위 3개의 4bytes에 대해서만 연산을 진행
                a = _mm_dp_ps(table_xmm, rhs.table_xmm, 0x71);

                return *a.m128_f32;
            }
            float MATH_CALL Vector3::length()const{
                __m128 a;

                a = _mm_cmpeq_ps(table_xmm, _mm_setzero_ps());
                if ((!a.m128_u64[0]) || (!a.m128_u32[3])){
                    a = _mm_dp_ps(table_xmm, table_xmm, 0x71);
                }
                else a = _mm_setzero_ps();

                return std::sqrt(*a.m128_f32);
            }


            Vector4::Vector4(float fill){
                table_xmm = _mm_set1_ps(fill);
            }
            Vector4::Vector4(float _x, float _y, float _z, float _w){
                table_xmm = _mm_set_ps(_w, _z, _y, _x);
            }
            Vector4::Vector4(const Vector4& rhs){
                table_xmm = rhs.table_xmm;
            }

            Vector4& Vector4::operator=(const Vector4& rhs){
                table_xmm = rhs.table_xmm;
                return *this;
            }

            bool MATH_CALL Vector4::operator!()const{
                __m128 a;

                a = _mm_cmpeq_ps(table_xmm, _mm_setzero_ps());
                return a.m128_u64[0] && a.m128_u64[1];
            }
            bool MATH_CALL Vector4::operator==(const Vector4& rhs)const{
                __m128 a, b;

                a = _mm_sub_ps(table_xmm, rhs.table_xmm);

                // -0.f = 1 << 31
                b = _mm_set1_ps(-0.f);

                b = _mm_andnot_ps(b, a);
                a = _mm_set1_ps(FLT_EPSILON);

                b = _mm_cmple_ps(b, a);
                return a.m128_u64[0] && a.m128_u64[1];
            }

            Vector4& MATH_CALL Vector4::operator+=(const Vector4& rhs){
                table_xmm = _mm_add_ps(table_xmm, rhs.table_xmm);
                return *this;
            }
            Vector4& MATH_CALL Vector4::operator-=(const Vector4& rhs){
                table_xmm = _mm_sub_ps(table_xmm, rhs.table_xmm);
                return *this;
            }
            Vector4& MATH_CALL Vector4::operator*=(const Vector4& rhs){
                table_xmm = _mm_mul_ps(table_xmm, rhs.table_xmm);
                return *this;
            }
            Vector4& MATH_CALL Vector4::operator/=(const Vector4& rhs){
                table_xmm = _mm_div_ps(table_xmm, rhs.table_xmm);
                return *this;
            }

            Vector4& MATH_CALL Vector4::operator*=(float rhs){
                table_xmm = _mm_mul_ps(table_xmm, _mm_set1_ps(rhs));
                return *this;
            }
            Vector4& MATH_CALL Vector4::operator/=(float rhs){
                table_xmm = _mm_div_ps(table_xmm, _mm_set1_ps(rhs));
                return *this;
            }


            Matrix::Matrix(float fill){
                table_ymm[0] = table_ymm[1] = _mm256_set1_ps(fill);
            }
            Matrix::Matrix(
                float __11, float __12, float __13, float __14,
                float __21, float __22, float __23, float __24,
                float __31, float __32, float __33, float __34,
                float __41, float __42, float __43, float __44
            ){
                table_ymm[0] = _mm256_set_ps(
                    __24, __23, __22, __21,
                    __14, __13, __12, __11
                );
                table_ymm[1] = _mm256_set_ps(
                    __44, __43, __42, __41,
                    __34, __33, __32, __31
                );
            }
            Matrix::Matrix(const Matrix& rhs){
                table_ymm[0] = rhs.table_ymm[0];
                table_ymm[1] = rhs.table_ymm[1];
            }

            Matrix& Matrix::operator=(const Matrix& rhs){
                table_ymm[0] = rhs.table_ymm[0];
                table_ymm[1] = rhs.table_ymm[1];
                return *this;
            }

            bool MATH_CALL Matrix::operator!()const{
                __m256cmp a, b;

                b.r = _mm256_setzero_ps();

                a.r = _mm256_cmp_ps(table_ymm[0], b.r, _CMP_EQ_OQ);
                if ((!a.c[0]) || (!a.c[1]) || (!a.c[2]) || (!a.c[3]))return false;

                a.r = _mm256_cmp_ps(table_ymm[1], b.r, _CMP_EQ_OQ);
                return (a.c[0] && a.c[1] && a.c[2] && a.c[3]);
            }
            bool MATH_CALL Matrix::operator==(const Matrix& rhs)const{
                __m256cmp a, b;


                a.r = _mm256_sub_ps(table_ymm[0], rhs.table_ymm[0]);

                // -0.f = 1 << 32
                b.r = _mm256_set1_ps(-0.f);

                b.r = _mm256_andnot_ps(b.r, a.r);
                a.r = _mm256_set1_ps(FLT_EPSILON);

                b.r = _mm256_cmp_ps(b.r, a.r, _CMP_LE_OQ);
                if ((!b.c[0]) || (!b.c[1]) || (!b.c[2]) || (!b.c[3]))return false;


                a.r = _mm256_sub_ps(table_ymm[1], rhs.table_ymm[1]);

                // -0.f = 1 << 32
                b.r = _mm256_set1_ps(-0.f);

                b.r = _mm256_andnot_ps(b.r, a.r);
                a.r = _mm256_set1_ps(FLT_EPSILON);

                b.r = _mm256_cmp_ps(b.r, a.r, _CMP_LE_OQ);
                return (b.c[0] && b.c[1] && b.c[2] && b.c[3]);
            }

            Matrix& MATH_CALL Matrix::operator+=(const Matrix& rhs){
                table_ymm[0] = _mm256_add_ps(table_ymm[0], rhs.table_ymm[0]);
                table_ymm[1] = _mm256_add_ps(table_ymm[1], rhs.table_ymm[1]);
                return *this;
            }
            Matrix& MATH_CALL Matrix::operator-=(const Matrix& rhs){
                table_ymm[0] = _mm256_sub_ps(table_ymm[0], rhs.table_ymm[0]);
                table_ymm[1] = _mm256_sub_ps(table_ymm[1], rhs.table_ymm[1]);
                return *this;
            }
            Matrix& MATH_CALL Matrix::operator*=(const Matrix& rhs){
                __m128 t[4];
                __m256 a, b, c, d;
                __m256 _rhs[2];


                t[0] = rhs.table_xmm[0];
                t[1] = rhs.table_xmm[1];
                t[2] = rhs.table_xmm[2];
                t[3] = rhs.table_xmm[3];

                _MM_TRANSPOSE4_PS(t[0], t[1], t[2], t[3]);
                _rhs[0] = _mm256_set_m128(t[1], t[0]);
                _rhs[1] = _mm256_set_m128(t[3], t[2]);


                {
                    a = _mm256_set_m128(table_xmm[0], table_xmm[0]);

                    c = _mm256_dp_ps(a, _rhs[0], 0xf1);
                    d = _mm256_dp_ps(a, _rhs[1], 0xf1);
                }
                {
                    a = _mm256_set_m128(table_xmm[1], table_xmm[1]);

                    b = _mm256_dp_ps(a, _rhs[0], 0xf2);
                    c = _mm256_or_ps(c, b);

                    b = _mm256_dp_ps(a, _rhs[1], 0xf2);
                    d = _mm256_or_ps(d, b);
                }
                {
                    a = _mm256_set_m128(table_xmm[2], table_xmm[2]);

                    b = _mm256_dp_ps(a, _rhs[0], 0xf4);
                    c = _mm256_or_ps(c, b);

                    b = _mm256_dp_ps(a, _rhs[1], 0xf4);
                    d = _mm256_or_ps(d, b);
                }
                {
                    a = _mm256_set_m128(table_xmm[3], table_xmm[3]);

                    b = _mm256_dp_ps(a, _rhs[0], 0xf8);
                    c = _mm256_or_ps(c, b);

                    b = _mm256_dp_ps(a, _rhs[1], 0xf8);
                    d = _mm256_or_ps(d, b);
                }


                table_xmm[0] = _mm256_extractf128_ps(c, 0);
                table_xmm[1] = _mm256_extractf128_ps(c, 1);
                table_xmm[2] = _mm256_extractf128_ps(d, 0);
                table_xmm[3] = _mm256_extractf128_ps(d, 1);

                _MM_TRANSPOSE4_PS(table_xmm[0], table_xmm[1], table_xmm[2], table_xmm[3]);

                return *this;
            }

            Matrix& MATH_CALL Matrix::operator*=(float rhs){
                __m256 a;

                a = _mm256_set1_ps(rhs);

                table_ymm[0] = _mm256_mul_ps(table_ymm[0], a);
                table_ymm[1] = _mm256_mul_ps(table_ymm[1], a);

                return *this;
            }
            Matrix& MATH_CALL Matrix::operator/=(float rhs){
                __m256 a;

                a = _mm256_set1_ps(rhs);

                table_ymm[0] = _mm256_div_ps(table_ymm[0], a);
                table_ymm[1] = _mm256_div_ps(table_ymm[1], a);

                return *this;
            }

            Matrix& MATH_CALL Matrix::identify(){
                table_ymm[0] = _mm256_set_ps(
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 0.f, 1.f
                );
                table_ymm[1] = _mm256_set_ps(
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f
                );
                return *this;
            }
            Matrix& MATH_CALL Matrix::transpose(){
                _MM_TRANSPOSE4_PS(table_xmm[0], table_xmm[1], table_xmm[2], table_xmm[3]);
                return *this;
            }
            Matrix& MATH_CALL Matrix::inverse(){
                __m128 dst1[4], dst2[4];
                __m128 a, b, c;
                __m128 tmp[3];
                __m256 s;


                _MM_TRANSPOSE4_PS(table_xmm[0], table_xmm[1], table_xmm[2], table_xmm[3]);


                {
                    a = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(3, 1, 3, 2));
                    b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(1, 3, 2, 3));
                    tmp[0] = _mm_mul_ps(a, b);

                    a = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(3, 0, 2, 1));
                    b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 1, 2));
                    tmp[1] = _mm_mul_ps(a, b);

                    a = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(1, 0, 2, 0));
                    b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 1, 0, 2));
                    tmp[2] = _mm_mul_ps(a, b);
                }


                // 0
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 0, 3, 0));
                b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 2, 1));
                dst1[0] = _mm_dp_ps(a, b, 0x71);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(2, 1, 2, 1));
                //b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 2, 1));
                dst2[0] = _mm_dp_ps(a, b, 0x71);
                //

                // 1
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(0, 1, 3, 0));
                b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 2, 0));
                c = _mm_dp_ps(a, b, 0x72);
                dst1[0] = _mm_or_ps(dst1[0], c);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 0));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(0, 0, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 2, 0));
                c = _mm_dp_ps(a, b, 0x72);
                dst2[0] = _mm_or_ps(dst2[0], c);
                //

                // 2
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 2));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(3, 2, 2, 0));
                b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst1[0] = _mm_or_ps(dst1[0], c);

                c = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 2, 0, 3));
                a = _mm_shuffle_ps(c, a, _MM_SHUFFLE(3, 3, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst2[0] = _mm_or_ps(dst2[0], c);
                //

                // 3
                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(3, 0, 0, 1));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
                b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 2, 1, 0));
                c = _mm_dp_ps(a, b, 0x78);
                dst1[0] = _mm_or_ps(dst1[0], c);

                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(2, 1, 1, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 2, 1, 0));
                c = _mm_dp_ps(a, b, 0x78);
                dst2[0] = _mm_or_ps(dst2[0], c);
                //

                // 4
                a = _mm_shuffle_ps(tmp[0], a, _MM_SHUFFLE(0, 3, 2, 1));
                b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 3, 2, 1));
                dst1[1] = _mm_dp_ps(a, b, 0x71);

                a = _mm_shuffle_ps(tmp[0], a, _MM_SHUFFLE(0, 3, 3, 0));
                //b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 3, 2, 1));
                dst2[1] = _mm_dp_ps(a, b, 0x71);
                //

                // 5
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 0));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(1, 0, 2, 0));
                b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 3, 2, 0));
                c = _mm_dp_ps(a, b, 0x72);
                dst1[1] = _mm_or_ps(dst1[1], c);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 2, 0, 1));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(3, 1, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 3, 2, 0));
                c = _mm_dp_ps(a, b, 0x72);
                dst2[1] = _mm_or_ps(dst2[1], c);
                //

                // 6
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 2, 0, 3));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(2, 3, 2, 0));
                b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 3, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst1[1] = _mm_or_ps(dst1[1], c);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 2));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(1, 2, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 3, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst2[1] = _mm_or_ps(dst2[1], c);
                //

                // 7
                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(2, 1, 1, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
                b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 2, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst1[1] = _mm_or_ps(dst1[1], c);

                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(3, 0, 0, 1));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(0, 2, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst2[1] = _mm_or_ps(dst2[1], c);
                //


                {
                    a = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(3, 1, 3, 2));
                    b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(1, 3, 2, 3));
                    tmp[0] = _mm_mul_ps(a, b);

                    a = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(3, 0, 2, 1));
                    b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 3, 1, 2));
                    tmp[1] = _mm_mul_ps(a, b);

                    a = _mm_shuffle_ps(table_xmm[0], table_xmm[0], _MM_SHUFFLE(1, 0, 2, 0));
                    b = _mm_shuffle_ps(table_xmm[1], table_xmm[1], _MM_SHUFFLE(0, 1, 0, 2));
                    tmp[2] = _mm_mul_ps(a, b);
                }


                // 8
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 0, 3, 0));
                b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 2, 1));
                dst1[2] = _mm_dp_ps(a, b, 0x71);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(2, 1, 2, 1));
                //b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 2, 1));
                dst2[2] = _mm_dp_ps(a, b, 0x71);
                //

                // 9
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(0, 1, 3, 0));
                b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 2, 0));
                c = _mm_dp_ps(a, b, 0x72);
                dst1[2] = _mm_or_ps(dst1[2], c);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 0));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(0, 0, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 2, 0));
                c = _mm_dp_ps(a, b, 0x72);
                dst2[2] = _mm_or_ps(dst2[2], c);
                //

                // 10
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 2));
                a = _mm_shuffle_ps(a, tmp[2], _MM_SHUFFLE(3, 2, 2, 0));
                b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst1[2] = _mm_or_ps(dst1[2], c);

                c = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 2, 0, 3));
                a = _mm_shuffle_ps(c, a, _MM_SHUFFLE(3, 3, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 3, 1, 0));
                c = _mm_dp_ps(a, b, 0x74);
                dst2[2] = _mm_or_ps(dst2[2], c);
                //

                // 11
                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(3, 0, 0, 1));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
                b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 2, 1, 0));
                c = _mm_dp_ps(a, b, 0x78);
                dst1[2] = _mm_or_ps(dst1[2], c);

                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(2, 1, 1, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
                //b = _mm_shuffle_ps(table_xmm[3], table_xmm[3], _MM_SHUFFLE(0, 2, 1, 0));
                c = _mm_dp_ps(a, b, 0x78);
                dst2[2] = _mm_or_ps(dst2[2], c);
                //

                // 12
                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 1, 1, 2));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 2, 0));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 1, 3, 2));
                dst1[3] = _mm_dp_ps(a, b, 0x71);

                a = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 0, 3, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 0, 2));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 2, 1, 3));
                dst2[3] = _mm_dp_ps(a, b, 0x71);
                //

                // 13
                c = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 0));
                a = _mm_shuffle_ps(c, tmp[2], _MM_SHUFFLE(1, 0, 2, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 2, 0, 3));
                c = _mm_dp_ps(a, b, 0x72);
                dst1[3] = _mm_or_ps(dst1[3], c);

                c = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 2, 0, 1));
                a = _mm_shuffle_ps(c, tmp[2], _MM_SHUFFLE(3, 1, 2, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 0, 3, 2));
                c = _mm_dp_ps(a, b, 0x72);
                dst2[3] = _mm_or_ps(dst2[3], c);
                //

                // 14
                c = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 2, 0, 3));
                a = _mm_shuffle_ps(c, tmp[2], _MM_SHUFFLE(2, 3, 2, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 0, 3, 1));
                c = _mm_dp_ps(a, b, 0x74);
                dst1[3] = _mm_or_ps(dst1[3], c);

                c = _mm_shuffle_ps(tmp[0], tmp[1], _MM_SHUFFLE(0, 3, 0, 2));
                a = _mm_shuffle_ps(c, tmp[2], _MM_SHUFFLE(1, 2, 2, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 1, 0, 3));
                c = _mm_dp_ps(a, b, 0x74);
                dst2[3] = _mm_or_ps(dst2[3], c);
                //

                // 15
                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(2, 1, 1, 0));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 2, 0, 3));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 1, 0, 2));
                c = _mm_dp_ps(a, b, 0x78);
                dst1[3] = _mm_or_ps(dst1[3], c);

                a = _mm_shuffle_ps(tmp[1], tmp[2], _MM_SHUFFLE(3, 0, 0, 1));
                a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 0, 3, 2));
                b = _mm_shuffle_ps(table_xmm[2], table_xmm[2], _MM_SHUFFLE(0, 0, 2, 1));
                c = _mm_dp_ps(a, b, 0x78);
                dst2[3] = _mm_or_ps(dst2[3], c);
                //


                dst1[0] = _mm_sub_ps(dst1[0], dst2[0]);
                dst1[1] = _mm_sub_ps(dst1[1], dst2[1]);
                dst1[2] = _mm_sub_ps(dst1[2], dst2[2]);
                dst1[3] = _mm_sub_ps(dst1[3], dst2[3]);


                a = _mm_dp_ps(table_xmm[0], dst1[0], 0xff);
                s = _mm256_set_m128(a, a);

                table_ymm[0] = _mm256_set_m128(dst1[1], dst1[0]);
                table_ymm[0] = _mm256_div_ps(table_ymm[0], s);

                table_ymm[1] = _mm256_set_m128(dst1[3], dst1[2]);
                table_ymm[1] = _mm256_div_ps(table_ymm[1], s);


                return *this;
            }
        };
    };
};
#endif