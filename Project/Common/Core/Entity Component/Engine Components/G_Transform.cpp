#include "G_Transform.h"
#include <pugixml.hpp>
#include "../../Reflection/G_Reflection.h"
namespace Gem
{
    using namespace Reflection;

    Transform::Transform()
    {
        m_Position = Vector3::zero();
        m_Rotation = Quaternion::identity();
        m_Scale = Vector3::zero();
    }
    Transform::~Transform()
    {

    }

   
    Pointer<Reflection::Type> Transform::getType()
    {
        return typeOf("Transform");
    }
        
}
