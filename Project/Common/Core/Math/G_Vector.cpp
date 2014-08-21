#include "G_Vector.h"
#include "../Reflection/G_Reflection.h"

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
        x = aY;
    }
    Vector2::~Vector2()
    {
    }
    Type Vector2::getType()
    {
        return TypeFactory::create("Vector2",TypeID::VECTOR2,sizeof(Vector2));
    }
    Type Vector2::baseType()
    {
        return Object::getType();
    }
    Type * Vector2::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
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
    Vector3::Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3::~Vector3()
    {

    }


    Type Vector3::getType()
    {
        return TypeFactory::create("Vector3",TypeID::VECTOR3,sizeof(Vector3));
    }
    Type Vector3::baseType()
    {
        return Object::getType();
    }
    Type * Vector3::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
    }

}