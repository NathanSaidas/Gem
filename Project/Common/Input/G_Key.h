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

        void Update();

        void OnEvent(int aAction);
        bool IsDown();
        bool IsUp();
        ButtonState CurrentState();

        virtual Pointer<Reflection::Type> GetType() override;
        
    protected:
        ButtonState m_CurrentState;
    };
    typedef Key Button;
}
GEM_CLASS(Key,Object)

#endif