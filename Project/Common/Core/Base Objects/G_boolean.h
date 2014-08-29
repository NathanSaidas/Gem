#ifndef G_BOOLEAN_H
#define G_BOOLEAN_H

#include "G_Object.h"

namespace Gem
{
    class boolean sealed: public Object
    {
        public:
        boolean(boolean & aValue);
        boolean(bool aValue);
        boolean();
        ~boolean();


        inline bool get()
        {
            return m_Value;
        }
        inline void set(bool aValue)
        {
            m_Value = aValue;
        }

        inline operator bool()
        {
            return m_Value;
        }

        virtual Reflection::Type * getType() override;
    private:
        bool m_Value;
    };
}

#endif