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

            inline static Type * Int()
            {
                return create("Int",TypeID::INT,sizeof(int),nullptr);
            }
            inline static Type * Float()
            {
                return create("Float",TypeID::FLOAT,sizeof(float),nullptr);
            }
            inline static Type * CharPtr()
            {
                return create("CharPtr",TypeID::CHAR_PTR,sizeof(char*),nullptr);
            }
            inline static Type * Boolean()
            {
                return create("Boolean",TypeID::BOOLEAN,sizeof(bool),nullptr);
            }

        protected:
            Type():Primitive(){}
            
            int m_TypeID;
            int m_Size;
            Type * m_Basetype;

            

            friend Type;
            friend PoolAllocator;
        };
    }
}

#endif