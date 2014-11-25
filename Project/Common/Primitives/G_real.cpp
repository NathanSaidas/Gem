#include "G_real.h"
#include "../Reflection/G_Type.h"
#include "../Reflection/G_ReflectionOperators.h"
#include "G_string.h"
namespace Gem
{
    real::real(real & aValue)
    {
        m_Value = aValue;
    }
    real::real(float aValue)
    {
        m_Value = aValue;
    }
    real::real()
    {

    }
    real::~real()
    {

    }
    void real::onInstantiate()
    {

    }
    void real::onDestroy()
    {

    }
    Pointer<Reflection::Type> real::GetType()
    {
        return typeOf("real");
    }
}