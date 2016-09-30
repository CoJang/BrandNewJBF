#pragma once

#include<cmath>

#include"Common.h"
#include"Function.h"
#include"Geometry.h"

namespace JBF{
    namespace Global{
        namespace Math{
            static const double PId = 3.1415926535897932384626433832795;
            static const float PIf = 3.1415926535897932384626433832795f;

            static const double RAD2DEGd = 180. / PId;
            static const float RAD2DEGf = 180.f / PIf;
            static const double DEG2RADd = PId / 180.;
            static const float DEG2RADf = PIf / 180.f;
        };
    };
};