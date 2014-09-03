#include "G_Window.h"
#include "G_WindowHook.h"
#include "../Reflection/G_Reflection.h"
#include "../Entity Component/G_GameObjectManager.h"
#include <GLFW\glfw3.h>
namespace Gem
{
    using namespace Reflection;
    Window::Window() : Object()
    {
        m_Title = "";
        m_Width = 0;
        m_Height = 0;
        m_Handle = nullptr;
        m_HandleID = 0;
        m_WindowHook = nullptr;
    }
    Window::~Window()
    {
        
    }

    int Window::handle()
    {
        return m_HandleID;
    }
    int Window::width()
    {
        return m_Width;
    }

    int Window::height()
    {
        return m_Height;
    }
    char * Window::title()
    {
        return m_Title;
    }

    //Reflection Virtuals
    Pointer<Reflection::Type> Window::getType()
    {
        return typeOf("Window");
    }

    //These are events that will be sent from a window manager
    void Window::keyEvent(int aKey, int aScanCode, int aAction, int aMods)
    {
        //TODO: Write implementation
    }
    void Window::mouseButtonEvent(int aButton, int aAction, int aMods)
    {
        //TODO: Write implementation
    }
    void Window::mouseMoveEvent(float aX, float aY)
    {
        //TODO: Write implementation
    }
    void Window::mouseScroll(float aX, float aY)
    {
        //TODO: Write implementation
    }
    void Window::update()
    {
        //if updating in background
        //glfwMakeContextCurrent(m_Handle);
        //TODO: Write implementation
        if(m_WindowHook != nullptr)
        {
            m_WindowHook->update();
        }
        GameObjectManager::instance()->renderPass(m_HandleID,0,0);
        //Handle flags
        glfwSwapBuffers(m_Handle);
    }
    void Window::onCreate()
    {
        //TODO: Write implementation
    }
    void Window::onDestroy()
    {
        //TODO: Write implementation
    }
    void Window::createWindow()
    {
        onCreate();
    }
    void Window::destroyWindow()
    {
        onDestroy();
    }
}