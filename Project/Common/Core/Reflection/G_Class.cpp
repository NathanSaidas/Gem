#include "G_Class.h"
#include "G_Type.h"
#include "../Reflection/G_ReflectionOperators.h"
#include "../Primitives/G_string.h"

namespace Gem
{
    namespace Reflection
    {
        //Class Methods
        Pointer<Reflection::Type> Class::getType()
        {
            return typeOf("Class");
        }

    }
}