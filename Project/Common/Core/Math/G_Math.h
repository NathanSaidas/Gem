#ifndef OL_MATH_H
#define OL_MATH_H

#include <math.h>
#include "G_Vector.h"
#include "G_Matrix.h"
#include "G_Quaterion.h"

namespace Gem
{
    const double PI = 3.14159265358979;

    

    class Math
    {
    public:
        inline static float degreesToRad(float aDegrees)
        {
            return (aDegrees * (PI / 180));
        }
        inline static float radToDegrees(float aRadians)
        {
            return (aRadians * (180/(float)PI));
        }
        inline static float clamp(float aValue, float aMin, float aMax)
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
        inline static Vector3 degreesToRad(Vector3 aDegrees)
        {
            aDegrees.x = degreesToRad(aDegrees.x);
            aDegrees.y = degreesToRad(aDegrees.y);
            aDegrees.z = degreesToRad(aDegrees.z);
            return aDegrees;
        }
        inline static Vector3 radToDegrees(Vector3 aRadians)
        {
            aRadians.x = radToDegrees(aRadians.x);
            aRadians.y = radToDegrees(aRadians.y);
            aRadians.z = radToDegrees(aRadians.z);
            return aRadians;
        }
    };
}


#endif