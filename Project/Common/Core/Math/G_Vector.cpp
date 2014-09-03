#include "G_Vector.h"
#include "../Reflection/G_Reflection.h"
#include "G_Math.h"
#include "G_Quaterion.h"
#include <pugixml.hpp>
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
    Pointer<Reflection::Type> Vector2::getType()
    {
        return typeOf("Vector2");
    }
    pugi::xml_node Vector2::serialize(pugi::xml_node & aNode,bool aIncludeTypeInfo)
    {
        aNode.append_attribute("X") = x;
        aNode.append_attribute("Y") = y;
        return aNode;
    }
    bool Vector2::deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo)
    {
        int threshHold = 2;
        //if(aIncludeTypeInfo == true)
        //{
        //    
        //}
        int count = 0;
        std::string name = "";
        for(pugi::xml_attribute_iterator iter = aNode.attributes_begin(); iter != aNode.attributes_end(); ++iter)
        {
            if(count > threshHold)
            {
                break;
            }

            name = iter->name();
            if(name == "X")
            {
                x = iter->as_float();
                count ++;
            }
            else if(name == "Y")
            {
                y = iter->as_float();
                count ++;
            }
            
        }
        return count >= threshHold;
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

        Quaterion rotation(rX,rY,rZ,rW);
        Quaterion conjugate = rotation.conjugate();

        Quaterion w = rotation.multiply((*this)).multiply(conjugate);

        return Vector3(w.x,w.y,w.z);
    }
    pugi::xml_node Vector3::serialize(pugi::xml_node & aNode,bool aIncludeTypeInfo)
    {
        aNode.append_attribute("X") = x;
        aNode.append_attribute("Y") = y;
        aNode.append_attribute("Z") = z;
        return aNode;
    }
    bool Vector3::deserialize(pugi::xml_node & aNode, bool aIncludeTypeInfo )
    {
        int threshHold = 3;
        int count = 0;
        std::string name = "";
        for(pugi::xml_attribute_iterator iter = aNode.attributes_begin(); iter != aNode.attributes_end(); ++iter)
        {
            if(count > threshHold)
            {
                break;
            }

            name = iter->name();
            if(name == "X")
            {
                x = iter->as_float();
                count ++;
            }
            else if(name == "Y")
            {
                y = iter->as_float();
                count ++;
            }
            else if(name == "Z")
            {
                z = iter->as_float();
                count ++;
            }
            
        }
        return count >= threshHold;
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