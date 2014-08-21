#include "G_Object.h"
#include "../Reflection/G_Types.h"
#include "../Reflection/G_TypeFactory.h"
namespace Gem
{

    using namespace Reflection;

    Object::Object()
    {

    }
    Object::~Object()
    {

    }
    Type Object::getType()
    {
        return TypeFactory::create("Object",TypeID::OBJECT,sizeof(Object));
    }
    Type Object::baseType()
    {
        return TypeFactory::create("Class",TypeID::CLASS,sizeof(Class));
    }
    Type * Object::instanceOf(int & aCount)
    {
        aCount = 1;
        char ** names = new char * [aCount];
        int * typeIDs = new int[aCount];
        int * sizes = new int[aCount];

        names[0] = "Class";
        typeIDs[0] = TypeID::CLASS;
        sizes[0] = sizeof(Class);

        Type * types = TypeFactory::create(names,typeIDs,sizes,aCount);

        delete[]names;
        delete[]typeIDs;
        delete[]sizes;
        return types; 
    }

}