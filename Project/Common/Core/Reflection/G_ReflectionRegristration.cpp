#include "G_ReflectionRegristration.h"

namespace Gem
{
    namespace Reflection
    {
        /*
        Primitive * createPrimitive(Primitive * aAddress, std::string & aTypeName)
        {
            TypeList & list = getTypeRegistry();
            TypeInfo * type = list.getType(aTypeName.c_str());

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
        Primitive * destroyPrimitive(Primitive * aAddress, std::string & aTypeName)
        {
            TypeList & list = getTypeRegistry();
            TypeInfo * type = list.getType(aTypeName.c_str());

            if (type == nullptr)
            {
                return aAddress;
            }
            CreatePrimitiveFunc destroyFunc = type->destroyFunc();
            if (destroyFunc == nullptr)
            {
                return aAddress;
            }
            return destroyFunc(aAddress);
        }
        */
    }
}