#include"pch.h"
#include"JBF/Global/Math/Math.h"

namespace JBF{
    namespace Global{
        namespace Math{
            INLINE float Abs(float v){
                unsigned long* p = (decltype(p))&v;
                *p = *p << 1 >> 1;
                return v;
            }

            Matrix* MatrixRotationX(Matrix* pOut, float Angle){
                float cos = Cos(Angle);
                float sin = Sin(Angle);

                pOut->_11 = 1.f;
                pOut->_12 = 0.f;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = cos;
                pOut->_23 = sin;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = -sin;
                pOut->_33 = cos;
                pOut->_34 = 0.f;

                pOut->_41 = 0.f;
                pOut->_42 = 0.f;
                pOut->_43 = 0.f;
                pOut->_44 = 1.f;

                return pOut;
            }
            Matrix* MatrixRotationY(Matrix* pOut, float Angle){
                float cos = Cos(Angle);
                float sin = Sin(Angle);

                pOut->_11 = cos;
                pOut->_12 = 0.f;
                pOut->_13 = -sin;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = 1.f;
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = sin;
                pOut->_32 = 0.f;
                pOut->_33 = cos;
                pOut->_34 = 0.f;

                pOut->_41 = 0.f;
                pOut->_42 = 0.f;
                pOut->_43 = 0.f;
                pOut->_44 = 1.f;

                return pOut;
            }
            Matrix* MatrixRotationZ(Matrix* pOut, float Angle){
                float cos = Cos(Angle);
                float sin = Sin(Angle);

                pOut->_11 = cos;
                pOut->_12 = sin;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = -sin;
                pOut->_22 = cos;
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = 0.f;
                pOut->_33 = 1.f;
                pOut->_34 = 0.f;

                pOut->_41 = 0.f;
                pOut->_42 = 0.f;
                pOut->_43 = 0.f;
                pOut->_44 = 1.f;

                return pOut;
            }

            Matrix* MatrixTranslation(Matrix* pOut, float x, float y, float z){
                pOut->_11 = 1.f;
                pOut->_12 = 0.f;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = 1.f;
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = 0.f;
                pOut->_33 = 1.f;
                pOut->_34 = 0.f;

                pOut->_41 = x;
                pOut->_42 = y;
                pOut->_43 = z;
                pOut->_44 = 1.f;

                return pOut;
            }
            Matrix* MatrixTranslation(Matrix* pOut, const Vector3* pV){
                pOut->_11 = 1.f;
                pOut->_12 = 0.f;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = 1.f;
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = 0.f;
                pOut->_33 = 1.f;
                pOut->_34 = 0.f;

                *(Vector3*)&pOut->_41 = *pV;
                pOut->_44 = 1.f;

                return pOut;
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
            Matrix* MatrixOrthoLH(Matrix* pOut, float w, float h, float zn, float zf){
                pOut->_11 = 2.f / w;
                pOut->_12 = 0.f;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = 2.f / h;
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = 0.f;
                pOut->_33 = 1.f / (zf - zn);
                pOut->_34 = 1.f;

                pOut->_41 = 0.f;
                pOut->_42 = 0.f;
                pOut->_43 = zn / (zn - zf);
                pOut->_44 = 1.f;

                return pOut;
            }
            Matrix* MatrixOrthoOffCenterLH(Matrix* pOut, float l, float r, float b, float t, float zn, float zf){
                pOut->_11 = 2.f / (r - l);
                pOut->_12 = 0.f;
                pOut->_13 = 0.f;
                pOut->_14 = 0.f;

                pOut->_21 = 0.f;
                pOut->_22 = 2.f / (t - b);
                pOut->_23 = 0.f;
                pOut->_24 = 0.f;

                pOut->_31 = 0.f;
                pOut->_32 = 0.f;
                pOut->_33 = 1.f / (zf - zn);
                pOut->_34 = 1.f;

                pOut->_41 = (l + r) / (l - r);
                pOut->_42 = (t + b) / (b - t);
                pOut->_43 = zn / (zn - zf);
                pOut->_44 = 1.f;

                return pOut;
            }
        };
    };
};