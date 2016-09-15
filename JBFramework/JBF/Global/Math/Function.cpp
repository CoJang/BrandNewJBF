﻿#include"pch.h"
#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
            INLINE float Abs(float v){
                unsigned long* p = (decltype(p))&v;
                *p = *p << 1 >> 1;
                return v;
            }

            Matrix* MatrixLookAtLH(Matrix* pOut, const Vector3* pEye, const Vector3* pAt, const Vector3* pUp){
                Vector3 zaxis = ((*pAt) - (*pEye)).normalized();
                Vector3 xaxis = pUp->crossed(zaxis).normalized();
                Vector3 yaxis = zaxis.crossed(xaxis);

                pOut->_11 = xaxis.x;
                pOut->_21 = xaxis.y;
                pOut->_31 = xaxis.z;
                pOut->_41 = -xaxis.dot(*pEye);

                pOut->_12 = yaxis.x;
                pOut->_22 = yaxis.y;
                pOut->_32 = yaxis.z;
                pOut->_42 = -yaxis.dot(*pEye);

                pOut->_13 = zaxis.x;
                pOut->_23 = zaxis.y;
                pOut->_33 = zaxis.z;
                pOut->_43 = -zaxis.dot(*pEye);

                pOut->_14 = 0.f;
                pOut->_24 = 0.f;
                pOut->_34 = 0.f;
                pOut->_44 = 1.f;

                return pOut;
            }
            Matrix* MatrixPerspectiveFovLH(Matrix* pOut, float fovy, float Aspect, float zn, float zf){
                float h = 1.f / Tan(fovy * 0.5f);
                float w = h / Aspect;

                pOut->_11 = w;
                pOut->_12 = 0.f;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = h;
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = 0.f;
                pOut->_33 = zf / (zf - zn);
                pOut->_34 = 1.f;

                pOut->_41 = 0.f;
                pOut->_42 = 0.f;
                pOut->_43 = -(zn * zf) / (zf - zn);
                pOut->_44 = 0.f;

                return pOut;
            }
        };
    };
};