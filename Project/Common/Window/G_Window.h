#ifndef G_WINDOW_H
#define G_WINDOW_H

#include "../G_Object.h"

//glfw window struct def
struct GLFWwindow;

//This class will serve as the base class for all future windows

namespace Gem
{
    class WindowHook;

    class Window : public object
    {
		G_CLASS_DEF(Window)
    public:
        Window();
        virtual ~Window();

        int Handle();
        int Width();
        int Height();
        char * Title();
    protected:
        //These are events that will be sent from a window manager
        virtual void KeyEvent(int aKey, int aScanCode, int aAction, int aMods);
        virtual void MouseButtonEvent(int aButton, int aAction, int aMods);
        virtual void MouseMoveEvent(float aX, float aY);
        virtual void MouseScroll(float aX, float aY);

		virtual void OnWindowResize(float aWidth, float aHeight);

        virtual void Update();
        virtual void OnCreate();
        virtual void OnDestroy();

    private:
        WindowHook * m_WindowHook;
        GLFWwindow * m_Handle;
        int m_Width;
        int m_Height;
        int m_HandleID;
        char * m_Title;

        

        void CreateWindow();
        void DestroyWindow();
        friend class WindowManager;
        friend class PoolAllocator;



    };
}
#endif