#include "Allocator.h"

#include "../Reflection/G_Reflection.h"


namespace Gem
{
    using namespace Reflection;

    Type Allocator::getType()
    {
        return TypeFactory::create("Allocator",TypeID::ALLOCATOR,sizeof(Allocator));
    }
    Type Allocator::baseType()
    {
        return TypeFactory::create("Object",TypeID::OBJECT,sizeof(Object));
    }
    Type * Allocator::instanceOf(int & aCount)
    {
        int prevSize = 0;
        Type * prevTypes = Object::instanceOf(prevSize);
        aCount = prevSize + 1;
        char ** names = new char * [1];
        int * typeIDs = new int[1];
        int * sizes = new int[1];

        names[0] = "Object";
        typeIDs[0] = TypeID::OBJECT;
        sizes[0] = sizeof(Object);

        Type * types = TypeFactory::create(names,typeIDs,sizes,aCount,prevTypes,prevSize);

        delete[]names;
        delete[]typeIDs;
        delete[]sizes;
        return types; 
    }
}