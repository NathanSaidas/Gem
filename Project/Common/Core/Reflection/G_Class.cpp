#include "G_Class.h"
#include "G_Types.h"
#include "G_TypeFactory.h"

namespace Gem
{
    namespace Reflection
    {
        //Class Methods
        Type * Class::getType()
        {
            return Type::create("Class",TypeID::CLASS,sizeof(Class),nullptr);
        }

    }
}