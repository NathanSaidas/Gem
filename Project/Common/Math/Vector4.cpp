#include "Vector4.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm\gtx\vector_angle.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\compatibility.hpp>




namespace Gem
{
	RDEFINE_CLASS(Vector4, object)
    Vector4::Vector4() : object(), glm::vec4()
    {
        
    }
    Vector4::Vector4(Float32 x, Float32 y, Float32 z, Float32 w) : object() , glm::vec4(x,y,z,w)
    {

    }
    Vector4::Vector4(const Vector4 & aVec) : object(), glm::vec4(aVec.x, aVec.y, aVec.z, aVec.w)
    {

    }
    Vector4::Vector4(const glm::vec4 & aVec) : object(), glm::vec4(aVec)
    {

    }
    Vector4::~Vector4()
    {

    }

    Float32 Vector4::Angle(const Vector4 & aFrom, const Vector4 & aTo)
    {
        return glm::angle(aFrom.Raw(), aTo.Raw());
    }
    Vector4 Vector4::ClampMagnitude(const Vector4 & aVector, Float32 aMaxLength)
    {
        Float32 magnitude = glm::clamp(aVector.GetMagnitude(), -aMaxLength, aMaxLength);
        return aVector.Normalized() * magnitude;
    }
    Float32 Vector4::Distance(const Vector4 & aFrom, const Vector4 & aTo)
    {
        return glm::distance(aFrom.Raw(), aTo.Raw());
    }
    Float32 Vector4::Dot(const Vector4 & aFrom, const Vector4 & aTo)
    {
        return glm::dot(aFrom.Raw(), aTo.Raw());
    }

    Vector4 Vector4::Lerp(const Vector4 & aFrom, const Vector4 & aTo, Float32 aTime)
    {
        return Vector4(glm::lerp(aFrom.Raw(), aTo.Raw(), aTime));
    }

    Vector4 Vector4::Reflect(const Vector4 & aIncident, const Vector4 & aNormal)
    {
        return Vector4(glm::reflect(aIncident.Raw(), aNormal.Raw()));
    }
    Vector4 Vector4::Refract(const Vector4 & aIncident, const Vector4 & aNormal, const Float32 & aTheta)
    {
        return Vector4(glm::refract(aIncident.Raw(), aNormal.Raw(), aTheta));
    }
    Vector4 Vector4::FaceForward(const Vector4 & aNormal, const Vector4 & aIncident, const Vector4 & aNormalRef)
    {
        return Vector4(glm::faceforward(aNormal.Raw(), aIncident.Raw(), aNormalRef.Raw()));
    }
    Vector4 Vector4::RotateX(const Vector4 & aVec, Float32 aAngle)
    {
        return Vector4(glm::rotateX(aVec.Raw(), aAngle));
    }
    Vector4 Vector4::RotateY(const Vector4 & aVec, Float32 aAngle)
    {
        return Vector4(glm::rotateY(aVec.Raw(), aAngle));
    }
    Vector4 Vector4::RotateZ(const Vector4 & aVec, Float32 aAngle)
    {
        return Vector4(glm::rotateZ(aVec.Raw(), aAngle));
    }

    void Vector4::Scale(const Vector4 & aScale)
    {
        x = aScale.x * x;
        y = aScale.y * y;
        z = aScale.z * z;
        w = aScale.w * w;
    }
    void Vector4::Scale(const Float32 & aX, const Float32 & aY, const Float32 & aZ, const Float32 & aW)
    {
        x = aX * x;
        y = aY * y;
        z = aZ * z;
        w = aW * w;
    }
    Float32 Vector4::GetMagnitude() const
    {
        return glm::sqrt(x * x + y * y + z * z + w * w);
    }
    Float32 Vector4::GetSqrMagnitude() const
    {
        return x * x + y * y + z * z + w * w;
    }
    Vector4 Vector4::Normalized() const
    {
        return Vector4(glm::normalize(Raw()));
    }

    void Vector4::Normalize()
    {
        glm::vec4 vec = glm::normalize(Raw());
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
    }

    //Converts the data into raw 8 bit format. (12 bytes)
    glm::vec4 Vector4::Raw() const
    {
        return glm::vec4(x, y, z, w);
    }
}

#ifdef GLM_FORCE_RADIANS
#undef GLM_FORCE_RADIANS
#endif
