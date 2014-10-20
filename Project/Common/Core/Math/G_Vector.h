#ifndef OL_VECTOR_H
#define OL_VECTOR_H

#include "../Primitives/G_PrimitiveObjects.h"
#include <math.h>

namespace Gem
{
    class Vector2 : public Object
    {
    public:
        Vector2();
        Vector2(float x, float y);
        ~Vector2();
        
        Vector2 operator+(Vector2 & aVec)
        {
            return add(aVec);
        }
        Vector2 operator-(Vector2 & aVec)
        {
            return subtract(aVec);
        }
        Vector2 operator*(Vector2 & aVec)
        {
            return multiply(aVec);
        }
        Vector2 operator/(Vector2 & aVec)
        {
            return divide(aVec);
        }
        Vector2 operator*(float & aValue)
        {
            return scalarMultiply(aValue);
        }
        Vector2 operator/(float & aValue)
        {
            return scalarDivide(aValue);
        }

        inline Vector2 operator=(Vector2 & aVec)
        {
            x = aVec.x;
            y = aVec.y;
            return Vector2(x,y);
        }
        inline bool operator==(Vector2 & aVec)
        {
            return (x == aVec.x && y == aVec.y);
        }

        inline Vector2 add(Vector2 & aVec)
        {
            return Vector2(x + aVec.x,y + aVec.y);
        }
        inline Vector2 subtract(Vector2 & aVec)
        {
            return Vector2(x - aVec.x,y - aVec.y);
        }
        inline Vector2 multiply(Vector2 & aVec)
        {
            return Vector2(x * aVec.x,y * aVec.y);
        }
        inline Vector2 divide(Vector2 & aVec)
        {
            return Vector2(x / aVec.x,y / aVec.y);
        }
        inline Vector2 scalarMultiply(float & aValue)
        {
            return Vector2(x * aValue, y * aValue);
        }
        inline Vector2 scalarDivide(float & aValue)
        {
            return Vector2(x / aValue, y / aValue);
        }
        inline float dot(Vector2 & aVec)
        {
            return x * aVec.x + y * aVec.y;
        }
        inline float cross(Vector2 & aVec)
        {
            return x * aVec.y - y * aVec.x;
        }
        inline float length()
        {
            return sqrtf(x * x + y * y);
        }
        inline float sqrLength()
        {
            return x * x + y * y;
        }
        inline Vector2 normalized()
        {
            float len = length();
            return Vector2(x/len,y/len);
        }
        inline void normalize()
        {
            float len = length();
            x /= len;
            y /= len;
        }
        inline void rotateLeft()
        {
            float lX = x;
            float lY = y;
            x = -lY;
            y = lX;
        }
        inline void rotateRight()
        {
            float lX = x;
            float lY = y;
            x = y;
            y = -x;
        }
        inline void rotate(float & aAngle)
        {
            float lX = x;
            float lY = y;

            x = lX * cosf(aAngle) - lY * sinf(aAngle);
            y = lX * sinf(aAngle) - lY * cosf(aAngle);
        }
        inline float angle()
        {
            return atan2f(x,y);
        }

        inline Vector2 lerp(Vector2 & aFrom, Vector2 & aTo, float & aTime)
        {
            float time = 1.0f - aTime;
            return aFrom * aTime + aTo * time;
        }

        virtual Pointer<Reflection::Type> getType() override;

        static Vector2 one()
        {
            return Vector2(1.0f,1.0f);
        }
        static Vector2 zero()
        {
            return Vector2();
        }
        //Members
        union
        {
            struct
            {
                float x;
                float y;
            };
        };
        union
        {
            struct
            {
                float u;
                float v;
            };
        };
    };

    class Vector3 : public Object
    {
    public:
        Vector3();
        Vector3(Vector2 aVec);
        Vector3(Vector2 aVec, float z);
        Vector3(float x, Vector2 aVec);
        Vector3(float x, float y, float z);
        ~Vector3();

        inline Vector3 add(Vector2 & aVec)
        {
            return Vector3(x + aVec.x,y + aVec.y,z);
        }
        inline Vector3 add(Vector3 & aVec)
        {
            return Vector3(x + aVec.x, y + aVec.y, z + aVec.z);
        }
        inline Vector3 subtract(Vector2 & aVec)
        {
            return Vector3(x - aVec.x,y - aVec.y,z);
        }
        inline Vector3 subtract(Vector3 & aVec)
        {
            return Vector3(x - aVec.x, y - aVec.y, z - aVec.z);
        }
        inline static Vector3 subtract(const Vector3 & aLeft, const Vector3 & aRight)
        {
            Vector3 result;
            result.x = aLeft.x - aRight.x;
            result.y = aLeft.y - aRight.y;
            result.z = aLeft.z - aRight.z;
            return result;
        }
        inline Vector3 multiply(Vector2 & aVec)
        {
            return Vector3(x * aVec.x,y * aVec.y,z);
        }
        inline Vector3 multiply(Vector3 & aVec)
        {
            return Vector3(x * aVec.x, y * aVec.y, z * aVec.z);
        }
        Vector3 & operator *(Vector3 & aVec)
        {
            return multiply(aVec);
        }
        inline Vector3 divide(Vector2 & aVec)
        {
            return Vector3(x / aVec.x,y / aVec.y,z);
        }
        inline Vector3 divide(Vector3 & aVec)
        {
            return Vector3(x / aVec.x, y / aVec.y, z / aVec.z);
        }

        inline Vector3 scalarMultiply(float & aValue)
        {
            return Vector3(x * aValue, y * aValue, z * aValue);
        }
        inline Vector3 scalarDivide(float & aValue)
        {
            return Vector3(x / aValue, y / aValue, z / aValue);
        }

		///Dot Product Confirmed Working October,19,2014
        inline float dot(const Vector3 & aVec)
        {
            return x * aVec.x + y * aVec.y + z * aVec.z;
        }
		///Dot Product Confirmed Working October,19,2014
		inline static float dot(const Vector3 & aLeft, const Vector3 & aRight)
		{
			return aLeft.x * aRight.x + aLeft.y * aRight.y + aLeft.z * aRight.z;
		}
		///Cross Product Confirmed Working October,19,2014
        inline Vector3 cross(const Vector3 & aVec)
        {
            return Vector3(y * aVec.z - z * aVec.y, z * aVec.x - x * aVec.z, x * aVec.y - y * aVec.x);
        }
		///Cross Product Confirmed Working October,19,2014
        inline static Vector3 cross(const Vector3 & aLeft, const Vector3 & aRight)
        {
            Vector3 result;
            result.x = aLeft.y * aRight.z - aLeft.z * aRight.y;
            result.y = aLeft.z * aRight.x - aLeft.x * aRight.z;
            result.z = aLeft.x * aRight.y - aLeft.y * aRight.x;
            return result;
        }

		///Length Confirmed Working October,19,2014
        inline float length()
        {
            return sqrtf(x * x + y * y + z * z);
        }
		///SqrLength Confirmed Working October,19,2014
        inline float sqrLength()
        {
            return x * x + y * y + z * z;
        }
		///Normalized Confirmed Working October,19,2014
        inline Vector3 normalized()
        {
            float len = length();
            return Vector3(x/len,y/len,z/len);
        }

		///Normalized Confirmed Working October,19,2014
        inline void normalize()
        {
            float len = length();
            x /= len;
            y /= len;
            z /= len;
        }

        Vector3 rotate(float aAngle, Vector3 aAxis);
		string toString();
        virtual Pointer<Reflection::Type> getType() override;

        inline void setZero()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        inline static const Vector3 one()
        {
            return Vector3(1.0f,1.0f,1.0f);
        }
        inline static const Vector3 zero()
        {
            return Vector3(0.0f,0.0f,0.0f);
        }
        inline static const Vector3 up()
        {
            return Vector3(0.0f,1.0f,0.0f);
        }
        inline static const Vector3 forward()
        {
            return Vector3(0.0f,0.0f,1.0f);
        }
		inline static const Vector3 right()
		{
			return Vector3(1.0f, 0.0f, 0.0f);
		}
        //Members
        //union
        //{
        //    float x;
        //    float r;
        //};
        //union
        //{
        //    float y;
        //    float g;
        //};
        //union
        //{
        //    float z;
        //    float b;
        //};
        float x;
        float y;
        float z;
    };

    class Vector4 : public Object
    {
    public:
        Vector4(){}
        Vector4(float aX, float aY, float aZ, float aW)
            : x(aX), y(aY), z(aZ),w(aW){}

        union
        {
            float x;
            float y;
            float z;
            float w;
        };
        union
        {
            float r;
            float g;
            float b;
            float a;
        };

        virtual Pointer<Reflection::Type> getType() override;
    };
}
GEM_CLASS(Vector2,Object)
GEM_CLASS(Vector3,Object)
GEM_CLASS(Vector4,Object)
#endif