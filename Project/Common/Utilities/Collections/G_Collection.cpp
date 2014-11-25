#include "G_Collection.h"
#include "../../Reflection/G_Reflection.h"

namespace Gem
{
    namespace Collections
    {
        Pointer<Reflection::Type> Collection::GetType()
        {
            return typeOf("Collections::Collection");
        }
    }
}