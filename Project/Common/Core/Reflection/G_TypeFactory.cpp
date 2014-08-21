#include "G_TypeFactory.h"

namespace Gem
{
    namespace Reflection
    {
        Type TypeFactory::create(char * aName, int aTypeID, int aSize)
        {
            return Type(aName,aTypeID,aSize);
        }
        Type * TypeFactory::create(char ** aName, int * aTypeID, int * aSize, int aCount)
        {
            if(aCount <= 0)
            {
                return nullptr;
            }
            Type * types = new Type[aCount];

            for(int i = 0; i < aCount; i++)
            {
                types[i] = Type(aName[i],aTypeID[i],aSize[i]);
            }
            return types;
        }
        Type * TypeFactory::create(char ** aName, int * aTypeID, int * aSize, int aCount, Type * aPreviousTypes, int aPreviousCount)
        {
            if(aCount <= 0)
            {
                return nullptr;
            }
            Type * types = new Type[aCount];
            for(int i = 0; i < aCount; i++)
            {
                if(i < aPreviousCount)
                {
                    types[i] = aPreviousTypes[i];
                    continue;
                }
                int offsetIndex = i - aPreviousCount;
                types[i] = Type(aName[offsetIndex],aTypeID[offsetIndex],aSize[offsetIndex]);
            }
            delete aPreviousTypes;
            return types;
        }
        Type * TypeFactory::create(Type aType, int aCount, Type * aPreviousTypes, int aPreviousCount)
        {
            if(aCount <= 0)
            {
                return nullptr;
            }
            Type * types = new Type[aCount];
            for(int i = 0; i < aCount; i++)
            {
                if(i < aPreviousCount)
                {
                    types[i] = aPreviousTypes[i];
                    continue;
                }
                int offsetInedx = i - aPreviousCount;
                types[i] = aType;
            }
            delete aPreviousTypes;
            return types;
        }
        //Type factory operators
        char * TypeFactory::nameOf(Enum & aEnum)
        {
            return aEnum.name;
        }
        char * TypeFactory::nameOf(Type aType)
        {
            return aType.name;
        }
        int TypeFactory::typeID(Type aType)
        {
            return aType.typeID;
        }
        int TypeFactory::sizeOf(Type aType)
        {
            return aType.size;
        }

    }
}