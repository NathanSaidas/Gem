#include "G_ReflectionOperators.h"
#include "../Reflection/G_Type.h"
#include "../Primitives/G_integer.h"
#include "../Primitives/G_string.h"
#include "G_ReflectionRegristration.h"
#include "G_ReflectionRuntime.h"

namespace Gem
{
    Pointer<Reflection::Type> typeOf(const char * aTypename)
    {
        return typeOf(string(aTypename));
    }
    Pointer<Reflection::Type> typeOf(string & aTypename)
    {
        Reflection::Type * type = Reflection::Runtime::instance()->getType(aTypename);
        return Pointer<Reflection::Type>(type);
    }
    Pointer<Reflection::Type> typeOf(integer & aID)
    {
        Reflection::Type * type = Reflection::Runtime::instance()->getType(aID);
        return Pointer<Reflection::Type>(type);
    }
    bool instanceOf(Pointer<Reflection::Type> & aBaseType, Pointer<Reflection::Type> & aDerived)
    {
        if (aBaseType.isAlive() == false || aDerived.isAlive() == false)
        {
            return false;
        }

        Reflection::Type * baseType = aBaseType.ref();
        Reflection::Type * derivedType = aDerived.ref();

        if (*baseType == *derivedType)
        {
            return true;
        }
        baseType = baseType->baseClass();

        while (baseType != nullptr)
        {
            if (*baseType == *derivedType)
            {
                return true;
            }
            baseType = baseType->baseClass();
        }
        return false;
    }
    bool instanceOf(Reflection::Primitive * aBaseType, Reflection::Primitive * aDerivedType)
    {
        if (aBaseType == nullptr || aDerivedType == nullptr)
        {
            return false;
        }
        Pointer<Reflection::Type> baseType = aBaseType->getType();
        Pointer<Reflection::Type> derivedType = aDerivedType->getType();

        return instanceOf(baseType, derivedType);
    }

    namespace Reflection
    {

        
        Primitive * Hidden::createPrimitive(Primitive * aAddress, string & aTypename)
        {
            Type * type = Runtime::instance()->getType(aTypename);
            if (type == nullptr)
            {
                return nullptr;
            }
            return type->invokeCreate(aAddress);
        }
        Primitive * Hidden::createPrimitive(Primitive * aAddress, integer & aID)
        {
            Type * type = Runtime::instance()->getType(aID);
            if (type == nullptr)
            {
                return nullptr;
            }
            return type->invokeCreate(aAddress);
        }
        Primitive * Hidden::destroyPrimitive(Primitive * aAddress, string & aTypename)
        {
            Type * type = Runtime::instance()->getType(aTypename);
            if (type == nullptr)
            {
                return nullptr;
            }
            return type->invokeDestroy(aAddress);
        }
        Primitive * Hidden::destroyPrimitive(Primitive * aAddress, integer & aID)
        {
            Type * type = Runtime::instance()->getType(aID);
            if (type == nullptr)
            {
                return nullptr;
            }
            return type->invokeDestroy(aAddress);
        }
    }
}