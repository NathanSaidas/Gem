#ifndef G_QUATERION_H
#define G_QUATERION_H
#include <math.h>
#include "G_Vector.h"
#include "G_Matrix.h"

#include "../Base Objects/G_Object.h"
#include "../Utilities/G_IXmlSerializable.h"

namespace Gem
{

    class Quaterion : Object, IXmlSerializable
    {
    public:

        float x, y, z, w;

        Quaterion(){}
        Quaterion(float aX, float aY, float aZ, float aW)
            :x(aX),y(aY),z(aZ),w(aW){}
        Quaterion(float aReal, const Vector3 & aVec)
            :x(aReal),y(aVec.x),z(aVec.y),w(aVec.z){}

        //From euler angles
        Quaterion(float aX, float aY, float aZ);
        Quaterion(const Vector3 & aVec);
        ~Quaterion();


        Quaterion &operator =(const Quaterion & aValue)
        {
            x = aValue.x;
            y = aValue.y;
            z = aValue.z;
            w = aValue.w;
            return *this;
        }
        const Quaterion operator +(const Quaterion & aValue) const
        {
            return Quaterion(x + aValue.x, y + aValue.y, z + aValue.z, w + aValue.w);
        }
        const Quaterion operator -(const Quaterion & aValue) const
        {
            return Quaterion(x - aValue.x, y - aValue.y, z - aValue.z, w - aValue.w);
        }
        const Quaterion operator *(const Quaterion & aValue) const
        {
            return Quaterion(
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

        static const Quaterion euler(float x, float y, float z)
        {
            return Quaterion(x,y,z);
        }
        static const Quaterion euler(Vector3 aVec)
        {
            return Quaterion(aVec);
        }

        inline float length()
        {
            return sqrtf(x * x + y * y + z* z + w * w);
        }

        inline Quaterion normalized()
        {
            Quaterion newQuat;
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

        inline Quaterion conjugate()
        {
            return Quaterion(-x,-y,-z,w);
        }

        inline Quaterion multiply(Quaterion aValue)
        {
            float lw = w * aValue.w - x * aValue.x - y * aValue.y - z * aValue.z;
            float lx = x * aValue.w + w * aValue.x + y * aValue.z - z * aValue.y;
            float ly = y * aValue.w + w * aValue.y + z * aValue.x - x * aValue.z;
            float lz = z * aValue.w + w * aValue.z + x * aValue.y - y * aValue.x;

            return Quaterion(lx,ly,lz,lw);
        }

        inline static Quaterion identity()
        {
            return Quaterion(0.0f,0.0f,0.0f,1.0f);
        }

        inline Quaterion multiply(Vector3 aVec)
        {
            float localW = -x * aVec.x - y * aVec.y - z * aVec.z;
            float localX =  w * aVec.x + y * aVec.z - z * aVec.y;
            float localY =  w * aVec.y + z * aVec.x - x * aVec.z;
            float localZ =  w * aVec.z + x * aVec.y - y * aVec.x;

            return Quaterion(localX,localY,localZ,localW);
        }
        virtual pugi::xml_node serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo = false);
        virtual bool deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo = false);
        virtual Reflection::Type * getType();
    private:
    };
}

#endif