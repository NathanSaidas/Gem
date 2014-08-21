#include "G_WindowManager.h"
#include "G_Window.h"
#include "G_WindowHook.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "../Memory/G_Memory.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Utilities.h"
#include "../Input/G_Input.h"
namespace Gem
{
    using namespace Reflection;
    WindowManager * WindowManager::s_Instance = 0;
    bool WindowManager::init()
    {
        glfwSetErrorCallback(errorCallback);
        if(!glfwInit())
        {
            return false;
        }
        return true;
    }
    void WindowManager::deinit()
    {
        glfwTerminate();
    }
    WindowManager * WindowManager::instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::instantiate<WindowManager>();
        }
        return s_Instance;
    }
    void WindowManager::destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::destroy<WindowManager>(s_Instance);
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

    int WindowManager::createWindow(char * aTitle, int aWidth, int aHeight)
    {
        Window * window = Memory::instantiate<Window>();
        window->m_Title = aTitle;
        window->m_Width = aWidth;
        window->m_Height = aHeight;
        window->m_HandleID = m_WindowHandleGen.getUniqueNumber();
        
        //Specify window hints
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        window->m_Handle = glfwCreateWindow(aWidth,aHeight,aTitle,NULL,NULL);

        if(!window->m_Handle)
        {
            //Error
            Memory::destroy<Window>(window);
            return -1;
        }

        glfwMakeContextCurrent(window->m_Handle);
        //Set event callbacks
        glfwSetKeyCallback(window->m_Handle,keyEvent);
        glfwSetMouseButtonCallback(window->m_Handle,mouseEvent);
        glfwSetCursorPosCallback(window->m_Handle, mouseMove);
        glfwSetScrollCallback(window->m_Handle, mouseScroll);

        if(m_HasInitGlew == false)
        {
            if(glewInit() == GLEW_OK)
            {
                m_HasInitGlew = true;
            }
            else
            {
                //error
            }
        }

        m_Windows.push_back(window);
        return window->m_HandleID;
    }
    void WindowManager::destroyWindow(int aHandle)
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
                hook->onDetachFromWindow(window->m_HandleID);
                hook->m_Window = nullptr;
            }
            window = Memory::destroy<Window>(window);
        }
    }
    void WindowManager::attachHook(WindowHook * aHook, int aHandle)
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
                aHook->onDetachFromWindow(aHook->m_Window->m_HandleID);
            }
            if(window->m_WindowHook != nullptr)
            {
                window->m_WindowHook->onDetachFromWindow(window->m_HandleID);
            }

            window->m_WindowHook = aHook;
            aHook->m_Window = window;
            aHook->onAttachToWindow(window->m_HandleID);
        }
    }
    void WindowManager::detachHook(WindowHook * aHook, int aHandle)
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
            aHook->onDetachFromWindow(window->m_HandleID);
            aHook->m_Window = nullptr;
            window->m_WindowHook = nullptr;
        }
    }

    int WindowManager::getFocusedWindow()
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

    Type WindowManager::getType()
    {
        return TypeFactory::create("WindowManager",TypeID::WINDOW_MANAGER,sizeof(WindowManager));
    }
    Type WindowManager::baseType()
    {
        return Object::getType();
    }
    Type * WindowManager::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
    }


    GLFWwindow * WindowManager::getCurrentWindow()
    {
        int id = getFocusedWindow();
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] !=nullptr && m_Windows[i]->m_HandleID == id)
            {
                return m_Windows[i]->m_Handle;
            }
        }
        return nullptr;
    }
    bool WindowManager::hasWindows()
    {
        return m_Windows.size() > 0;
    }
    void WindowManager::update()
    {
        glfwPollEvents();
        //Update all the windows
        for(int i = 0; i < m_Windows.size(); i++)
        {
            if(m_Windows[i] != nullptr)
            {
                
                m_Windows[i]->update(); 
            }
        }
        
        //Check the closing flag on any of the windows.
        //If the closing flag on the main window (window handle 0) is true then close all other windows
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
                destroyWindow(m_Windows[i]->m_HandleID);
            }
        }

        if(closeApp == true)
        {
            //Destroy all windows
            for(int i = m_Windows.size() - 1; i >= 0; i--)
            {
                destroyWindow(m_Windows[i]->m_HandleID);
            }
        }

        
    }


    void WindowManager::errorCallback(int aError, const char * aDescription)
    {
    }
    void WindowManager::keyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods)
    {
        instance()->processKeyEvent(aWindow,aKey,aScanCode,aAction,aMods);
    }
    void WindowManager::mouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods)
    {
        instance()->processMouseEvent(aWindow,aButton,aAction,aMods);
    }
    void WindowManager::mouseMove(GLFWwindow* aWindow, double aX, double aY)
    {
        instance()->processMouseMove(aWindow,aX,aY);
    }
    void WindowManager::mouseScroll(GLFWwindow * aWindow, double aX, double aY)
    {
        instance()->processMouseScroll(aWindow,aX,aY);
    }

    void WindowManager::processKeyEvent(GLFWwindow * aWindow, int aKey, int aScanCode, int aAction, int aMods)
    {
        Input::instance()->processKeyEvent(aKey,aAction,aMods);
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
    void WindowManager::processMouseEvent(GLFWwindow * aWindow, int aButton, int aAction, int aMods)
    {
        Input::instance()->processMouseEvent(aButton,aAction,aMods);
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
    void WindowManager::processMouseMove(GLFWwindow* aWindow, double aX, double aY)
    {
        Input::instance()->processMouseMove(aX,aY);
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
    void WindowManager::processMouseScroll(GLFWwindow * aWindow, double aX, double aY)
    {
        Input::instance()->processMouseScroll(aX,aY);
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
}