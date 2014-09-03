#ifndef G_REAL_H
#define G_REAL_H

#include "../Reflection/G_Primitive.h"

namespace Gem
{
    class real sealed: public Reflection::Primitive
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
        Pointer<Reflection::Type> getType() override;

    protected:
        void onInstantiate();
        void onDestroy();
        
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
GEM_CLASS(real, Primitive)
#endif