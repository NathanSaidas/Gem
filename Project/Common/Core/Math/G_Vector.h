#ifndef OL_VECTOR_H
#define OL_VECTOR_H

#include "../Base Objects/G_Object.h"
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

        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);

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
            float x;
            float y;
        };
        union
        {
            float u;
            float v;
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
        inline Vector3 multiply(Vector2 & aVec)
        {
            return Vector3(x * aVec.x,y * aVec.y,z);
        }
        inline Vector3 multiply(Vector3 & aVec)
        {
            return Vector3(x * aVec.x, y * aVec.y, z * aVec.z);
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

        inline float dot(Vector3 & aVec)
        {
            return x * aVec.x + y * aVec.y + z * aVec.z;
        }
        inline Vector3 cross(Vector3 & aVec)
        {
            return Vector3(y * aVec.z - z * aVec.y, z * aVec.x - x * aVec.z, x * aVec.y - y * aVec.x);
        }
        
        inline float length()
        {
            return sqrtf(x * x + y * y + z * z);
        }
        inline float sqrLength()
        {
            return x * x + y * y + z * z;
        }
        inline Vector3 normalized()
        {
            float len = length();
            return Vector3(x/len,y/len,z/len);
        }
        inline void normalize()
        {
            float len = length();
            x /= len;
            y /= len;
            z /= len;
        }

        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);


        //Members
        union
        {
            float x;
            float y;
            float z;
        };
        union
        {
            float r;
            float g;
            float b;
        };
    };
}

#endif