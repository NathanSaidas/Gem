#ifndef G_KEY_H
#define G_KEY_H

#include "G_KeyDef.h"
#include "../G_Object.h"

namespace Gem
{
    class Key : public object
    {
		G_CLASS_DEF(Key)
    public:
        Key();
        virtual ~Key();

        void Update();

        void OnEvent(int aAction);
        bool IsDown();
        bool IsUp();
        ButtonState CurrentState();
        
    protected:
        ButtonState m_CurrentState;
    };
    typedef Key Button;
}

#endif