#include "G_Quaterion.h"
#include "../Reflection/G_Reflection.h"
#include "G_Math.h"
#include <pugixml.hpp>
namespace Gem
{
    using namespace Reflection;

    Quaterion::Quaterion(float aX, float aY, float aZ)
    {
        double tX = Math::degreesToRad(aX);
        double tY = Math::degreesToRad(aY);
        double tZ = Math::degreesToRad(aZ);

        double c1 = cos(tX* 0.5);
        double c2 = cos(tY* 0.5);
        double c3 = cos(tZ* 0.5);
        
        double s1 = sin(tX* 0.5);
        double s2 = sin(tY* 0.5);
        double s3 = sin(tZ* 0.5);

        double c1c2 = c1*c2;
        double s1s2 = s1*s2;

        w = c1c2 * c3 - s1s2 * s3;
        x = s1s2 * c3 + c1c2 * s3;
        y = s1 * c2 * c3  + c1 * s2 * s3;
        z = c1 * s2 * c3  - s1 * c2 * s3;

    }
    Quaterion::Quaterion(const Vector3 & aVec)
    {
        double tX = Math::degreesToRad(aVec.x);
        double tY = Math::degreesToRad(aVec.y);
        double tZ = Math::degreesToRad(aVec.z);

        double c1 = cos(tX* 0.5);
        double c2 = cos(tY* 0.5);
        double c3 = cos(tZ* 0.5);
        
        double s1 = sin(tX* 0.5);
        double s2 = sin(tY* 0.5);
        double s3 = sin(tZ* 0.5);

        double c1c2 = c1*c2;
        double s1s2 = s1*s2;

        w = c1c2 * c3 - s1s2 * s3;
        x = s1s2 * c3 + c1c2 * s3;
        y = s1 * c2 * c3  + c1 * s2 * s3;
        z = c1 * s2 * c3  - s1 * c2 * s3;



    }
    Quaterion::~Quaterion()
    {

    }

    const Vector3 Quaterion::eulerAngles() const
    {
        Vector3 eulerAngle;

        double test = x * y + z * w;
        if(test > 0.499)
        {
            eulerAngle.x = 2.0 * atan2(x,w);
            eulerAngle.y = -PI * 0.5;
            eulerAngle.z = 0.0;
            return Math::radToDegrees(eulerAngle);
        }
        else if(test < -0.499)
        {
            eulerAngle.x = -2.0 * atan2(x,w);
            eulerAngle.y = -PI * 0.5;
            eulerAngle.z = 0.0;
            return Math::radToDegrees(eulerAngle);
        }
        double sqx = x * x;
        double sqy = y * y;
        double sqz = z * z;
        eulerAngle.x = atan2(2.0 * y * w - 2.0 * x * z, 1.0 - 2.0 * sqy - 2.0 * sqz);
        eulerAngle.y = asin(2.0 * test);
        eulerAngle.z = atan2(2.0 * x * w - 2.0 * y * z, 1.0 - 2.0 * sqx - 2.0 * sqz);

        return Math::radToDegrees(eulerAngle);
    }
    pugi::xml_node Quaterion::serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo )
    {
        aNode.append_attribute("X") = x;
        aNode.append_attribute("Y") = y;
        aNode.append_attribute("Z") = z;
        aNode.append_attribute("W") = w;
        return aNode;
    }
    bool Quaterion::deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo)
    {
        int threshHold = 4;
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
            else if(name == "W")
            {
                w = iter->as_float();
                count ++;
            }
        }
        return count >= threshHold;
    }
    Type * Quaterion::getType()
    {
        return Type::create("Quaterion",TypeID::QUATERION,sizeof(Quaterion),Object::getType());
    }

}