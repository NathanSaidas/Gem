#ifndef G_WINDOW_HOOK_H
#define G_WINDOW_HOOK_H

#include "../Base Objects/G_Object.h"
#include "../Memory/G_ISTMListener.h"

#include "../Entity Component/G_GameObject.h"
#include "../Utilities/G_Array.h"

#include "../Math/G_Math.h"


namespace Gem
{
    class Window;
    class WindowHook : public Object, ISTMListener
    {
    public:
        WindowHook();
        virtual ~WindowHook();
        //Gets invoked by the window
        virtual void update();

        virtual Reflection::Type * getType();
        virtual void onMemoryClear(Object * aPtr);
        virtual void onMemoryMove(Object * aOldPtr);

    protected:
        bool isFocused();

        virtual void onAttachToWindow(int aHandle);
        virtual void onDetachFromWindow(int aHandle);
    private:
        Window * m_Window;
        friend class WindowManager;

        float m_Timer;
        float m_CurrentTime;

        int m_CoolField;
        Vector3 m_Position;

        Array<GameObject*,GameObject> m_GameObjects;
    };
}

#endif