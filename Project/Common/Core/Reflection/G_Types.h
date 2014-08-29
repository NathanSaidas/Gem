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
                NONE = -2,
                INVALID_ID = -1,
                CLASS = 0,
                INTERFACE,
                OBJECT,
                POINTER,
                TYPE,
                //Primitive Types
                INT,
                FLOAT,
                CHAR_PTR,
                BOOLEAN,
                //
                STRING,
                ARRAY,
                ALLOCATOR,
                POOL_ALLOCATOR,
                MEMORY_MANAGER,
                SHORT_TERM_MEMORY_BLOCK,
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
                MATRIX_4X4,
                QUATERION,

                //ENTITY COMPONENT
                GAME_OBJECT,
                //COMPONENTS
                COMPONENT,
                TRANSFORM,
                DEBUG_COMPONENT,

                //END COMPONENTS
                GAME_OBJECT_MANAGER,
                SCENE,
                SCENE_MANAGER,

                IO,
                STREAM_PROPERTY,
                STREAM_OBJECT,
                STREAM_FILE,
                STREAM_DOC

            };
        }
    }

}
#endif
