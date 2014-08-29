#ifndef G_TYPE_H
#define G_TYPE_H

#include "G_Primitive.h"
#include "G_Types.h"

//Base class of every data type. Enum / Class
namespace Gem
{
    class PoolAllocator;
    namespace Reflection
    {

        class Type;
        class Type : public Primitive
        {
        public:
            
            ~Type()
            {
                Type::freeType(m_Basetype);
            }
            bool operator ==(const Type & aRhs)
            {
                return aRhs.m_TypeID == m_TypeID;
            }
            inline Type * baseType()
            {
                return m_Basetype;
            }

            //Create methods
            static Type * create(char * aName, int aTypeID, int aSize, Type * aBaseType);
            static void freeType(Type * aType);

            //Operators
            static char * nameOf(Type * aType);
            static int typeID(Type * aType);
            static int sizeOf(Type * aType);
            static bool instanceOf(Type * aBase, Type * aDerived);



            Type * getType();
            
        protected:
            Type():Primitive(){m_Name = "INVALID", m_TypeID == TypeID::INVALID_ID; m_Size = 0; m_Basetype = nullptr;}
            
            int m_TypeID;
            int m_Size;
            Type * m_Basetype;

            

            friend Type;
            friend PoolAllocator;
        };
    }
}

#endif