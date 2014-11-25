#ifndef G_INT_H
#define G_INT_H

#include "../Reflection/G_Primitive.h"

namespace Gem
{

    class integer sealed: public Reflection::Primitive
    {
    public:
        integer(const integer & aValue);
        integer(int aValue);
        integer();
        ~integer();


        inline int Get()
        {
            return m_Value;
        }
        inline void Set(int aValue)
        {
            m_Value = aValue;
        }

        inline operator int()
        {
            return m_Value;
        }

		inline integer operator++()
		{
			return m_Value++;
		}
		inline integer operator--()
		{
			return m_Value--;
		}


        Pointer<Reflection::Type> GetType() override;
    protected:
        void onInstantiate();
        void onDestroy();
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
GEM_CLASS(integer, Primitive)
#endif