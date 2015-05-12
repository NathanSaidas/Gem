#ifndef GAME_ENGINE_MATH_H
#define GAME_ENGINE_MATH_H

//This math library wraps around glm's math functions for vector / matrix/ quaternion operations.
//Wrapping around provides clear functionality and reflection / extended class support. 

//TODO: Implement a serailization system to serialize the types
//TODO: Implmenet ToString method to return a formatted string of the proper data types.

#define GLM_FORCE_RADIANS

#include "MathTypes.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

namespace Gem
{
	namespace Math
	{
		inline int __fastcall Clamp(int value, int min, int max)
		{
			return value > max ? max : (value < min ? min : value);
		}

		inline float __fastcall Clamp(float value, float min, float max)
		{
			return value > max ? max : (value < min ? min : value);
		}

		inline float __fastcall Abs(float value)
		{
			return value < 0.0f ? -value : value;
		}

		inline Vector2 __fastcall Abs(Vector2 value)
		{
			return Vector2(Abs(value.x), Abs(value.y));
		}

	}
}





/* Comparison Test*/

/* 

printf("Engine\n");
{
Vector2 a(3.0f, 4.0f);
Vector2 b(1.0f, 5.0f);

Vector2 buffer(0.0f, 1.0f);

float angle = Vector2::Angle(a, b);
printf("angle %f\n", angle);
float distance = Vector2::Distance(a, b);
printf("distance %f\n", distance);
float dot = Vector2::Dot(a, b);
printf("dot %f\n", dot);
Vector2 lerp = Vector2::Lerp(a, b, 0.75f);
printf("lerp %f | %f\n", lerp.x, lerp.y);
Vector2 reflect = Vector2::Reflect(a, b);
printf("reflect %f | %f\n", reflect.x, reflect.y);
Vector2 refract = Vector2::Refract(a, b, cos(45.0f));
printf("refract %f | %f\n", refract.x, refract.y);
Vector2 faceForward = Vector2::FaceForward(a, b, buffer);
printf("faceForward %f | %f\n", faceForward.x, faceForward.y);

b.Scale(5.0f, 2.0f);
float mag = a.GetMagnitude();
printf("mag %f\n", mag);
float sqrMag = a.GetMagnitude();
Vector2 normalized = a.Normalized();
printf("normalized %f | %f\b", normalized.x, normalized.y);
a.Normalize();

glm::vec2 raw = a.Raw();
glm::vec2 imp = a;

}
printf("GLM\n");
{
glm::vec2 a(3.0f, 4.0f);
glm::vec2 b(1.0f, 5.0f);

glm::vec2 buffer(0.0f, 1.0f);

float angle = glm::angle(a, b);
printf("angle %f\n", angle);
float distance = glm::distance(a, b);
printf("distance %f\n", distance);
float dot = glm::dot(a, b);
printf("dot %f\n", dot);
glm::vec2 lerp = glm::lerp(a, b, 0.75f);
printf("lerp %f | %f\n", lerp.x, lerp.y);
glm::vec2 reflect = glm::reflect(a, b);
printf("reflect %f | %f\n", reflect.x, reflect.y);
glm::vec2 refract = glm::refract(a, b, cos(45.0f));
printf("refract %f | %f\n", refract.x, refract.y);
glm::vec2 faceForward = glm::faceforward(a, b, buffer);
printf("faceForward %f | %f\n", faceForward.x, faceForward.y);
float mag = glm::length(a);
printf("mag %f\n", mag);
Vector2 normalized = glm::normalize(a);
printf("normalized %f | %f\n", normalized.x, normalized.y);
}
*/




#endif
