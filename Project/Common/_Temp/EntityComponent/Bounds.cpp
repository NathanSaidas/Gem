#include "Bounds.h"

namespace Gem
{
    RDEFINE_CLASS(Bounds,object)

    Bounds::Bounds() : object(),
    m_Min(Vector3::Zero()),
    m_Max(Vector3::Zero())
    {

    }
    Bounds::Bounds(const Vector3 & aMin, const Vector3 & aMax) : object(),
    m_Min(aMin), 
    m_Max(aMax)
    {

    }
    Bounds::~Bounds()
    {

    }

}