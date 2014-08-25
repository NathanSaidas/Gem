#ifndef G_WINDOW_H
#define G_WINDOW_H

#include "../Base Objects/G_Object.h"

//glfw window struct def
struct GLFWwindow;

//This class will serve as the base class for all future windows

namespace Gem
{
    class WindowHook;

    class Window : public Object
    {
    public:
        
        int handle();
        int width();
        int height();
        char * title();

        //Reflection Virtuals
        virtual Reflection::Type * getType();
    protected:
        //These are events that will be sent from a window manager
        virtual void keyEvent(int aKey, int aScanCode, int aAction, int aMods);
        virtual void mouseButtonEvent(int aButton, int aAction, int aMods);
        virtual void mouseMoveEvent(float aX, float aY);
        virtual void mouseScroll(float aX, float aY);

        virtual void update();
        virtual void onCreate();
        virtual void onDestroy();

    private:
        WindowHook * m_WindowHook;
        GLFWwindow * m_Handle;
        int m_Width;
        int m_Height;
        int m_HandleID;
        char * m_Title;

        Window();
        virtual ~Window();

        void createWindow();
        void destroyWindow();
        friend class WindowManager;
        friend class PoolAllocator;



    };
}

#endif