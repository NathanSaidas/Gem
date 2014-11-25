#include "G_Type.h"
#include "G_ReflectionRegristration.h"
#include "G_ReflectionOperators.h"
#include "../Memory/G_MemoryManager.h"

namespace Gem
{


    namespace Reflection
    {
        Type::Type() : Primitive()
        {
            m_Name = string::Empty();
            m_Alignment = 0;
            m_Size = 0;
            m_TypeID = 0;
            m_CreateFunc = nullptr;
            m_DestroyFunc = nullptr;
            m_BaseType = nullptr;
            //m_CreateArrayFunc = nullptr;
            //m_DestroyArrayFunc = nullptr;
        }
        Type::~Type()
        {
            if (m_BaseType != nullptr)
            {
                m_BaseType = Memory::Destroy<Type>(m_BaseType);
            }
        }
        Type * Type::copy(Type * aType)
        {
            if (aType == nullptr)
            {
                return nullptr;
            }
            Type * original = Memory::Instantiate<Type>();
            Type * iter = original;
            while (true)
            {
                iter->m_Alignment = aType->m_Alignment;
                iter->m_BaseType = aType->m_BaseType;
                iter->m_Name = aType->m_Name;
                iter->m_Size = aType->m_Size;
                iter->m_TypeID = aType->m_TypeID;
                iter->m_CreateFunc = aType->m_CreateFunc;
                iter->m_DestroyFunc = aType->m_DestroyFunc;
                //iter->m_CreateArrayFunc = aType->m_CreateArrayFunc;
                //iter->m_DestroyArrayFunc = aType->m_DestroyArrayFunc;
                //if has base type
                if (aType->m_BaseType != nullptr)
                {
                    iter->m_BaseType = Memory::Instantiate<Type>();
                    iter = iter->m_BaseType;
                    aType = aType->m_BaseType;
                }
                else
                {
                    aType->m_BaseType = nullptr;
                    break;
                }
            }

            //Count how many base classes there are
            /*int count = 1;
            Type * baseType = aType->baseClass();
            while (baseType != nullptr)
            {
                count++;
                baseType = baseType->baseClass();
            }

            //Allocate memory for x amount of base classes, there will always be at least one for the original
            std::vector<Type*> types;
            for (int i = 0; i < count; i++)
            {
                types.push_back(Memory::instantiate<Type>());
            }

            //Create the first type
            Type * type = types[0];
            type->m_Alignment = aType->m_Alignment;
            type->m_Size = aType->m_Size;
            type->m_TypeID = aType->m_TypeID;
            type->m_CreateFunc = aType->m_CreateFunc;
            type->m_DestroyFunc = aType->m_DestroyFunc;
            type->m_Name = aType->m_Name;
            //If the size is greater than 1 there was allocation for a base class
            //Set the baseclass pointer
            //Move this type to baseclass and aType to baseclass of aType
            if (types.size() > 1)
            {
                type->m_BaseType = types[1];
                type = type->m_BaseType;
                aType = aType->m_BaseType;
            }
            types.erase(types.begin() + 0);

            //Used = 1

            //Using new baseclass pointers do this loop until no more baseclasses
            do
            {
                type->m_Alignment = aType->m_Alignment;
                type->m_Size = aType->m_Size;
                type->m_TypeID = aType->m_TypeID;
                type->m_CreateFunc = aType->m_CreateFunc;
                type->m_DestroyFunc = aType->m_DestroyFunc;
                type->m_Name = aType->m_Name;
                if (aType->m_BaseType != nullptr)
                {
                    type->m_BaseType = types[1];
                    type = type->m_BaseType;
                    aType = aType->m_BaseType;
                }


            } while (aType->m_BaseType != nullptr);*/

            

            
            return original;
        }
        string  Type::name() const
        {
            return m_Name;
        }
        integer Type::typeID() const
        {
            return m_TypeID;
        }
        integer Type::size() const
        {
            return m_Size;
        }
        integer Type::alignment() const
        {
            return m_Alignment;
        }
        Type *  Type::baseClass() const
        {
            return m_BaseType;
        }
        Pointer<Type> Type::GetType() 
        {
            return typeOf("Type");
        }
        Primitive * Type::invokeCreate(Primitive* aAddress)
        {
            if (m_CreateFunc == nullptr)
            {
                return nullptr;
            }
            return m_CreateFunc(aAddress);
        }
        Primitive * Type::invokeDestroy(Primitive* aAddress)
        {
            if (m_DestroyFunc == nullptr)
            {
                return aAddress;
            }
            return m_DestroyFunc(aAddress);
        }
        //Primitive ** Type::invokeCreate(Primitive * aAddress, unsigned int aLength)
        //{
        //    if (m_CreateArrayFunc == nullptr)
        //    {
        //        return nullptr;
        //    }
        //    return m_CreateArrayFunc(aAddress, aLength);
        //}
        //Primitive ** Type::invokeDestroy(Primitive * aAddress, unsigned int aLength)
        //{
        //    if (m_DestroyArrayFunc == nullptr)
        //    {
        //        return (Primitive**)aAddress;
        //    }
        //    return m_DestroyArrayFunc(aAddress, aLength);
        //}
    }
}