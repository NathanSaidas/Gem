#ifndef G_OPERATORS_H
#define G_OPERATORS_H

#include "G_Type.h"
#include "G_Types.h"
#include "G_Class.h"
#include "G_Enum.h"
#include "../Memory/PoolAllocator.h"
namespace Gem
{
    namespace Reflection
    {
        //Reflection Operators
        inline char * nameOf(Type * aType)
        {
            return Type::nameOf(aType);
        }
        //Frees the type it allocated
        inline char * nameOf(Class & aClass)
        {
            char * result = "";
            Type * type = aClass.getType();
            if(type != nullptr)
            {
                result = Type::nameOf(type);
                Type::freeType(type);
            }
            return result;
        }
        //Frees the type it allocated
        inline char * nameOf(Class * aClass)
        {
            char * result = "";
            if(aClass != nullptr)
            {
                Type * type = aClass->getType();
                if(type != nullptr)
                {
                    result = Type::nameOf(type);
                    Type::freeType(type);
                }
            }
            return result;
        }
        //inline char * nameOf(Enum aEnum)
        //{
        //    return Type::nameOf(aEnum);
        //}
        //inline char * nameOf(Enum * aEnum)
        //{
        //    if(aEnum == nullptr)
        //    {
        //        return "";
        //    }
        //    return Type::nameOf((*aEnum));
        //}
        inline int typeID(Type * aType)
        {
            return Type::typeID(aType);
        }
        inline int typeID(Class & aClass)
        {
            int result = TypeID::INVALID_ID;

            Type * type = aClass.getType();
            if(type != nullptr)
            {
                result = Type::typeID(type);
                Type::freeType(type);
            }

            return result;
        }
        inline int typeID(Class * aClass)
        {
            int result = TypeID::INVALID_ID;
            if(aClass != nullptr)
            {
                Type * type = aClass->getType();
                if(type != nullptr)
                {
                    result = Type::typeID(type);
                    Type::freeType(type);
                }
            }
            return result;
        }
        
        template<class T>
        Type * typeOf()
        {
            T type;
            return type.getType();
        }

        inline int sizeOf(Type * aType)
        {
            return Type::sizeOf(aType);
        }
        inline int sizeOf(Class & aClass)
        {
            int result = 0;
            Type * type = aClass.getType();
            if(type != nullptr)
            {
                result = Type::sizeOf(type);
                Type::freeType(type);
            }
            return result;
        }
        inline int sizeOf(Class * aClass)
        {
            int result = 0;
            if(aClass != nullptr)
            {
                Type * type = aClass->getType();
                if(type != nullptr)
                {
                    result = Type::sizeOf(type);
                    Type::freeType(type);
                }
            }
            return result;
        }

        inline bool instanceOf(Type * aBase, Type * aDerived)
        {
            return Type::instanceOf(aBase,aDerived);
        }
        
        inline bool instanceOf(Class & aBase, Class & aDerived)
        {
            bool result = false;
            Type * base = aBase.getType();
            Type * derived = aDerived.getType();
            
            result = Type::instanceOf(base,derived);

            if(base != nullptr)
            {
                Type::freeType(base);
            }
            if(derived != nullptr)
            {
                Type::freeType(derived);
            }

            return result;
        }
        inline bool instanceOf(Class * aBase, Class * aDerived)
        {

            if(aBase == nullptr || aDerived == nullptr)
            {
                return false;
            }

            bool result = false;
            Type * base = aBase->getType();
            Type * derived = aDerived->getType();
            
            result = Type::instanceOf(base,derived);

            if(base != nullptr)
            {
                Type::freeType(base);
            }
            if(derived != nullptr)
            {
                Type::freeType(derived);
            }

            return result;
        }
    }
}

#endif