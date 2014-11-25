#include "G_integer.h"
#include "G_string.h"
#include "../Reflection/G_Type.h"
#include "../Reflection/G_ReflectionOperators.h"

namespace Gem
{
    integer::integer(const integer & aValue)
    {
        m_Value = aValue.m_Value;
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
    void integer::onInstantiate()
    {

    }
    void integer::onDestroy()
    {

    }
    Pointer<Reflection::Type> integer::GetType()
    {
        return typeOf("integer");
    }
}