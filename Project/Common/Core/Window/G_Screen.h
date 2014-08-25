#ifndef G_SCREEN_H
#define G_SCREEN_H

#include "../Base Objects/G_Object.h"

namespace Gem
{

    class Screen : public Object
    {

        virtual Reflection::Type * getType();
    };

}

#endif