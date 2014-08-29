#ifndef G_REAL_H
#define G_REAL_H

#include "G_Object.h"

namespace Gem
{
    class real sealed: public Object
    {
    public:
        real(real & aValue);
        real(float aValue);
        real();
        ~real();


        inline float get()
        {
            return m_Value;
        }
        inline void set(float aValue)
        {
            m_Value = aValue;
        }

        inline operator float()
        {
            return m_Value;
        }


        virtual Reflection::Type * getType() override;
    private:
        float m_Value;
    };

    inline int operator+=(real lhs, const int & rhs)
    {
        return int(lhs + rhs);
    }
    inline int operator-=(real lhs, const int & rhs)
    {
        return int(lhs - rhs);
    }
    inline int operator*=(real lhs, const int & rhs)
    {
        return int(lhs * rhs);
    }
    inline int operator/=(real lhs, const int & rhs)
    {
        return int(lhs / rhs);
    }
}

#endif