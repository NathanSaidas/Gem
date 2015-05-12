#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4x4.h"


#include <glm\gtx\vector_angle.hpp>


namespace Gem
{
	RDEFINE_CLASS(Quaternion, object)


    Quaternion::Quaternion() : object(), glm::quat()
    {

    }
    Quaternion::Quaternion(glm::quat & aQuat) : object(), glm::quat(aQuat)
    {

    }
	Quaternion::Quaternion(Float32 w, Float32 x, Float32 y, Float32 z) : object(), glm::quat()
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
    Quaternion::~Quaternion()
    {

    }

    float Quaternion::Angle(const Quaternion & aQuatA, const Quaternion & aQuatB)                           
    {
        //TODO: Implement to return an angle between two rotations
        return 0.0f;
    }
    float Quaternion::Dot(const Quaternion & aQuatA, const Quaternion & aQuatB)                             
    {
        return glm::dot(aQuatA.Raw(), aQuatB.Raw());
    }
    Quaternion Quaternion::Inverse(const Quaternion & aRotation)                                            
    {
        return Quaternion(glm::inverse(aRotation.Raw()));
    }
    Quaternion Quaternion::Euler(const Vector3 & aEulerAngles)                                              
    {
        return Quaternion(glm::quat(
            glm::vec3(
            glm::radians(aEulerAngles.x), 
            glm::radians(aEulerAngles.y), 
            glm::radians(aEulerAngles.z))
            ));
    }
    Quaternion Quaternion::LookRotation(const Vector3 & aLookDirection)                                     
    {
        glm::quat quat1 = RotationBetween(Vector3::Forward(), aLookDirection).Raw();

        glm::vec3 desiredUp = Vector3::Up().Raw();
        glm::vec3 right = glm::cross(aLookDirection, desiredUp);
        desiredUp = glm::cross(right, aLookDirection);

        glm::vec3 newUp = quat1 * Vector3::Up().Raw();
        glm::quat quat2 = RotationBetween(newUp, desiredUp);
        return quat2 * quat1;
    }

    Quaternion Quaternion::LookRotation(const Vector3 & aLookDirection, const Vector3 & aDesiredUp)
    {
        glm::quat quat1 = RotationBetween(Vector3::Forward(), aLookDirection).Raw();

        glm::vec3 desiredUp = aDesiredUp.Raw();
        glm::vec3 right = glm::cross(aLookDirection, desiredUp);
        desiredUp = glm::cross(right, aLookDirection);

        glm::vec3 newUp = quat1 * Vector3::Up().Raw();
        glm::quat quat2 = RotationBetween(newUp, desiredUp);
        return quat2 * quat1;
    }
    Quaternion Quaternion::Slerp(const Quaternion & aFrom, const Quaternion & aTo, const Float32 & aTime)   
    {
        return glm::slerp(aFrom.Raw(), aTo.Raw(), aTime);
    }

    Quaternion Quaternion::Identity()
    {
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    }

    Quaternion Quaternion::RotationBetween(Vector3 aStart, Vector3 aEnd)
    {
        glm::vec3 start = glm::normalize(aStart.Raw());
        glm::vec3 end = glm::normalize(aEnd.Raw());

        float cosTheta = glm::dot(start, end);
        glm::vec3 rotationAxis;

        if (cosTheta < -1 + 0.001f)
        {
            rotationAxis = glm::cross(Vector3::Forward(), start);
            if (glm::length2(rotationAxis) < 0.01f)
            {
                rotationAxis = glm::cross(Vector3::Right(), start);
            }
            rotationAxis = glm::normalize(rotationAxis);
            return glm::angleAxis(180.0f, rotationAxis);
        }
        rotationAxis = glm::cross(start, end);

        float s = glm::sqrt( (1 + cosTheta) * 2.0f);
        float invs = 1 / s;

        return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
    }

    Vector3 Quaternion::GetEulerAngles() const
    {
        return Vector3(glm::eulerAngles(Raw()));
    }

    Matrix4x4 Quaternion::GetRotationMatrix() const
    {
        return Matrix4x4(glm::toMat4(Raw()));
    }
	Vector3 Quaternion::GetDirection() const
	{
		return (*this) * Vector3::Forward();
	}
    glm::quat Quaternion::Raw() const
    {
        return glm::quat(w, x, y, z);
    }
}

#ifdef GLM_FORCE_RADIANS
#undef GLM_FORCE_RADIANS
#endif