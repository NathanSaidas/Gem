#include "G_ReflectionRegristration.h"

namespace Gem
{
    namespace Reflection
    {
        /*
        Primitive * createPrimitive(Primitive * aAddress, std::string & aTypeName)
        {
            TypeList & list = GetTypeRegistry();
            TypeInfo * type = list.GetType(aTypeName.c_str());

            if (type == nullptr)
            {
                return nullptr;
            }
            CreatePrimitiveFunc createFunc = type->createFunc();
            if (createFunc == nullptr)
            {
                return nullptr;
            }
            return createFunc(aAddress);
        }
        Primitive * DestroyPrimitive(Primitive * aAddress, std::string & aTypeName)
        {
            TypeList & list = GetTypeRegistry();
            TypeInfo * type = list.GetType(aTypeName.c_str());

            if (type == nullptr)
            {
                return aAddress;
            }
            CreatePrimitiveFunc DestroyFunc = type->DestroyFunc();
            if (DestroyFunc == nullptr)
            {
                return aAddress;
            }
            return DestroyFunc(aAddress);
        }
        */
    }
}