#ifndef G_INT_H
#define G_INT_H

#include "G_Object.h"

namespace Gem
{

    class integer sealed: public Object
    {
    public:
        integer(integer & aValue);
        integer(int aValue);
        integer();
        ~integer();


        inline int get()
        {
            return m_Value;
        }
        inline void set(int aValue)
        {
            m_Value = aValue;
        }

        inline operator int()
        {
            return m_Value;
        }


        virtual Reflection::Type * getType() override;
    private:
        int m_Value;
    };

    inline int operator+=(integer lhs, const int & rhs)
    {
        return int(lhs + rhs);
    }
    inline int operator-=(integer lhs, const int & rhs)
    {
        return int(lhs - rhs);
    }
    inline int operator*=(integer lhs, const int & rhs)
    {
        return int(lhs * rhs);
    }
    inline int operator/=(integer lhs, const int & rhs)
    {
        return int(lhs / rhs);
    }
}

#endif