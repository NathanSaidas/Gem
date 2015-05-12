#ifndef GAME_ENGINE_VECTOR_4_H
#define GAME_ENGINE_VECTOR_4_H

#include "MathTypes.h"

#include <glm\glm.hpp>

namespace Gem
{
	template class GEM_API Gem::Reflection::MetaObject<Gem::Vector4>;

    class GEM_API Vector4 : public object, public glm::vec4
    {
		RDECLARE_CLASS(Vector4)
    public:
        Vector4();
        Vector4(Float32 x, Float32 y, Float32 z, Float32 w);
        Vector4(const Vector4 & aVec);
        Vector4(const glm::vec4 & aVec);
        ~Vector4();

        static Float32 Angle(const Vector4 & aFrom, const Vector4 & aTo);
        static Vector4 ClampMagnitude(const Vector4 & aVector, float aMaxLength);
        static Float32 Distance(const Vector4 & aFrom, const Vector4 & aTo);
        static Float32 Dot(const Vector4 & aFrom, const Vector4 & aTo);
        static Vector4 Lerp(const Vector4 & aFrom, const Vector4 & aTo, float aTime);

        static Vector4 Reflect(const Vector4 & aIncident, const Vector4 & aNormal);
        static Vector4 Refract(const Vector4 & aIncident, const Vector4 & aNormal, const Float32 & aTheta);
        static Vector4 FaceForward(const Vector4 & aNormal, const Vector4 & aIncident, const Vector4 & aNormalRef);
        static Vector4 RotateX(const Vector4 & aVec, Float32 aAngle);
        static Vector4 RotateY(const Vector4 & aVec, Float32 aAngle);
        static Vector4 RotateZ(const Vector4 & aVec, Float32 aAngle);

        void Scale(const Vector4 & aScale);
        void Scale(const Float32 & aX, const Float32 & aY, const Float32 & aZ, const Float32 & aW);
        Float32 GetMagnitude() const;
        Float32 GetSqrMagnitude() const;
        Vector4 Normalized() const;
        void Normalize();

        inline void Set(Float32 x, Float32 y, Float32 z, Float32 w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }
        inline void Set(const Vector4 & aVector)
        {
            x = aVector.x;
            y = aVector.y;
            z = aVector.z;
            w = aVector.w;
        }

        //Converts the data into raw 8 bit format. (12 bytes)
        glm::vec4 Raw() const;

    private:
    };

    TYPE_DEFINE(Vector4)
}

#endif
