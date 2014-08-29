#include "G_Type.h"
#include "../Memory/G_Memory.h"
#include "G_Types.h"

namespace Gem
{
    namespace Reflection
    {
        Type * Type::create(char * aName, int aTypeID, int aSize, Type * aBaseType)
        {
            Type * type = Memory::instantiate<Type>();
            type->m_Name = aName;
            type->m_TypeID = aTypeID;
            type->m_Size = aSize;
            type->m_Basetype = aBaseType;
            
            return type;
        }
        void Type::freeType(Type * aType)
        {
            if(aType != nullptr)
            {
                Memory::destroy<Type>(aType);
            }
        }

        char * Type::nameOf(Type * aType)
        {
            if(aType != nullptr)
            {
                return aType->m_Name;
            }
            return "INVALID_ID";
        }
        int Type::typeID(Type * aType)
        {
            if(aType != nullptr)
            {
                return aType->m_TypeID;
            }
            return TypeID::INVALID_ID;
        }
        int Type::sizeOf(Type * aType)
        {
            if(aType != nullptr)
            {
                return aType->m_Size;
            }
            return 0;
        }
        bool Type::instanceOf(Type * aBase, Type * aDerived)
        {
            if(aBase == nullptr || aDerived == nullptr)
            {
                return false;
            }

            if(*aBase == *aDerived)
            {
                return true;
            }
            if(aBase->m_TypeID == TypeID::INVALID_ID || aDerived->m_TypeID == TypeID::INVALID_ID)
            {
                return false;
            }
            Type * type = aDerived->baseType();
            while(type != nullptr)
            {
                if(*type == *aBase)
                {
                    return true;
                }
                type = type->baseType();
            }
            return false;
        }

        Type * Type::getType()
        {
            return create("Type",TypeID::TYPE,sizeof(Type),nullptr);
        }
    }
}