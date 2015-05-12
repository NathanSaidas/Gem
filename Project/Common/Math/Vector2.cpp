#include "Vector2.h"
#include "../Utilities/StringExtensions.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm\gtx\vector_angle.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\compatibility.hpp>

namespace Gem
{
	RDEFINE_CLASS(Vector2, object)
	Vector2::Vector2() : object(), glm::vec2()
	{
		
	}
	Vector2::Vector2(Float32 x, Float32 y) : object(), glm::vec2(x,y)
	{

	}
	Vector2::Vector2(const Vector2 & aVec) : object(), glm::vec2(aVec.x,aVec.y)
	{

	}
	Vector2::Vector2(const glm::vec2 & aVec) : object(), glm::vec2(aVec)
	{

	}
	Vector2::~Vector2()
	{

	}

	float Vector2::Angle(const Vector2 & aFrom, const Vector2 & aTo)
	{
		glm::vec2 fromRaw = aFrom.Raw();
		glm::vec2 toRaw = aTo.Raw();
		return glm::angle(fromRaw, toRaw);
	}
	Vector2 Vector2::ClampMagnitude(const Vector2 & aVector, float aMaxLength)
	{
        float magnitude = glm::clamp(aVector.GetMagnitude(), -aMaxLength, aMaxLength);
        return aVector.Normalized() * magnitude;
	}
	float Vector2::Distance(const Vector2 & aFrom, const Vector2 & aTo)
	{
		return glm::distance(aFrom.Raw(), aTo.Raw());
	}
	float Vector2::Dot(const Vector2 & aFrom, const Vector2 & aTo)
	{
		return glm::dot(aFrom.Raw(), aTo.Raw());
	}
	Vector2 Vector2::Lerp(const Vector2 & aFrom, const Vector2 & aTo, float aTime)
	{
		return Vector2(glm::lerp(aFrom.Raw(), aTo.Raw(), aTime));
	}
	
	Vector2 Vector2::Reflect(const Vector2 & aIncident, const Vector2 & aNormal)
	{
		return Vector2(glm::reflect(aIncident.Raw(), aNormal.Raw()));
	}
	Vector2 Vector2::Refract(const Vector2 & aIncident, const Vector2 & aNormal, const float & aTheta)
	{
		return Vector2(glm::refract(aIncident.Raw(), aNormal.Raw(), aTheta));
	}
	Vector2 Vector2::FaceForward(const Vector2 & aNormal, const Vector2 & aIncident, const Vector2 & aNormalRef)
	{
		return Vector2(glm::faceforward(aNormal.Raw(), aIncident.Raw(), aNormalRef.Raw()));
	}

    Vector2 Vector2::Rotate(const Vector2 & aVec, const float & aAngle)
    {
        return Vector2(glm::rotate(aVec.Raw(), aAngle));
    }

	void Vector2::Scale(const Vector2 & aScale)
	{
		x = aScale.x * x;
		y = aScale.y * y;
	}
	void Vector2::Scale(const float & aX, const float & aY)
	{
		x = aX * x;
		y = aY * y;
	}
	float Vector2::GetMagnitude() const
	{
		return glm::sqrt(x * x + y * y);
	}
	float Vector2::GetSqrMagnitude() const
	{
		return x * x + y * y;
	}
	Vector2 Vector2::Normalized() const
	{
		glm::vec2 vec(x, y);
		return Vector2(glm::normalize(vec));
	}
	void Vector2::Normalize()
	{
		glm::vec2 vec(x, y);
		vec = glm::normalize(vec);
		x = vec.x;
		y = vec.y;
	}

	glm::vec2 Vector2::Raw() const
	{
		return glm::vec2(x, y);
	}

	std::string Vector2::ToString()
	{
		return std::string("x: ").append(Utilities::F2S(x)).append(" y: ").append(Utilities::F2S(y));
	}
}
#ifdef GLM_FORCE_RADIANS
#undef GLM_FORCE_RADIANS
#endif