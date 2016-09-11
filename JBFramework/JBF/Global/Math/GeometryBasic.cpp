#include"pch.h"

#ifndef USE_INTRINSIC
#include<algorithm>
#include<utility>
#include<cfloat>

#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
            Vector3::Vector3(float fill){
                std::fill(table + 0, table + 3, fill);
            }
            Vector3::Vector3(float _x, float _y, float _z){
                table[0] = _x;
                table[1] = _y;
                table[2] = _z;
            }
            Vector3::Vector3(const Vector3& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
            }

            Vector3& Vector3::operator=(const Vector3& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
                return *this;
            }

            bool MATH_CALL Vector3::operator!()const{
                return
                    (table[0] == 0.f) &&
                    (table[1] == 0.f) &&
                    (table[2] == 0.f)
                    ;
            }
            bool MATH_CALL Vector3::operator==(const Vector3& rhs)const{
                return
                    (Abs(table[0] - rhs.table[0]) <= FLT_EPSILON) &&
                    (Abs(table[1] - rhs.table[1]) <= FLT_EPSILON) &&
                    (Abs(table[2] - rhs.table[2]) <= FLT_EPSILON)
                    ;
            }

            Vector3& MATH_CALL Vector3::operator+=(const Vector3& rhs){
                table[0] += rhs.table[0];
                table[1] += rhs.table[1];
                table[2] += rhs.table[2];

                return *this;
            }
            Vector3& MATH_CALL Vector3::operator-=(const Vector3& rhs){
                table[0] -= rhs.table[0];
                table[1] -= rhs.table[1];
                table[2] -= rhs.table[2];

                return *this;
            }
            Vector3& MATH_CALL Vector3::operator*=(const Vector3& rhs){
                table[0] *= rhs.table[0];
                table[1] *= rhs.table[1];
                table[2] *= rhs.table[2];

                return *this;
            }
            Vector3& MATH_CALL Vector3::operator/=(const Vector3& rhs){
                table[0] /= rhs.table[0];
                table[1] /= rhs.table[1];
                table[2] /= rhs.table[2];

                return *this;
            }

            Vector3& MATH_CALL Vector3::operator*=(float rhs){
                table[0] *= rhs;
                table[1] *= rhs;
                table[2] *= rhs;

                return *this;
            }
            Vector3& MATH_CALL Vector3::operator/=(float rhs){
                rhs = 1.f / rhs;

                table[0] *= rhs;
                table[1] *= rhs;
                table[2] *= rhs;

                return *this;
            }

            Vector3& MATH_CALL Vector3::normalize(){
                float len = length();

                if (len != 0.f){
                    len = 1.f / len;

                    table[0] *= len;
                    table[1] *= len;
                    table[2] *= len;
                }
                else ZeroMemory(table, sizeof table);

                return *this;
            }
            Vector3& MATH_CALL Vector3::cross(const Vector3& rhs){
                Vector3 cpy = *this;

                table[0] = (cpy.table[1] * rhs.table[2]) - (cpy.table[2] * rhs.table[1]);
                table[1] = (cpy.table[2] * rhs.table[0]) - (cpy.table[0] * rhs.table[2]);
                table[2] = (cpy.table[0] * rhs.table[1]) - (cpy.table[1] * rhs.table[0]);

                return *this;
            }

            float MATH_CALL Vector3::dot(const Vector3& rhs)const{
                return
                    (table[0] * rhs.table[0]) +
                    (table[1] * rhs.table[1]) +
                    (table[2] * rhs.table[2])
                    ;
            }
            float MATH_CALL Vector3::length()const{
                return
                    ((!(*this)) == false) ?
                    std::sqrt(
                        (table[0] * table[0]) +
                        (table[1] * table[1]) +
                        (table[2] * table[2])
                    )
                    :
                    0.f;
            }


            Vector4::Vector4(float fill){
                std::fill(table + 0, table + 4, fill);
            }
            Vector4::Vector4(float _x, float _y, float _z, float _w){
                table[0] = _x;
                table[1] = _y;
                table[2] = _z;
                table[3] = _w;
            }
            Vector4::Vector4(const Vector4& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
            }

            Vector4& Vector4::operator=(const Vector4& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
                return *this;
            }

            bool MATH_CALL Vector4::operator!()const{
                return
                    (table[0] == 0.f) &&
                    (table[1] == 0.f) &&
                    (table[2] == 0.f) &&
                    (table[3] == 0.f)
                    ;
            }
            bool MATH_CALL Vector4::operator==(const Vector4& rhs)const{
                return
                    (Abs(table[0] - rhs.table[0]) <= FLT_EPSILON) &&
                    (Abs(table[1] - rhs.table[1]) <= FLT_EPSILON) &&
                    (Abs(table[2] - rhs.table[2]) <= FLT_EPSILON) &&
                    (Abs(table[3] - rhs.table[3]) <= FLT_EPSILON)
                    ;
            }

            Vector4& MATH_CALL Vector4::operator+=(const Vector4& rhs){
                table[0] += rhs.table[0];
                table[1] += rhs.table[1];
                table[2] += rhs.table[2];
                table[3] += rhs.table[3];

                return *this;
            }
            Vector4& MATH_CALL Vector4::operator-=(const Vector4& rhs){
                table[0] -= rhs.table[0];
                table[1] -= rhs.table[1];
                table[2] -= rhs.table[2];
                table[3] -= rhs.table[3];

                return *this;
            }
            Vector4& MATH_CALL Vector4::operator*=(const Vector4& rhs){
                table[0] *= rhs.table[0];
                table[1] *= rhs.table[1];
                table[2] *= rhs.table[2];
                table[3] *= rhs.table[3];

                return *this;
            }
            Vector4& MATH_CALL Vector4::operator/=(const Vector4& rhs){
                table[0] /= rhs.table[0];
                table[1] /= rhs.table[1];
                table[2] /= rhs.table[2];
                table[3] /= rhs.table[3];

                return *this;
            }

            Vector4& MATH_CALL Vector4::operator*=(float rhs){
                table[0] *= rhs;
                table[1] *= rhs;
                table[2] *= rhs;
                table[3] *= rhs;

                return *this;
            }
            Vector4& MATH_CALL Vector4::operator/=(float rhs){
                rhs = 1.f / rhs;

                table[0] *= rhs;
                table[1] *= rhs;
                table[2] *= rhs;
                table[3] *= rhs;

                return *this;
            }


            Matrix::Matrix(float fill){
                std::fill(table + 0, table + 16, fill);
            }
            Matrix::Matrix(
                float __11, float __12, float __13, float __14,
                float __21, float __22, float __23, float __24,
                float __31, float __32, float __33, float __34,
                float __41, float __42, float __43, float __44
            ){
                _11 = __11;
                _12 = __12;
                _13 = __13;
                _14 = __14;

                _21 = __21;
                _22 = __22;
                _23 = __23;
                _24 = __24;

                _31 = __31;
                _32 = __32;
                _33 = __33;
                _34 = __34;

                _41 = __41;
                _42 = __42;
                _43 = __43;
                _44 = __44;
            }
            Matrix::Matrix(const Matrix& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
            }

            Matrix& Matrix::operator=(const Matrix& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
                return *this;
            }

            bool MATH_CALL Matrix::operator!()const{
                size_t i;
                const float* ptr = table;

                for (i = 0; i < 16; ++i, ++ptr){
                    if (*ptr != 0.f)return false;
                }

                return true;
            }
            bool MATH_CALL Matrix::operator==(const Matrix& rhs)const{
                size_t i;
                const float* ptrSelf = table;
                const float* ptrRhs = rhs.table;

                for (i = 0; i < 16; ++i, ++ptrSelf, ++ptrRhs){
                    if (Abs(*ptrSelf - *ptrRhs) > FLT_EPSILON)return false;
                }

                return true;
            }

            Matrix& MATH_CALL Matrix::operator+=(const Matrix& rhs){
                size_t i;
                float* ptrSelf = table;
                const float* ptrRhs = rhs.table;

                for (i = 0; i < 16; ++i, ++ptrSelf, ++ptrRhs)*ptrSelf += *ptrRhs;

                return *this;
            }
            Matrix& MATH_CALL Matrix::operator-=(const Matrix& rhs){
                size_t i;
                float* ptrSelf = table;
                const float* ptrRhs = rhs.table;

                for (i = 0; i < 16; ++i, ++ptrSelf, ++ptrRhs)*ptrSelf -= *ptrRhs;

                return *this;
            }
            Matrix& MATH_CALL Matrix::operator*=(const Matrix& rhs){
                Matrix cpy = *this;

                size_t i, j;
                float* ptrSelf = table;
                const float* ptrRhs = rhs.table;
                float* ptrCpy = cpy.table;

                for (i = 0; i < 16; i += 4){
                    for (j = 0; j < 4; ++j){
                        ptrSelf[i + j] = ptrCpy[i] * ptrRhs[j];
                        ptrSelf[i + j] += ptrCpy[i + 1] * ptrRhs[j + 4];
                        ptrSelf[i + j] += ptrCpy[i + 2] * ptrRhs[j + 8];
                        ptrSelf[i + j] += ptrCpy[i + 3] * ptrRhs[j + 12];
                    }
                }

                return *this;
            }

            Matrix& MATH_CALL Matrix::operator*=(float rhs){
                size_t i;
                float* ptr = table;

                for (i = 0; i < 16; ++i, ++ptr)*ptr *= rhs;

                return *this;
            }
            Matrix& MATH_CALL Matrix::operator/=(float rhs){
                size_t i;
                float* ptr = table;

                rhs = 1.f / rhs;

                for (i = 0; i < 16; ++i, ++ptr)*ptr *= rhs;

                return *this;
            }

            Matrix& MATH_CALL Matrix::identify(){
                ZeroMemory(table, sizeof table);
                _11 = _22 = _33 = _44 = 1.f;

                return *this;
            }
            Matrix& MATH_CALL Matrix::transpose(){
                size_t i, j;
                float(*ptr)[4] = (float(*)[4])table;

                for (j = 0; j < 3; ++j){
                    for (i = j + 1; i < 4; ++i){
                        std::swap(ptr[j][i], ptr[i][j]);
                    }
                }

                return *this;
            }
            Matrix& MATH_CALL Matrix::inverse(){
                Matrix out;

                size_t i;
                float* ptrSelf = table;
                float* ptrOut = out.table;
                float tmp[12];
                float det;

                transpose();

                tmp[0] = ptrSelf[10] * ptrSelf[15];
                tmp[1] = ptrSelf[11] * ptrSelf[14];
                tmp[2] = ptrSelf[9] * ptrSelf[15];
                tmp[3] = ptrSelf[11] * ptrSelf[13];
                tmp[4] = ptrSelf[9] * ptrSelf[14];
                tmp[5] = ptrSelf[10] * ptrSelf[13];
                tmp[6] = ptrSelf[8] * ptrSelf[15];
                tmp[7] = ptrSelf[11] * ptrSelf[12];
                tmp[8] = ptrSelf[8] * ptrSelf[14];
                tmp[9] = ptrSelf[10] * ptrSelf[12];
                tmp[10] = ptrSelf[8] * ptrSelf[13];
                tmp[11] = ptrSelf[9] * ptrSelf[12];

                ptrOut[0] = tmp[0] * ptrSelf[5] + tmp[3] * ptrSelf[6] + tmp[4] * ptrSelf[7];
                ptrOut[0] -= tmp[1] * ptrSelf[5] + tmp[2] * ptrSelf[6] + tmp[5] * ptrSelf[7];
                ptrOut[1] = tmp[1] * ptrSelf[4] + tmp[6] * ptrSelf[6] + tmp[9] * ptrSelf[7];
                ptrOut[1] -= tmp[0] * ptrSelf[4] + tmp[7] * ptrSelf[6] + tmp[8] * ptrSelf[7];
                ptrOut[2] = tmp[2] * ptrSelf[4] + tmp[7] * ptrSelf[5] + tmp[10] * ptrSelf[7];
                ptrOut[2] -= tmp[3] * ptrSelf[4] + tmp[6] * ptrSelf[5] + tmp[11] * ptrSelf[7];
                ptrOut[3] = tmp[5] * ptrSelf[4] + tmp[8] * ptrSelf[5] + tmp[11] * ptrSelf[6];
                ptrOut[3] -= tmp[4] * ptrSelf[4] + tmp[9] * ptrSelf[5] + tmp[10] * ptrSelf[6];
                ptrOut[4] = tmp[1] * ptrSelf[1] + tmp[2] * ptrSelf[2] + tmp[5] * ptrSelf[3];
                ptrOut[4] -= tmp[0] * ptrSelf[1] + tmp[3] * ptrSelf[2] + tmp[4] * ptrSelf[3];
                ptrOut[5] = tmp[0] * ptrSelf[0] + tmp[7] * ptrSelf[2] + tmp[8] * ptrSelf[3];
                ptrOut[5] -= tmp[1] * ptrSelf[0] + tmp[6] * ptrSelf[2] + tmp[9] * ptrSelf[3];
                ptrOut[6] = tmp[3] * ptrSelf[0] + tmp[6] * ptrSelf[1] + tmp[11] * ptrSelf[3];
                ptrOut[6] -= tmp[2] * ptrSelf[0] + tmp[7] * ptrSelf[1] + tmp[10] * ptrSelf[3];
                ptrOut[7] = tmp[4] * ptrSelf[0] + tmp[9] * ptrSelf[1] + tmp[10] * ptrSelf[2];
                ptrOut[7] -= tmp[5] * ptrSelf[0] + tmp[8] * ptrSelf[1] + tmp[11] * ptrSelf[2];

                tmp[0] = ptrSelf[2] * ptrSelf[7];
                tmp[1] = ptrSelf[3] * ptrSelf[6];
                tmp[2] = ptrSelf[1] * ptrSelf[7];
                tmp[3] = ptrSelf[3] * ptrSelf[5];
                tmp[4] = ptrSelf[1] * ptrSelf[6];
                tmp[5] = ptrSelf[2] * ptrSelf[5];
                tmp[6] = ptrSelf[0] * ptrSelf[7];
                tmp[7] = ptrSelf[3] * ptrSelf[4];
                tmp[8] = ptrSelf[0] * ptrSelf[6];
                tmp[9] = ptrSelf[2] * ptrSelf[4];
                tmp[10] = ptrSelf[0] * ptrSelf[5];
                tmp[11] = ptrSelf[1] * ptrSelf[4];

                ptrOut[8] = tmp[0] * ptrSelf[13] + tmp[3] * ptrSelf[14] + tmp[4] * ptrSelf[15];
                ptrOut[8] -= tmp[1] * ptrSelf[13] + tmp[2] * ptrSelf[14] + tmp[5] * ptrSelf[15];
                ptrOut[9] = tmp[1] * ptrSelf[12] + tmp[6] * ptrSelf[14] + tmp[9] * ptrSelf[15];
                ptrOut[9] -= tmp[0] * ptrSelf[12] + tmp[7] * ptrSelf[14] + tmp[8] * ptrSelf[15];
                ptrOut[10] = tmp[2] * ptrSelf[12] + tmp[7] * ptrSelf[13] + tmp[10] * ptrSelf[15];
                ptrOut[10] -= tmp[3] * ptrSelf[12] + tmp[6] * ptrSelf[13] + tmp[11] * ptrSelf[15];
                ptrOut[11] = tmp[5] * ptrSelf[12] + tmp[8] * ptrSelf[13] + tmp[11] * ptrSelf[14];
                ptrOut[11] -= tmp[4] * ptrSelf[12] + tmp[9] * ptrSelf[13] + tmp[10] * ptrSelf[14];
                ptrOut[12] = tmp[2] * ptrSelf[10] + tmp[5] * ptrSelf[11] + tmp[1] * ptrSelf[9];
                ptrOut[12] -= tmp[4] * ptrSelf[11] + tmp[0] * ptrSelf[9] + tmp[3] * ptrSelf[10];
                ptrOut[13] = tmp[8] * ptrSelf[11] + tmp[0] * ptrSelf[8] + tmp[7] * ptrSelf[10];
                ptrOut[13] -= tmp[6] * ptrSelf[10] + tmp[9] * ptrSelf[11] + tmp[1] * ptrSelf[8];
                ptrOut[14] = tmp[6] * ptrSelf[9] + tmp[11] * ptrSelf[11] + tmp[3] * ptrSelf[8];
                ptrOut[14] -= tmp[10] * ptrSelf[11] + tmp[2] * ptrSelf[8] + tmp[7] * ptrSelf[9];
                ptrOut[15] = tmp[10] * ptrSelf[10] + tmp[4] * ptrSelf[8] + tmp[9] * ptrSelf[9];
                ptrOut[15] -= tmp[8] * ptrSelf[9] + tmp[11] * ptrSelf[10] + tmp[5] * ptrSelf[8];

                det = ptrSelf[0] * ptrOut[0];
                det += ptrSelf[1] * ptrOut[1];
                det += ptrSelf[2] * ptrOut[2];
                det += ptrSelf[3] * ptrOut[3];

                det = 1.f / det;
                for (i = 0; i < 16; ++i, ++ptrSelf, ++ptrOut)*ptrSelf = (*ptrOut) * det;

                return *this;
            }
        };
    };
};
#endif