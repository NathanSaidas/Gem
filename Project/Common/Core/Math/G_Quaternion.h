#ifndef G_QUATERNION_H
#define G_QUATERNION_H
#include <math.h>
#include "G_Vector.h"
#include "G_Matrix.h"

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{

    class Quaternion : public Object
    {
    public:

        float x, y, z, w;

        Quaternion(){}
        Quaternion(float aX, float aY, float aZ, float aW)
            :x(aX),y(aY),z(aZ),w(aW){}
        Quaternion(float aReal, const Vector3 & aVec)
            :x(aReal),y(aVec.x),z(aVec.y),w(aVec.z){}

        //From euler angles
        Quaternion(float aX, float aY, float aZ);
        Quaternion(const Vector3 & aVec);
        ~Quaternion();


        Quaternion &operator =(const Quaternion & aValue)
        {
            x = aValue.x;
            y = aValue.y;
            z = aValue.z;
            w = aValue.w;
            return *this;
        }
        const Quaternion operator +(const Quaternion & aValue) const
        {
            return Quaternion(x + aValue.x, y + aValue.y, z + aValue.z, w + aValue.w);
        }
        const Quaternion operator -(const Quaternion & aValue) const
        {
            return Quaternion(x - aValue.x, y - aValue.y, z - aValue.z, w - aValue.w);
        }
        const Quaternion operator *(const Quaternion & aValue) const
        {
            return Quaternion(
                /*x*/w * aValue.x + x * aValue.w - y * aValue.z + z * aValue.y,
                /*y*/w * aValue.y + x * aValue.z + y * aValue.w - z * aValue.x,
                /*z*/w * aValue.z - x * aValue.y + y * aValue.x + z * aValue.w,
                /*w*/w * aValue.w - x * aValue.x - y * aValue.y - z * aValue.z
                );
        }
        const Vector3 operator * (const Vector3 & aValue) const
        {
            float sqx = x * x;
            float sqy = y * y;
            float sqz = z * z;
            float sqw = w * w;

            Matrix3x3 mat;
            mat.m11 = (1.0f - 2.0f * sqy - 2*.0f * sqz);
            mat.m12 = 2.0f * (x * y + w * z);
            mat.m13 = 2.0f * (x * z - w * y);

            mat.m21 = 2.0f * (x * y - w * z);
            mat.m22 = (1.0f - 2.0f * sqx - 2.0f * sqz);
            mat.m23 = 2.0f * (y * z + w * x);

            mat.m31 = 2.0f * (x * z + w * y);
            mat.m32 = 2.0f * (y * z - w * x);
            mat.m33 = (1.0f - 2.0f * sqx - 2.0f * sqy);

            return mat * aValue;
        }
        const Vector3 eulerAngles() const;

        static const Quaternion euler(float x, float y, float z)
        {
            return Quaternion(x,y,z);
        }
        static const Quaternion euler(Vector3 aVec)
        {
            return Quaternion(aVec);
        }

        inline float length()
        {
            return sqrtf(x * x + y * y + z* z + w * w);
        }

        inline Quaternion normalized()
        {
            Quaternion newQuat;
            float lLength = length();
            newQuat.x = x/lLength;
            newQuat.y = y/lLength;
            newQuat.z = z/lLength;
            newQuat.w = w/lLength;
            return newQuat;
        }
        inline void normalize()
        {
            float lLength = length();
            x = x/lLength;
            y = y/lLength;
            z = z/lLength;
            w = w/lLength;
        }

        inline Quaternion conjugate()
        {
            return Quaternion(-x,-y,-z,w);
        }

        inline Quaternion multiply(Quaternion aValue)
        {
            float lw = w * aValue.w - x * aValue.x - y * aValue.y - z * aValue.z;
            float lx = x * aValue.w + w * aValue.x + y * aValue.z - z * aValue.y;
            float ly = y * aValue.w + w * aValue.y + z * aValue.x - x * aValue.z;
            float lz = z * aValue.w + w * aValue.z + x * aValue.y - y * aValue.x;

            return Quaternion(lx,ly,lz,lw);
        }

        inline static Quaternion identity()
        {
            return Quaternion(0.0f,0.0f,0.0f,1.0f);
        }

        inline Quaternion multiply(Vector3 aVec)
        {
            float localW = -x * aVec.x - y * aVec.y - z * aVec.z;
            float localX =  w * aVec.x + y * aVec.z - z * aVec.y;
            float localY =  w * aVec.y + z * aVec.x - x * aVec.z;
            float localZ =  w * aVec.z + x * aVec.y - y * aVec.x;

            return Quaternion(localX,localY,localZ,localW);
        }
        virtual Pointer<Reflection::Type> getType() override;
    private:
    };
}
GEM_CLASS(Quaternion,Object)

#endif