#include "G_TypeFactory.h"
#include "../Memory/G_Memory.h"

namespace Gem
{
    namespace Reflection
    {

        //Type * Type::create(char * aName, int aTypeID, int aSize, Type * aBaseType)
        //{
        //    Type * type = Memory::instantiate<Type>();
        //    type->name = aName;
        //    type->typeID = aTypeID;
        //    type->size = aSize;
        //    type->m_Basetype = aBaseType;
        //    return type;
        //}
        //static Type * freeType(Type * type)
        //{
        //    return Memory::destroy<Type>(type);
        //}
        //
        ////Type factory operators
        //char * Type::nameOf(Enum & aEnum)
        //{
        //    return aEnum.name;
        //}
        //char * Type::nameOf(Type * aType)
        //{
        //    return aType->name;
        //}
        //int Type::typeID(Type * aType)
        //{
        //    return aType->typeID;
        //}
        //int Type::sizeOf(Type * aType)
        //{
        //    return aType->size;
        //}

    }
}