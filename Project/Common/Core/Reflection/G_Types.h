#ifndef G_TYPES
#define G_TYPES

namespace Gem
{

    namespace Reflection
    {
        namespace TypeID
        {
            //ADD TYPES TO LIST
            enum TypeID
            {
                INVALID_ID = 0,
                NONE,
                CLASS,
                INTERFACE,
                OBJECT,
                STRING,
                ARRAY,
                ALLOCATOR,
                POOL_ALLOCATOR,
                MEMORY_MANAGER,
                WINDOW,
                WINDOW_MANAGER,
                WINDOW_HOOK,
                APPLICATION,
                INPUT,
                INPUT_AXIS,
                KEY,
                UNIQUE_NUMBER_GENERATOR,
                VECTOR2,
                VECTOR3,
                VECTOR4,

            };
        }
    }

}
#endif
