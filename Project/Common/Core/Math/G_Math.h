#ifndef OL_MATH_H
#define OL_MATH_H

#include <math.h>
#include "G_Vector.h"

namespace Gem
{
    const float PI = 3.14159265358979;

    inline float degreesToRad(float aDegrees)
    {
        return (aDegrees * (PI / 180));
    }
    inline float radToDegrees(float aRadians)
    {
        return (aRadians * (180/PI));
    }
    inline float clamp(float aValue, float aMin, float aMax)
    {
        if(aValue > aMax)
        {
            return aMax;
        }
        if(aValue < aMin)
        {
            return aMin;
        }
        return aValue;
    }

    class Math
    {
    public:
        
        inline static Vector2 abs(Vector2 & aVec)
        {
            aVec.x = fabsf(aVec.x);
            aVec.y = fabsf(aVec.y);
            return aVec;
        }
        inline static Vector3 abs(Vector3 & aVec)
        {
            aVec.x = fabsf(aVec.x);
            aVec.y = fabsf(aVec.y);
            aVec.z = fabsf(aVec.z);
            return aVec;
        }

    };
}


#endif