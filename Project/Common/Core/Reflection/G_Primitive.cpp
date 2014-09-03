#include "G_Primitive.h"
#include "G_Type.h"
#include "../Primitives/G_string.h"
#include "G_ReflectionOperators.h"

namespace Gem
{
    namespace Reflection
    {
        Primitive::Primitive()
        {

        }
        Primitive::~Primitive()
        {

        }
        Pointer<Type> Primitive::getType()
        {
            return typeOf("Primitive");
        }

    }
    
}