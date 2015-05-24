#ifndef GAME_ENGINE_VECTOR_2_H
#define GAME_ENGINE_VECTOR_2_H

#include "MathTypes.h"

#include <glm\glm.hpp>

namespace Gem
{
	template class GEM_API Gem::Reflection::MetaObject<Gem::Vector2>;

	class GEM_API Vector2 : public object, public glm::vec2
	{
		RDECLARE_CLASS(Vector2)
	public:
		Vector2();
		Vector2(Float32 x, Float32 y);
		Vector2(const Vector2 & aVec);
		Vector2(const glm::vec2 & aVec);
		~Vector2();

		static float Angle(const Vector2 & aFrom, const Vector2 & aTo);
		static Vector2 ClampMagnitude(const Vector2 & aVector, float aMaxLength);
		static float Distance(const Vector2 & aFrom, const Vector2 & aTo);
		static float Dot(const Vector2 & aFrom, const Vector2 & aTo);
		static Vector2 Lerp(const Vector2 & aFrom, const Vector2 & aTo, float aTime);
        
		
		static Vector2 Reflect(const Vector2 & aIncident, const Vector2 & aNormal);
		static Vector2 Refract(const Vector2 & aIncident, const Vector2 & aNormal, const float & aTheta);
		static Vector2 FaceForward(const Vector2 & aNormal, const Vector2 & aIncident, const Vector2 & aNormalRef);
        static Vector2 Rotate(const Vector2 & aVec, const float & aAngle);


		void Scale(const Vector2 & aScale);
		void Scale(const float & aX, const float & aY);
        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector2 Normalized() const;
		void Normalize();

        inline void Set(Float32 x, Float32 y)
        {
            this->x = x;
            this->y = y;
        }
        inline void Set(const Vector2 & aVector)
        {
            x = aVector.x;
            y = aVector.y;
		}

		inline static Vector2 Zero()
		{
			return Vector2(0.0f, 0.0f);
		}

		inline static Vector2 One()
		{
			return Vector2(1.0f, 1.0f);
		}

		//Converts the data into raw 16 bit format. (8 bytes)
		glm::vec2 Raw() const;

		std::string ToString();
	private:
		
	};

	TYPE_DEFINE(Vector2)
}

#endif