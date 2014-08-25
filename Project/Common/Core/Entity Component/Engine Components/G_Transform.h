#ifndef G_TRANSFORM_H
#define G_TRANSFORM_H

#include "../G_Component.h"
#include "../../Math/G_Math.h"

namespace Gem
{
    class Transform : public Component
    {
    public:

        Transform();
        ~Transform();


        virtual pugi::xml_node serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo = false);
        virtual bool deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo = false);
        virtual Reflection::Type * getType();

    private:
        Vector3 m_Position;
        Quaterion m_Rotation;
        Vector3 m_Scale;
    };
}


#endif