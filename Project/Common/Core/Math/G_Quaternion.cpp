#include "G_Quaternion.h"
#include "../Reflection/G_Reflection.h"
#include "G_Math.h"
namespace Gem
{
    using namespace Reflection;


    Quaternion::Quaternion(float aX, float aY, float aZ)
    {
        double radX = Math::degreesToRad(aX);
        double radY = Math::degreesToRad(aY);
        double radZ = Math::degreesToRad(aZ);

        double sZ = sin(radZ* 0.5);
        double cZ = cos(radZ* 0.5);

        double cY = cos(radY* 0.5);
        double sY = sin(radY* 0.5);

        double cX = cos(radX* 0.5);
        double sX = sin(radX* 0.5);

        double c1c2 = cX*cY;
        double s1s2 = sX*sY;

        //z = sX * sY * cZ - cX * cY * sZ;
        w = cX * cY * cZ + sX * sY * sZ;
        x = sX * cY * cZ - cX * sY * sZ;
        y = cX * sY * cZ + sX * cY * sZ;
        z = cX * cY * sZ - sX * sY * cZ;
    }
    Quaternion::Quaternion(const Vector3 & aVec)
    {
        double radX = Math::degreesToRad(aVec.x);
        double radY = Math::degreesToRad(aVec.y);
        double radZ = Math::degreesToRad(aVec.z);

        double sZ = sin(radZ* 0.5);
        double cZ = cos(radZ* 0.5);

        double cY = cos(radY* 0.5);
        double sY = sin(radY* 0.5);

        double cX = cos(radX* 0.5);
        double sX = sin(radX* 0.5);

        double c1c2 = cX*cY;
        double s1s2 = sX*sY;

        z = sX * sY * cZ + cX * cY * sZ;
        y = sX * cY * cZ + cX * sY * sZ;
        x = cX * sY * cZ - sX * cY * sZ;
        w = cX * cY * cZ - sX * sY * sZ;

    }
    Quaternion::~Quaternion()
    {

    }

    const Vector3 Quaternion::eulerAngles() const
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
    
    Pointer<Reflection::Type> Quaternion::getType()
    {
        return typeOf("Quaternion");
    }

}