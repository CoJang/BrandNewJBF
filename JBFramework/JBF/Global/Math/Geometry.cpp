#include"pch.h"
#include<memory>

#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
            const Matrix Matrix::constIdentity = Matrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );

            void* Vector2::operator new(size_t sz){
                void* p = _aligned_malloc(sz, 8u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void* Vector2::operator new[](size_t sz){
                void* p = _aligned_malloc(sz, 8u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void Vector2::operator delete(void* v){ _aligned_free(v); }
            void Vector2::operator delete[](void* v){ _aligned_free(v); }

                void* Vector3::operator new(size_t sz){
                void* p = _aligned_malloc(sz, 16u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void* Vector3::operator new[](size_t sz){
                void* p = _aligned_malloc(sz, 16u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void Vector3::operator delete(void* v){ _aligned_free(v); }
            void Vector3::operator delete[](void* v){ _aligned_free(v); }

                void* Vector4::operator new(size_t sz){
                void* p = _aligned_malloc(sz, 16u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void* Vector4::operator new[](size_t sz){
                void* p = _aligned_malloc(sz, 16u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void Vector4::operator delete(void* v){ _aligned_free(v); }
            void Vector4::operator delete[](void* v){ _aligned_free(v); }

                void* Matrix::operator new(size_t sz){
                void* p = _aligned_malloc(sz, 32u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void* Matrix::operator new[](size_t sz){
                void* p = _aligned_malloc(sz, 32u);
                if (!p)throw std::bad_alloc();
                return p;
            }
            void Matrix::operator delete(void* v){ _aligned_free(v); }
            void Matrix::operator delete[](void* v){ _aligned_free(v); }


            Vector2::Vector2(float fill){
                std::fill(table + 0, table + 2, fill);
            }
            Vector2::Vector2(float _x, float _y){
                table[0] = _x;
                table[1] = _y;
            }
            Vector2::Vector2(const Vector2& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
            }

            Vector2& Vector2::operator=(const Vector2& rhs){
                memcpy_s(table, sizeof table, rhs.table, sizeof rhs.table);
                return *this;
            }

            bool Vector2::operator!()const{
                return
                    (table[0] == 0.f) &&
                    (table[1] == 0.f)
                    ;
            }
            bool Vector2::operator==(const Vector2& rhs)const{
                return
                    (Abs(table[0] - rhs.table[0]) <= FLT_EPSILON) &&
                    (Abs(table[1] - rhs.table[1]) <= FLT_EPSILON)
                    ;
            }

            Vector2& Vector2::operator+=(const Vector2& rhs){
                table[0] += rhs.table[0];
                table[1] += rhs.table[1];

                return *this;
            }
            Vector2& Vector2::operator-=(const Vector2& rhs){
                table[0] -= rhs.table[0];
                table[1] -= rhs.table[1];

                return *this;
            }
            Vector2& Vector2::operator*=(const Vector2& rhs){
                table[0] *= rhs.table[0];
                table[1] *= rhs.table[1];

                return *this;
            }
            Vector2& Vector2::operator/=(const Vector2& rhs){
                table[0] /= rhs.table[0];
                table[1] /= rhs.table[1];

                return *this;
            }

            Vector2& Vector2::operator*=(float rhs){
                table[0] *= rhs;
                table[1] *= rhs;

                return *this;
            }
            Vector2& Vector2::operator/=(float rhs){
                rhs = 1.f / rhs;

                table[0] *= rhs;
                table[1] *= rhs;

                return *this;
            }

            Vector2& Vector2::normalize(){
                float len = length();

                if (len != 0.f){
                    len = 1.f / len;

                    table[0] *= len;
                    table[1] *= len;
                }
                else ZeroMemory(table, sizeof table);

                return *this;
            }

            float Vector2::dot(const Vector2& rhs)const{
                return
                    (table[0] * rhs.table[0]) +
                    (table[1] * rhs.table[1])
                    ;
            }
            float Vector2::length()const{
                return
                    ((!(*this)) == false) ?
                    std::sqrt(
                        (table[0] * table[0]) +
                        (table[1] * table[1])
                    )
                    :
                    0.f;
            }
        };
    };
};