#ifndef GAME_ENGINE_QUATERNION_H
#define GAME_ENGINE_QUATERNION_H

#include "MathTypes.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Gem
{
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4018)
#endif

	template class GEM_API Gem::Reflection::MetaObject<Gem::Quaternion>;

    class GEM_API Quaternion : public object, public glm::quat
    {
		RDECLARE_CLASS(Quaternion)
    public:
        Quaternion();
        Quaternion(glm::quat & aQuat);
		Quaternion(Float32 w, Float32 x, Float32 y, Float32 z);
        ~Quaternion();

        static float Angle(const Quaternion & aQuatA, const Quaternion & aQuatB);
        static float Dot(const Quaternion & aQuatA, const Quaternion & aQuatB);
        static Quaternion Inverse(const Quaternion & aRotation);
        static Quaternion Euler(const Vector3 & aEulerAngles);
        static Quaternion LookRotation(const Vector3 & aLookDirection);
        static Quaternion LookRotation(const Vector3 & aLookDirection, const Vector3 & aDesiredUp);
        static Quaternion Slerp(const Quaternion & aFrom, const Quaternion & aTo, const Float32 & aTime);
        static Quaternion Identity();

        static Quaternion RotationBetween(Vector3 aStart, Vector3 aEnd);
        

        Vector3 GetEulerAngles() const;
        Matrix4x4 GetRotationMatrix() const;
		Vector3 GetDirection() const;
        
        glm::quat Raw() const;
    };

    TYPE_DEFINE(Quaternion)
#ifdef _MSC_VER
#pragma warning (pop)
#endif

}

#endif