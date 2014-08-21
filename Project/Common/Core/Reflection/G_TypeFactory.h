#ifndef G_TYPE_FACTORY_H
#define G_TYPE_FACTORY_H

#include "G_Type.h"
#include "G_Enum.h"

namespace Gem
{
    namespace Reflection
    {
        //Type Accessor
        class TypeFactory
        {
        public:
            //Create methods
            static Type create(char * aName, int aTypeID, int aSize);
            static Type * create(char ** aName, int * aTypeID, int * aSize, int aCount);
            static Type * create(char ** aName, int * aTypeID, int * aSize, int aCount, Type * aPreviousTypes, int aPreviousCount);
            static Type * create(Type aType, int aCount, Type * aPreviousTypes, int aPreviousCount);
            //Operators
            static char * nameOf(Enum & aEnum);
            static char * nameOf(Type aType);
            static int typeID(Type aType);
            static int sizeOf(Type aType);
        };
    }
}

#endif