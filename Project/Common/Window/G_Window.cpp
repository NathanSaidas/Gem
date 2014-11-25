#include "G_Window.h"
#include "G_WindowHook.h"
#include "../Reflection/G_Reflection.h"
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

    int Window::Handle()
    {
        return m_HandleID;
    }
    int Window::Width()
    {
        return m_Width;
    }

    int Window::Height()
    {
        return m_Height;
    }
    char * Window::Title()
    {
        return m_Title;
    }

    //Reflection Virtuals
    Pointer<Reflection::Type> Window::GetType()
    {
        return typeOf("Window");
    }

    //These are events that will be sent from a window manager
    void Window::KeyEvent(int aKey, int aScanCode, int aAction, int aMods)
    {
        //TODO: Write implementation
    }
    void Window::MouseButtonEvent(int aButton, int aAction, int aMods)
    {
        //TODO: Write implementation
    }
    void Window::MouseMoveEvent(float aX, float aY)
    {
        //TODO: Write implementation
    }
    void Window::MouseScroll(float aX, float aY)
    {
        //TODO: Write implementation
    }
	void Window::OnWindowResize(float aWidth, float aHeight)
	{
		m_Width = (int)aWidth;
		m_Height = (int)aHeight;
	}
    void Window::Update()
    {
        //if updating in background
        glfwMakeContextCurrent(m_Handle);
        //TODO: Write implementation
        if(m_WindowHook != nullptr)
        {
            m_WindowHook->Update();
        }
        //Handle flags
        glfwSwapBuffers(m_Handle);
    }
    void Window::OnCreate()
    {
        //TODO: Write implementation
    }
    void Window::OnDestroy()
    {
        //TODO: Write implementation
    }
    void Window::CreateWindow()
    {
        OnCreate();
    }
    void Window::DestroyWindow()
    {
        OnDestroy();
    }
}