#pragma once

#include"JBF/Definitions.h"

#ifdef USE_INTRINSIC
#include<intrin.h>
#endif

namespace JBF{
    namespace Global{
        namespace Math{
            struct __declspec(align(8)) Vector2{
            public:
                union{
                    struct{
                        float x, y;
                    };
                    float table[2];
                };

            public:
                Vector2(){}
                Vector2(float fill);
                Vector2(float _x, float _y);
                Vector2(const Vector2& rhs);
            public:
                void* operator new(size_t);
                void* operator new[](size_t);
                void operator delete(void*);
                void operator delete[](void*);
            public:
                Vector2& operator=(const Vector2& rhs);
            public:
                bool operator!()const;
                bool operator==(const Vector2& rhs)const;
                INLINE bool operator!=(const Vector2& rhs)const{ return !(*this == rhs); }
            public:
                Vector2& operator+=(const Vector2& rhs);
                Vector2& operator-=(const Vector2& rhs);
                Vector2& operator*=(const Vector2& rhs);
                Vector2& operator/=(const Vector2& rhs);

                Vector2& operator*=(float rhs);
                Vector2& operator/=(float rhs);
            public:
                Vector2& normalize();
            public:
                float dot(const Vector2& rhs)const;
                float length()const;
            public:
                INLINE Vector2 operator+(const Vector2& rhs)const{ return Vector2(*this) += rhs; }
                INLINE Vector2 operator-(const Vector2& rhs)const{ return Vector2(*this) -= rhs; }
                INLINE Vector2 operator*(const Vector2& rhs)const{ return Vector2(*this) *= rhs; }
                INLINE Vector2 operator/(const Vector2& rhs)const{ return Vector2(*this) /= rhs; }

                INLINE Vector2 operator*(float rhs)const{ return Vector2(*this) *= rhs; }
                INLINE Vector2 operator/(float rhs)const{ return Vector2(*this) /= rhs; }
            public:
                INLINE Vector2 normalized()const{ return Vector2(*this).normalize(); }
            };

            struct __declspec(align(16)) Vector3{
            public:
                union{
                    struct{
                        float x, y, z;
                    };
                    float table[3];
#ifdef USE_INTRINSIC
                    __m128 table_xmm;
#endif
                };

            public:
                Vector3(){}
                Vector3(float fill);
                Vector3(float _x, float _y, float _z);
                Vector3(const Vector3& rhs);
            public:
                void* operator new(size_t);
                void* operator new[](size_t);
                void operator delete(void*);
                void operator delete[](void*);
            public:
                Vector3& operator=(const Vector3& rhs);
            public:
                bool MATH_CALL operator!()const;
                bool MATH_CALL operator==(const Vector3& rhs)const;
                INLINE bool operator!=(const Vector3& rhs)const{ return !(*this == rhs); }
            public:
                Vector3& MATH_CALL operator+=(const Vector3& rhs);
                Vector3& MATH_CALL operator-=(const Vector3& rhs);
                Vector3& MATH_CALL operator*=(const Vector3& rhs);
                Vector3& MATH_CALL operator/=(const Vector3& rhs);

                Vector3& MATH_CALL operator*=(float rhs);
                Vector3& MATH_CALL operator/=(float rhs);
            public:
                Vector3& MATH_CALL normalize();
                Vector3& MATH_CALL cross(const Vector3& rhs);
            public:
                float MATH_CALL dot(const Vector3& rhs)const;
                float MATH_CALL length()const;
            public:
                INLINE Vector3 operator+(const Vector3& rhs)const{ return Vector3(*this) += rhs; }
                INLINE Vector3 operator-(const Vector3& rhs)const{ return Vector3(*this) -= rhs; }
                INLINE Vector3 operator*(const Vector3& rhs)const{ return Vector3(*this) *= rhs; }
                INLINE Vector3 operator/(const Vector3& rhs)const{ return Vector3(*this) /= rhs; }

                INLINE Vector3 operator*(float rhs)const{ return Vector3(*this) *= rhs; }
                INLINE Vector3 operator/(float rhs)const{ return Vector3(*this) /= rhs; }
            public:
                INLINE Vector3 normalized()const{ return Vector3(*this).normalize(); }
                INLINE Vector3 crossed(const Vector3& rhs)const{ return Vector3(*this).cross(rhs); }
            };

            struct __declspec(align(16)) Vector4{
            public:
                union{
                    struct{
                        float x, y, z, w;
                    };
                    float table[4];
#ifdef USE_INTRINSIC
                    __m128 table_xmm;
#endif
                };

            public:
                Vector4(){}
                Vector4(float fill);
                Vector4(float _x, float _y, float _z, float _w);
                Vector4(const Vector4& rhs);
            public:
                void* operator new(size_t);
                void* operator new[](size_t);
                void operator delete(void*);
                void operator delete[](void*);
            public:
                Vector4& operator=(const Vector4& rhs);
            public:
                bool MATH_CALL operator!()const;
                bool MATH_CALL operator==(const Vector4& rhs)const;
                INLINE bool operator!=(const Vector4& rhs)const{ return !(*this == rhs); }
            public:
                Vector4& MATH_CALL operator+=(const Vector4& rhs);
                Vector4& MATH_CALL operator-=(const Vector4& rhs);
                Vector4& MATH_CALL operator*=(const Vector4& rhs);
                Vector4& MATH_CALL operator/=(const Vector4& rhs);

                Vector4& MATH_CALL operator*=(float rhs);
                Vector4& MATH_CALL operator/=(float rhs);
            public:
                INLINE Vector4 operator+(const Vector4& rhs)const{ return Vector4(*this) += rhs; }
                INLINE Vector4 operator-(const Vector4& rhs)const{ return Vector4(*this) -= rhs; }
                INLINE Vector4 operator*(const Vector4& rhs)const{ return Vector4(*this) *= rhs; }
                INLINE Vector4 operator/(const Vector4& rhs)const{ return Vector4(*this) /= rhs; }

                INLINE Vector4 operator*(float rhs)const{ return Vector4(*this) *= rhs; }
                INLINE Vector4 operator/(float rhs)const{ return Vector4(*this) /= rhs; }
            };

            struct __declspec(align(32)) Matrix{
            public:
                union{
                    struct{
                        float _11, _12, _13, _14;
                        float _21, _22, _23, _24;
                        float _31, _32, _33, _34;
                        float _41, _42, _43, _44;
                    };
                    float m[4][4];
                    float table[16];
#ifdef USE_INTRINSIC
                    __m256 table_ymm[2];
                    __m128 table_xmm[4];
#endif
                };

            public:
                Matrix(){}
                Matrix(float fill);
                Matrix(
                    float __11, float __12, float __13, float __14,
                    float __21, float __22, float __23, float __24,
                    float __31, float __32, float __33, float __34,
                    float __41, float __42, float __43, float __44
                );
                Matrix(const Matrix& rhs);
            public:
                void* operator new(size_t);
                void* operator new[](size_t);
                void operator delete(void*);
                void operator delete[](void*);
            public:
                Matrix& operator=(const Matrix& rhs);
            public:
                bool MATH_CALL operator!()const;
                bool MATH_CALL operator==(const Matrix& rhs)const;
                INLINE bool operator!=(const Matrix& rhs)const{ return !(*this == rhs); }
            public:
                Matrix& MATH_CALL operator+=(const Matrix& rhs);
                Matrix& MATH_CALL operator-=(const Matrix& rhs);
                Matrix& MATH_CALL operator*=(const Matrix& rhs);

                Matrix& MATH_CALL operator*=(float rhs);
                Matrix& MATH_CALL operator/=(float rhs);
            public:
                Matrix& MATH_CALL identify();
                Matrix& MATH_CALL transpose();
                Matrix& MATH_CALL inverse();
            public:
                INLINE Matrix operator+(const Matrix& rhs)const{ return Matrix(*this) += rhs; }
                INLINE Matrix operator-(const Matrix& rhs)const{ return Matrix(*this) -= rhs; }
                INLINE Matrix operator*(const Matrix& rhs)const{ return Matrix(*this) *= rhs; }

                INLINE Matrix operator*(float rhs)const{ return Matrix(*this) *= rhs; }
                INLINE Matrix operator/(float rhs)const{ return Matrix(*this) /= rhs; }
            public:
                INLINE Matrix transposed()const{ return Matrix(*this).transpose(); }
                INLINE Matrix inversed()const{ return Matrix(*this).inverse(); }

            public:
                static const Matrix constIdentity;
            };
        };
    };
};