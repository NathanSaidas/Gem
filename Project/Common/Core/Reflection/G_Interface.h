#ifndef G_INTERFACE_H
#define G_INTERFACE_H

#include "G_Type.h"

namespace Gem
{
    namespace Reflection
    {
        class Interface
        {
            public:
                virtual Type getType();
        };
        
    }
}

#endif