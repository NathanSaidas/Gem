#include "G_boolean.h"

namespace Gem
{
    boolean::boolean(boolean & aValue)
    {
        m_Value = aValue;
    }
    boolean::boolean(bool aValue)
    {
        m_Value = aValue;
    }
    boolean::boolean()
    {

    }
    boolean::~boolean()
    {

    }

    Reflection::Type * boolean::getType()
    {
        return Reflection::Type::create("boolean",Reflection::TypeID::BOOLEAN,sizeof(boolean),Object::getType());
    }
}