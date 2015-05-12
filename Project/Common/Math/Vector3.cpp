#include "Vector3.h"
#include "Vector4.h"
#include "../Utilities/StringExtensions.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm\gtx\vector_angle.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\compatibility.hpp>

namespace Gem
{
	RDEFINE_CLASS(Vector3, object)


    Vector3::Vector3() : object(), glm::vec3()
    {

    }
    Vector3::Vector3(Float32 x, Float32 y, Float32 z) : object(), glm::vec3()
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3::Vector3(const Vector3 & aVec) : object(), glm::vec3()
    {
        x = aVec.x;
        y = aVec.y;
        z = aVec.z;
    }
    Vector3::Vector3(const glm::vec3 & aVec) : object(), glm::vec3()
    {
        x = aVec.x;
        y = aVec.y;
        z = aVec.z;
    }
    Vector3::Vector3(const Vector4 & aVec) : object(), glm::vec3()
    {
        x = aVec.x;
        y = aVec.y;
        z = aVec.z;
    }
    Vector3::~Vector3()
    {

    }

    float Vector3::Angle(const Vector3 & aFrom, const Vector3 & aTo)                
    {
        return glm::angle(aFrom.Raw(), aTo.Raw());
    }
    Vector3 Vector3::ClampMagnitude(const Vector3 & aVector, float aMaxLength)      
    {
        float magnitude = glm::clamp(aVector.GetMagnitude(), -aMaxLength, aMaxLength);
        Vector3 normalizedVector = aVector.Normalized();
        return normalizedVector * magnitude;
    }
    float Vector3::Distance(const Vector3 & aFrom, const Vector3 & aTo)             
    {
        return glm::distance(aFrom.Raw(), aTo.Raw());
    }
    float Vector3::Dot(const Vector3 & aFrom, const Vector3 & aTo)                  
    {
        return glm::dot(aFrom.Raw(), aTo.Raw());
    }

    Vector3 Vector3::Cross(const Vector3 & aX, const Vector3 & aY)
    {
        return Vector3(glm::cross(aX.Raw(), aY.Raw()));
    }
    Vector3 Vector3::Lerp(const Vector3 & aFrom, const Vector3 & aTo, float aTime)  
    {
        return Vector3(glm::lerp(aFrom.Raw(), aTo.Raw(), aTime));
    }


    Vector3 Vector3::Reflect(const Vector3 & aIncident, const Vector3 & aNormal)
    {
        return Vector3(glm::reflect(aIncident.Raw(), aNormal.Raw()));
    }
    Vector3 Vector3::Refract(const Vector3 & aIncident, const Vector3 & aNormal, const float & aTheta)
    {
        return Vector3(glm::refract(aIncident.Raw(), aNormal.Raw(), aTheta));
    }
    Vector3 Vector3::FaceForward(const Vector3 & aNormal, const Vector3 & aIncident, const Vector3 & aNormalRef)
    {
        return Vector3(glm::faceforward(aNormal.Raw(), aIncident.Raw(), aNormalRef.Raw()));
    }
    Vector3 Vector3::RotateX(const Vector3 & aVec, float aAngle)
    {
        return Vector3(glm::rotateX(aVec.Raw(), aAngle));
    }
    Vector3 Vector3::RotateY(const Vector3 & aVec, float aAngle)
    {
        return Vector3(glm::rotateY(aVec.Raw(), aAngle));
    }
    Vector3 Vector3::RotateZ(const Vector3 & aVec, float aAngle)
    {
        return Vector3(glm::rotateZ(aVec.Raw(), aAngle));
    }

    void Vector3::Scale(const Vector3 & aScale)
    {
        x = aScale.x * x;
        y = aScale.y * y;
        z = aScale.z * z;
    }
    void Vector3::Scale(const float & aX, const float & aY, const float & aZ)
    {
        x = aX * x;
        y = aY * y;
        z = aZ * z;
    }
    float Vector3::GetMagnitude() const
    {
        return glm::sqrt(x*x + y*y + z*z);
    }
    float Vector3::GetSqrMagnitude() const
    {
        return x * x + y * y + z * z;
    }
    Vector3 Vector3::Normalized() const
    {
        return Vector3(glm::normalize(Raw()));
    }
    void Vector3::Normalize()                                                
    {
        glm::vec3 vec= glm::normalize(Raw());
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    //Converts the data into raw 8 bit format. (12 bytes)
    glm::vec3 Vector3::Raw() const
    {
        return glm::vec3(x, y, z);
    }

	std::string Vector3::ToString()
	{
		return std::string("x: ").append(Utilities::F2S(x)).append(" y: ").append(Utilities::F2S(y)).append(" z: ").append(Utilities::F2S(z));
	}
}

#ifdef GLM_FORCE_RADIANS
#undef GLM_FORCE_RADIANS
#endif