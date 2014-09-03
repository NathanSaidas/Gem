#include "G_Transform.h"
#include <pugixml.hpp>
#include "../../Reflection/G_Reflection.h"
namespace Gem
{
    using namespace Reflection;

    Transform::Transform()
    {
        m_Position = Vector3::zero();
        m_Rotation = Quaterion::identity();
        m_Scale = Vector3::zero();
    }
    Transform::~Transform()
    {

    }

    pugi::xml_node Transform::serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo)
    {
        aNode = Component::serialize(aNode,aIncludeTypeInfo);

        m_Position.serialize(aNode.append_child("Position"));
        m_Rotation.serialize(aNode.append_child("Rotation"));
        m_Scale.serialize(aNode.append_child("Scale"));

        return aNode;
    }
    bool Transform::deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo )
    {
        Component::deserialize(aNode,aIncludeTypeInfo);

        std::string name = "";
        for(pugi::xml_node_iterator iter = aNode.begin(); iter != aNode.end(); ++iter)
        {
            name = iter->name();
            pugi::xml_node node(iter->internal_object());
            if(name == "Position")
            {
                m_Position.deserialize(node);
            }
            else if(name == "Rotation")
            {
                m_Rotation.deserialize(node);
            }
            else if(name == "Scale")
            {
                m_Scale.deserialize(node);
            }
        }

        return true;
    }
    Pointer<Reflection::Type> Transform::getType()
    {
        return typeOf("Transform");
    }
        
}
