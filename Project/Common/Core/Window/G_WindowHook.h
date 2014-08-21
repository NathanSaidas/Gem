#ifndef G_WINDOW_HOOK_H
#define G_WINDOW_HOOK_H

#include "../Base Objects/G_Object.h"

namespace Gem
{
    class Window;
    class WindowHook : public Object
    {
    public:
        WindowHook();
        virtual ~WindowHook();
        //Gets invoked by the window
        virtual void update();

        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);

    protected:
        bool isFocused();

        virtual void onAttachToWindow(int aHandle);
        virtual void onDetachFromWindow(int aHandle);
    private:
        Window * m_Window;
        friend class WindowManager;

        float m_Timer;
        float m_CurrentTime;
    };
}

#endif