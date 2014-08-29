#ifndef G_I_STM_LISTENER_H
#define G_I_STM_LISTENER_H

#include "../Base Objects/G_BaseObjects.h"

namespace Gem
{
    //Abstract interface for classes to inherit from.
    class IShortTermMemoryListener
    {
    public:
        IShortTermMemoryListener();
        ~IShortTermMemoryListener();

        virtual void onMemoryClear(Object * aPtr) = 0;
        virtual void onMemoryMove(Object * aOldPtr) = 0;

    };
    typedef IShortTermMemoryListener ISTMListener;
}

#endif