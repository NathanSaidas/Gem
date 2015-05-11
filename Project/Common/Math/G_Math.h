#ifndef G_MATH_H
#define G_MATH_H

#include <glm/glm.hpp>
#include "Matrix.h"
#include "Vector.h"
#include "../G_Object.h"

namespace Gem
{
    typedef glm::fvec2 Vector2;
    typedef glm::fvec3 Vector3;
    typedef glm::fmat2x2 Matrix2x2;
    typedef glm::fmat3x3 Matrix3x3;
    typedef glm::fmat4x4 Matrix4x4;
    typedef glm::fquat Quaternion;


    class Math : public object
    {
		G_CLASS_DEF(Math)
    public:
		static const float PI;

        inline static float Abs(float & aValue)
        {
            if (aValue < 0.0f)
            {
                return -aValue;
            }
            return aValue;
        }
        inline static Vector2 Abs(Vector2 & aValue)
        {
            return Vector2(Abs(aValue.x), Abs(aValue.y));
        }
        inline static Vector3 Abs(Vector3 & aValue)
        {
            return Vector3(Abs(aValue.x), Abs(aValue.y), Abs(aValue.z));
        }
		inline static float Clamp01(float & aValue)
		{
			if (aValue < 0.0f)
			{
				return 0.0f;
			}
			else if (aValue > 1.0f)
			{
				return 1.0f;
			}
			return aValue;
		}
		inline static float Clamp(float & aValue, float aMin, float aMax)
		{
			if (aValue < aMin)
			{
				return aMin;
			}
			else if (aValue > aMax)
			{
				return aMax;
			}
			return aValue;
		}
    };


	class Rect : public object
	{
		G_CLASS_DEF(Rect)
	public:
		float x;
		float y;
		float width;
		float height;
	};
}
#endif