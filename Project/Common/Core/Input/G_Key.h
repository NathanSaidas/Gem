#ifndef G_KEY_H
#define G_KEY_H

#include "../Base Objects/G_Object.h"
#include "G_KeyDef.h"

namespace Gem
{
    class Key : public Object
    {
    public:
        Key();
        virtual ~Key();

        void update();

        void onEvent(int aAction);
        bool isDown();
        bool isUp();
        ButtonState currentState();

        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);
        
    protected:
        ButtonState m_CurrentState;
    };

    

    typedef Key Button;
}

#endif