#pragma once

#include"JBF/Definitions.h"

namespace JBF{
    namespace Global{
        namespace Math{
            struct __declspec(align(8)) Vector2;
            struct __declspec(align(16)) Vector3;
            struct __declspec(align(16)) Vector4;
            struct __declspec(align(32)) Matrix;

            extern INLINE float Abs(float v);

            extern INLINE float MATH_CALL Sin(float v);
            extern INLINE float MATH_CALL Cos(float v);
            extern INLINE float MATH_CALL Tan(float v);

            extern Vector3* MATH_CALL Vec3TransformCoord(Vector3* pOut, const Vector3* pV, const Matrix* pM);
            extern Vector4* MATH_CALL Vec4TransformCoord(Vector4* pOut, const Vector4* pV, const Matrix* pM);

            extern Matrix* MatrixLookAtLH(Matrix* pOut, const Vector3* pEye, const Vector3* pAt, const Vector3* pUp);

            extern Matrix* MatrixPerspectiveFovLH(Matrix* pOut, float fovy, float Aspect, float zn, float zf);
            extern Matrix* MatrixOrthoLH(Matrix* pOut, float w, float h, float zn, float zf);
            extern Matrix* MatrixOrthoOffCenterLH(Matrix* pOut, float l, float r, float b, float t, float zn, float zf);
        };
    };
};