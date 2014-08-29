#include "G_integer.h"

namespace Gem
{
    integer::integer(integer & aValue)
    {
        m_Value = aValue;
    }
    integer::integer(int aValue)
    {
        m_Value = aValue;
    }
    integer::integer()
    {

    }
    integer::~integer()
    {

    }

    Reflection::Type * integer::getType()
    {
        return Reflection::Type::create("integer",Reflection::TypeID::INT,sizeof(integer),Object::getType());
    }
}