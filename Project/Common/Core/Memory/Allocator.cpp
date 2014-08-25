#include "Allocator.h"

#include "../Reflection/G_Reflection.h"


namespace Gem
{
    using namespace Reflection;

    Type * Allocator::getType()
    {
        return Type::create("Allocator",TypeID::ALLOCATOR,sizeof(Allocator),Object::getType());
    }

}