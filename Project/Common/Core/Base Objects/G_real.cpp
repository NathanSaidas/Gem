#include "G_real.h"

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

    Reflection::Type * real::getType()
    {
        return Reflection::Type::create("real",Reflection::TypeID::FLOAT,sizeof(real),Object::getType());
    }
}