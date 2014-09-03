#include "G_boolean.h"
#include "../Reflection/G_Type.h"
#include "../Reflection/G_ReflectionOperators.h"
#include "G_string.h"
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
    void boolean::onInstantiate()
    {

    }
    void boolean::onDestroy()
    {

    }
    Pointer<Reflection::Type> boolean::getType()
    {
        return typeOf("boolean");
    }
}