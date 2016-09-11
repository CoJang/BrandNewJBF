#include"pch.h"

#ifndef USE_INTRINSIC
#include<cmath>

#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
            INLINE float MATH_CALL Sin(float v){ return sin(v); }
            INLINE float MATH_CALL Cos(float v){ return cos(v); }

            Vector3* MATH_CALL Vec3TransformCoord(Vector3* pOut, const Vector3* pV, const Matrix* pM){
                Vector3 i = *pV;
                pOut->x = i.x * pM->_11 + i.y * pM->_21 + i.z * pM->_31 + pM->_41;
                pOut->y = i.x * pM->_12 + i.y * pM->_22 + i.z * pM->_32 + pM->_42;
                pOut->z = i.x * pM->_13 + i.y * pM->_23 + i.z * pM->_33 + pM->_43;

                *(&pOut->z + 1) = i.x * pM->_14 + i.y * pM->_24 + i.z * pM->_34 + pM->_44;
                *(&pOut->z + 1) = 1.f / *(&pOut->z + 1);

                pOut->x *= *(&pOut->z + 1);
                pOut->y *= *(&pOut->z + 1);
                pOut->z *= *(&pOut->z + 1);

                return pOut;
            }
            Vector4* MATH_CALL Vec4TransformCoord(Vector4* pOut, const Vector4* pV, const Matrix* pM){
                Vector4 i = *pV;
                pOut->x = i.x * pM->_11 + i.y * pM->_21 + i.z * pM->_31 + i.w * pM->_41;
                pOut->y = i.x * pM->_12 + i.y * pM->_22 + i.z * pM->_32 + i.w * pM->_42;
                pOut->z = i.x * pM->_13 + i.y * pM->_23 + i.z * pM->_33 + i.w * pM->_43;
                pOut->w = i.x * pM->_14 + i.y * pM->_24 + i.z * pM->_34 + i.w * pM->_44;
                return pOut;
            }
        };
    };
};
#endif