#include "G_WindowManager.h"
#include "G_Window.h"
#include "G_WindowHook.h"
#include "../Renderer/G_Graphics.h"
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
#include "../Memory/G_Memory.h"
#include "../Reflection/G_Reflection.h"
#include "../Input/G_Input.h"
#include "G_Application.h"
namespace Gem
{
    const int INVALID_WINDOW_HANDLE = -1;

    using namespace Reflection;
    WindowManager * WindowManager::s_Instance = 0;
    bool WindowManager::Init()
    {
        glfwSetErrorCallback(ErrorCallback);
        if(!glfwInit())
        {
            return false;
        }
        return true;
    }
    void WindowManager::DeInit()
    {
        glfwTerminate();
    }
    WindowManager * WindowManager::Instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::Instantiate<WindowManager>();
        }
        return s_Instance;
    }
    void WindowManager::Destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::Destroy<WindowManager>(s_Instance);
        }
    }
    WindowManager::WindowManager() : Object()
    {
        //GLFW should be initialized by now
        m_HasInitGlew = false;

        
    }
    WindowManager::~WindowManager()
    {

    }

    int WindowManager::CreateWindow(char * aTitle, int aWidth, int aHeight)
    {
        Window * window = Memory::Instantiate<Window>();
        window->m_Title = aTitle;
        window->m_Width = aWidth;
        window->m_Height = aHeight;
        //window->m_HandleID = m_WindowHandleGen.GetUniqueNumber();
        
        //Specify window hints
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        window->m_Handle = glfwCreateWindow(aWidth,aHeight,aTitle,NULL,NULL);

        if(!window->m_Handle)
        {
            //Error
            Memory::Destroy<Window>(window);
            return -1;
        }

        glfwMakeContextCurrent(window->m_Handle);
        //Set event callbacks
        glfwSetKeyCallback(window->m_Handle,KeyEvent);
        glfwSetMouseButtonCallback(window->m_Handle,MouseEvent);
        glfwSetCursorPosCallback(window->m_Handle, MouseMove);
        glfwSetScrollCallback(window->m_Handle, MouseScroll);
		glfwSetWindowSizeCallback(window->m_Handle, WindowResize);

        if(m_HasInitGlew == false)
        {
            if(glewInit() == GLEW_OK)
            {
                m_HasInitGlew = true;
				const GLubyte * renderer = glGetString(GL_RENDERER);
				const GLubyte * version = glGetString(GL_VERSION);
				printf("Rendering On: %s\n", renderer);
				printf("OpenGL Version %s\n", version);

            }
            else
            {
                //error
            }
        }

        m_Windows.push_back(window);
        return window->m_HandleID;
    }
    void WindowManager::DestroyWindow(int aHandle)
    {
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_HandleID == aHandle)
            {
                window = m_Windows[i];
                m_Windows.erase(m_Windows.begin() + i);
                break;
            }
        }
        if(window != nullptr)
        {
            glfwDestroyWindow(window->m_Handle);
            WindowHook * hook = window->m_WindowHook;
            if(hook != nullptr)
            {
                hook->OnDetachFromWindow(window->m_HandleID);
                hook->m_Window = nullptr;
            }
            window = Memory::Destroy<Window>(window);
        }
    }
    void WindowManager::AttachHook(WindowHook * aHook, int aHandle)
    {
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_HandleID == aHandle)
            {
                window = m_Windows[i];
                break;
            }
        }

        if(window != nullptr && aHook != nullptr)
        {
            if(aHook->m_Window != nullptr)
            {
                aHook->OnDetachFromWindow(aHook->m_Window->m_HandleID);
            }
            if(window->m_WindowHook != nullptr)
            {
                window->m_WindowHook->OnDetachFromWindow(window->m_HandleID);
            }

            window->m_WindowHook = aHook;
            aHook->m_Window = window;
            aHook->OnAttachToWindow(window->m_HandleID);
        }
    }
    void WindowManager::DetachHook(WindowHook * aHook, int aHandle)
    {
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_HandleID == aHandle)
            {
                window = m_Windows[i];
                m_Windows.erase(m_Windows.begin() + i);
                break;
            }
        }

        if(window != nullptr && aHook != nullptr)
        {
            aHook->OnDetachFromWindow(window->m_HandleID);
            aHook->m_Window = nullptr;
            window->m_WindowHook = nullptr;
        }
    }

    int WindowManager::GetFocusedWindow()
    {
        //GLFWwindow * windowID = glfwGetCurrentContext();
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr)
            {
                if(glfwGetWindowAttrib(m_Windows[i]->m_Handle,GLFW_FOCUSED))
                {
                    return m_Windows[i]->m_HandleID;
                }
            }
        }
        return -1;
    }
	Vector2 WindowManager::GetWindowSize(int aHandle)
	{
		for (int i = 0; i < m_Windows.size(); i++)
		{
			if (m_Windows[i] != nullptr && m_Windows[i]->Handle() == aHandle)
			{
				return Vector2(m_Windows[i]->Width(), m_Windows[i]->Height());
			}
		}
		return Vector2(0.0f, 0.0f);
	}
    Pointer<Reflection::Type> WindowManager::GetType()
    {
        return typeOf("WindowManager");
    }



    GLFWwindow * WindowManager::GetCurrentWindow()
    {
        int id = GetFocusedWindow();
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] !=nullptr && m_Windows[i]->m_HandleID == id)
            {
                return m_Windows[i]->m_Handle;
            }
        }
        return nullptr;
    }
    bool WindowManager::HasWindows()
    {
        return m_Windows.size() > 0;
    }
    void WindowManager::Update()
    {
        //glfwPollEvents();
        //Update all the windows
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr)
            {
                
                m_Windows[i]->Update(); 
            }
        }
        
        //Check the closing flag on any of the windows.
        //If the closing flag on the main window (window Handle 0) is true then close all other windows
        bool closeApp = false;
        for(int i = m_Windows.size() - 1; i >= 0; i--)
        {
            if(glfwWindowShouldClose(m_Windows[i]->m_Handle))
            {
                //Destroy the window
                if(m_Windows[i]->m_HandleID == 0)
                {
                    closeApp = true;
                }
                DestroyWindow(m_Windows[i]->m_HandleID);
            }
        }

        if(closeApp == true)
        {
            //Destroy all windows
            for(int i = m_Windows.size() - 1; i >= 0; i--)
            {
                DestroyWindow(m_Windows[i]->m_HandleID);
            }
        }
		
        
    }


    void WindowManager::ErrorCallback(int aError, const char * aDescription)
    {
    }
    void WindowManager::KeyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods)
    {
        Instance()->ProcessKeyEvent(aWindow,aKey,aScanCode,aAction,aMods);
    }
    void WindowManager::MouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods)
    {
        Instance()->ProcessMouseEvent(aWindow,aButton,aAction,aMods);
    }
    void WindowManager::MouseMove(GLFWwindow* aWindow, double aX, double aY)
    {
        Instance()->ProcessMouseMove(aWindow,aX,aY);
    }
    void WindowManager::MouseScroll(GLFWwindow * aWindow, double aX, double aY)
    {
        Instance()->ProcessMouseScroll(aWindow,aX,aY);
    }
	void WindowManager::WindowResize(GLFWwindow * aWindow, int aWidth, int aHeight)
	{
		Application::Instance()->OnWindowResize(aWidth, aHeight);
		Instance()->ProcessWindowResize(aWindow, aWidth, aHeight);
	}

    void WindowManager::ProcessKeyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods)
    {
        Input::Instance()->ProcessKeyEvent(aKey,aAction,aMods);
        //find the window
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_Handle == aWindow)
            {
                window = m_Windows[i];
                break;
            }
        }

        if(window != nullptr)
        {
            
        }
    }
    void WindowManager::ProcessMouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods)
    {
        Input::Instance()->ProcessMouseEvent(aButton,aAction,aMods);
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_Handle == aWindow)
            {
                window = m_Windows[i];
                break;
            }
        }

        if(window != nullptr)
        {
            
        }
    }
    void WindowManager::ProcessMouseMove(GLFWwindow* aWindow, double aX, double aY)
    {
        Input::Instance()->ProcessMouseMove(aX,aY);
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_Handle == aWindow)
            {
                window = m_Windows[i];
                break;
            }
        }
        if(window != nullptr)
        {
            
        }
    }
    //Direction
    void WindowManager::ProcessMouseScroll(GLFWwindow * aWindow, double aX, double aY)
    {
        Input::Instance()->ProcessMouseScroll(aX,aY);
        Window * window = nullptr;
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr && m_Windows[i]->m_Handle == aWindow)
            {
                window = m_Windows[i];
                break;
            }
        }
        if(window != nullptr)
        {
            
        }
    }
	void WindowManager::ProcessWindowResize(GLFWwindow * aWindow, int aWidth, int aHeight)
	{
		Window * window = nullptr;
		for (int i = 0; i < m_Windows.size(); i++)
		{
			if (m_Windows[i] != nullptr && m_Windows[i]->m_Handle == aWindow)
			{
				window = m_Windows[i];
				break;
			}
		}
		if (window != nullptr)
		{
			window->OnWindowResize(aWidth, aHeight);
			if (window->m_WindowHook != nullptr)
			{
				window->m_WindowHook->OnWindowResize(aWidth, aHeight);
			}
		}
	}
}