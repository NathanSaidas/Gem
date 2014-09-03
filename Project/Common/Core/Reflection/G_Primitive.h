#ifndef G_PRIMITIVE_H
#define G_PRIMITIVE_H


#include "../Memory/G_Pointer.h"
#include "G_ReflectionRegristration.h"

namespace Gem
{
    namespace Reflection
    {
        //Base class of every type in the OnLooker Reflection
        class Type;
        class Primitive
        {
        public:
            Primitive();
            virtual ~Primitive();
            virtual Pointer<Type> getType();
        protected:

            friend class Allocator;
        };
    };
}
GEM_CLASS(Primitive, "")
#endif