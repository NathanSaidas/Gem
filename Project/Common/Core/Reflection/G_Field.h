#ifndef G_FIELD_H
#define G_FIELD_H

#include "G_Primitive.h"

namespace Gem
{
    namespace Reflection
    {
        //template<class T>
        //class Field : public Primitive
        //{
        //public:
        //    Field(){m_Value = nullptr;}
        //    Field(T * aAddress)
        //    {
        //        m_Value = aAddress;
        //    }
        //    ~Field(){}
        //
        //    void setValue(T aValue)
        //    {
        //        if(m_Value != nullptr)
        //        {
        //            (*m_Value) = aValue;
        //        }
        //    }
        //    T * getValue()
        //    {
        //        return m_Value;
        //    }
        //
        //private:
        //    T * m_Value;
        //};
        class IXmlSerializable;
        class Type;

        class Field : public Primitive
        {
        public:
            Field(){}
            Field(void * aAddress) : m_Address(aAddress)
            {
            }
            ~Field(){}

            inline void * address()
            {
                return m_Address;
            }
            inline Type * typeName()
            {
                return m_TypeName;
            }
        protected:
            void * m_Address;
            Type * m_TypeName;
        };

        
    }
}

#endif