#ifndef G_BOOLEAN_H
#define G_BOOLEAN_H

#include "../Reflection/G_Primitive.h"

namespace Gem
{
    class boolean sealed: public Reflection::Primitive
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

        Pointer<Reflection::Type> getType() override;
    protected:
        void onInstantiate();
        void onDestroy();
    private:
        bool m_Value;
    };
}
GEM_CLASS(boolean, Primitive)

#endif