#include "G_Vector.h"
#include "../Reflection/G_Reflection.h"
#include "G_Math.h"
#include "G_Quaternion.h"
#include "../Utilities/G_Utilities.h";
namespace Gem
{
    using namespace Reflection;
    //Vector2
    Vector2::Vector2()
    {
        x = 0.0f;
        y = 0.0f;
    }
    Vector2::Vector2(float aX, float aY)
    {
        x = aX;
        y = aY;
    }
    Vector2::~Vector2()
    {
    }
	string Vector2::toString()
	{
		return string("X:") + F2S(x) + " Y:" + F2S(y);
	}
    Pointer<Reflection::Type> Vector2::getType()
    {
        return typeOf("Vector2");
    }
   
    //Vector3
    Vector3::Vector3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector3::Vector3(Vector2 aVec)
    {
        x = aVec.x;
        y = aVec.y;
        z = 0.0f;
    }
    Vector3::Vector3(Vector2 aVec, float aZ)
    {
        x = aVec.x;
        y = aVec.y;
        z = aZ;
    }
    Vector3::Vector3(float aX, Vector2 aVec)
    {
        x = aX;
        y = aVec.x;
        z = aVec.y;
    }
    Vector3::Vector3(float aX, float aY, float aZ)
    {
        x = aX;
        y = aY;
        z = aZ;
    }
    Vector3::~Vector3()
    {

    }
    Vector3 Vector3::rotate(float aAngle, Vector3 aAxis)
    {
        
        float sinHalfAngle = (float)sin(Math::degreesToRad(aAngle / 2.0f));
        float cosHalfAngle = (float)cos(Math::degreesToRad(aAngle / 2.0f));

        float rX = aAxis.x * sinHalfAngle;
        float rY = aAxis.y * sinHalfAngle;
        float rZ = aAxis.z * sinHalfAngle;
        float rW = cosHalfAngle;

        Quaternion rotation(rX, rY, rZ, rW);
        Quaternion conjugate = rotation.conjugate();

        Quaternion w = rotation.multiply((*this)).multiply(conjugate);

        return Vector3(w.x,w.y,w.z);
    }
	string Vector3::toString()
	{
		return string("");
	}
    Pointer<Reflection::Type> Vector3::getType()
    {
        return typeOf("Vector3");
    }

    Pointer<Reflection::Type> Vector4::getType()
    {
        return typeOf("Vector4");
    }
}