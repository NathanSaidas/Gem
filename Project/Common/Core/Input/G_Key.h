#ifndef G_KEY_H
#define G_KEY_H

#include "../Primitives/G_PrimitiveObjects.h"
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

        virtual Pointer<Reflection::Type> getType() override;
        
    protected:
        ButtonState m_CurrentState;
    };
    typedef Key Button;
}
GEM_CLASS(Key,Object)

#endif