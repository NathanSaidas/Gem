#include "Application.h"


#include <glew-1.10.0\include\GL\glew.h>
#include <GLFW_3.0.4\include\GLFW\glfw3.h>


#include "../Engine.h"
//Error Handling
//#include "../Core/Debug.h"
//#include "../Core/Errors/CoreErrors.h"

#include "BaseWindow.h"

using namespace Gem::Debugging;
using namespace Gem::Reflection;

namespace Gem
{

#pragma region GLFW Callback Handling

    //List of private methods used in the Input class for processing events. These must be initialized by Application::Execute before being used.
    Method<Input, void, KeyCode>           PROCESS_KEY_DOWN_METHOD          = Method<Input, void, KeyCode>();
    Method<Input, void, KeyCode>           PROCESS_KEY_UP_METHOD            = Method<Input, void, KeyCode>();
    Method<Input, void, MouseButton>       PROCESS_MOUSE_BUTTON_DOWN_METHOD = Method<Input, void, MouseButton>();
    Method<Input, void, MouseButton>       PROCESS_MOUSE_BUTTON_UP_METHOD   = Method<Input, void, MouseButton>();
    Method<Input, void, Float32, Float32>  PROCESS_MOUSE_MOVE_METHOD        = Method<Input, void, Float32, Float32>();
    Method<Input, void, Float32>           PROCESS_MOUSE_SCROLL_METHOD      = Method<Input, void, Float32>();
    Method<Input, void, UInt32>            PROCESS_CHARACTER                = Method<Input, void, UInt32>();

    void OnMouseButtonEvent(GLFWwindow* aWindow, int button, int action, int mods)
    {
        Input* input = Input::GetInstance();
        if (GLFW_PRESS == action)
        {
            PROCESS_MOUSE_BUTTON_DOWN_METHOD(input, (MouseButton)button);
        }
        else if (GLFW_RELEASE)
        {
            PROCESS_MOUSE_BUTTON_UP_METHOD(input, (MouseButton)button);
        }
    }

    void OnMousePositionEvent(GLFWwindow* aWindow, double x, double y)
    {
        Input* input = Input::GetInstance();
        PROCESS_MOUSE_MOVE_METHOD(input, static_cast<Float32>(x), static_cast<Float32>(y));
    }

    void OnMouseEnter(GLFWwindow* aWindow, int entered)
    {
        if (entered == GL_TRUE)
        {

        }
        else if (entered == GL_FALSE)
        {

        }
    }

    void OnScroll(GLFWwindow* aWindow, double x, double y)
    {
        Input* input = Input::GetInstance();
        PROCESS_MOUSE_SCROLL_METHOD(input, static_cast<Float32>(y));
    }

    void OnKeyPress(GLFWwindow* aWindow, int key, int scancode, int action, int mods)
    {
        if (key != 340)
        {
            int i = 0;

        }

        Input* input = Input::GetInstance();
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            PROCESS_KEY_DOWN_METHOD(input, (KeyCode)key);
        }
        else if (action == GLFW_RELEASE)
        {
            PROCESS_KEY_UP_METHOD(input, (KeyCode)key);
        }
    }

    void OnCharPress(GLFWwindow* aWindow, UInt32 character)
    {
        Input* input = Input::GetInstance();
        PROCESS_CHARACTER(input, character);
    }
#pragma endregion


    RDEFINE_CLASS(Application, object)

    Application* Application::s_Instance = nullptr;

    Application* Application::GetInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new Application();
        }
        return s_Instance;
    }
    void Application::Terminate()
    {
        if (s_Instance != nullptr)
        {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    Application::Application()
    {

    }

    Application::~Application()
    {

    }
    SInt32 Application::Execute()
    {
        Application* app = GetInstance();

        //Initialize Subsystems (Memory, Reflection)
        Memory::MemoryManager::Initialize();
        Reflection::Runtime::Compile(nullptr);
        Time::Initialize();
        Input::Initialize();

        //Initialize Window Management API
        glfwInit();
        Input* input = Input::GetInstance();
        Type type = input->GetType();

        //Get Address of ProcessKeyDown function
        MethodInfo<Input, void, KeyCode>* processKeyDown = dynamic_cast<MethodInfo<Input, void, KeyCode>*>(type.GetMethodInfo("ProcessKeyDown"));
        if (processKeyDown != nullptr)
        {
            PROCESS_KEY_DOWN_METHOD = processKeyDown->GetMethod();
        }
        //Get Address of ProcessKeyUp function
        MethodInfo<Input, void, KeyCode>* processKeyUp = dynamic_cast<MethodInfo<Input, void, KeyCode>*>(type.GetMethodInfo("ProcessKeyUp"));
        if (processKeyUp != nullptr)
        {
            PROCESS_KEY_UP_METHOD = processKeyUp->GetMethod();
        }
        //Get Address of ProcessMouseDown function
        MethodInfo<Input, void, MouseButton>* processMouseButtonDown = dynamic_cast<MethodInfo<Input, void, MouseButton>*>(type.GetMethodInfo("ProcessMouseDown"));
        if (processMouseButtonDown != nullptr)
        {
            PROCESS_MOUSE_BUTTON_DOWN_METHOD = processMouseButtonDown->GetMethod();
        }
        //Get Address of ProcessMouseUp function
        MethodInfo<Input, void, MouseButton>* processMouseButtonUp = dynamic_cast<MethodInfo<Input, void, MouseButton>*>(type.GetMethodInfo("ProcessMouseUp"));
        if (processMouseButtonUp != nullptr)
        {
            PROCESS_MOUSE_BUTTON_UP_METHOD = processMouseButtonUp->GetMethod();
        }
        //Get Address of ProcessMouseMove function
        MethodInfo<Input, void, Float32, Float32>* processMouseMove = dynamic_cast<MethodInfo<Input, void, Float32, Float32>*>(type.GetMethodInfo("ProcessMouseMove"));
        if (processMouseMove != nullptr)
        {
            PROCESS_MOUSE_MOVE_METHOD = processMouseMove->GetMethod();
        }
        //Get Address of ProcessMouseScroll function
        MethodInfo<Input, void, Float32>* processMouseScroll = dynamic_cast<MethodInfo<Input, void, Float32>*>(type.GetMethodInfo("ProcessMouseScroll"));
        if (processMouseScroll != nullptr)
        {
            PROCESS_MOUSE_SCROLL_METHOD = processMouseScroll->GetMethod();
        }
        //Get Address of ProcessCharacter function
        MethodInfo<Input, void, UInt32>* processChar = dynamic_cast<MethodInfo<Input, void, UInt32>*>(type.GetMethodInfo("ProcessChar"));
        if (processChar != nullptr)
        {
            PROCESS_CHARACTER = processChar->GetMethod();
        }

        //Create the main window.
        app->m_MainWindow = MEM_POOL_ALLOC_T(BaseWindow, "Gem Framework");
        app->m_MainWindow->Open();


        //Game Loop
        while (!ShouldWindowClose(app->m_MainWindow))
        {
            Time::Update();
            Input::GetInstance()->ListenForFirstKey();
            glfwPollEvents();



            app->m_MainWindow->SwapBuffer();
            Input::GetInstance()->Update();
            Memory::MemoryManager::GetInstance()->ResetFrame();
        }

        app->m_MainWindow->Close();
        MEM_POOL_DEALLOC_T(app->m_MainWindow, BaseWindow);

        glfwTerminate();

        //Set as empty methods before input cleanup.
        PROCESS_KEY_DOWN_METHOD             = Method<Input, void, KeyCode>();
        PROCESS_KEY_UP_METHOD               = Method<Input, void, KeyCode>();
        PROCESS_MOUSE_BUTTON_DOWN_METHOD    = Method<Input, void, MouseButton>();
        PROCESS_MOUSE_BUTTON_UP_METHOD      = Method<Input, void, MouseButton>();
        PROCESS_MOUSE_MOVE_METHOD   = Method<Input, void, Float32, Float32>();
        PROCESS_MOUSE_SCROLL_METHOD = Method<Input, void, Float32>();

        Input::Terminate();
        Reflection::Runtime::Terminate();
        Memory::MemoryManager::Terminate();
        system("pause");
        return EXIT_SUCCESS;
    }

    bool Application::ShouldWindowClose(BaseWindow* aWindow)
    {
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::ShouldWindowClose");
            error.Log("Gem");
            return false;
        }

        if (aWindow->GetHandle() != nullptr)
        {
            return glfwWindowShouldClose((GLFWwindow*)aWindow->GetHandle());
        }
        return false;
    }

    bool Application::OpenWindow(BaseWindow* aWindow)
    {
        //TODO(Nathan): Check if this is the main thread.
        //If this is not the main thread return false.

        //Exit: Missing reference
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::OpenWindow");
            error.Log("Gem");
            return false;
        }

        //Exit: Window is already open.
        if (aWindow->IsOpen())
        {
            return true;
        }

        //Set Window Hints
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RED_BITS, (SInt32)aWindow->GetRedBits());
        glfwWindowHint(GLFW_GREEN_BITS, (SInt32)aWindow->GetGreenBits());
        glfwWindowHint(GLFW_BLUE_BITS, (SInt32)aWindow->GetBlueBits());
        glfwWindowHint(GLFW_ALPHA_BITS, (SInt32)aWindow->GetAlphaBits());
        glfwWindowHint(GLFW_DEPTH_BITS, (SInt32)aWindow->GetDepthBits());
        glfwWindowHint(GLFW_STENCIL_BITS, (SInt32)aWindow->GetStencilBits());
        glfwWindowHint(GLFW_RESIZABLE, (SInt32)aWindow->IsResizeable());
        GLFWmonitor* monitor = nullptr;
        if (aWindow->IsFullscreen())
        {
            monitor = glfwGetPrimaryMonitor();
        }
        //Create the window.
        GLFWwindow* windowHandle = glfwCreateWindow(aWindow->GetWidth(), aWindow->GetHeight(), aWindow->GetName().c_str(), monitor, nullptr);
        if (windowHandle != nullptr)
        {
            //Register callbacks for the window to process input.
            glfwSetMouseButtonCallback(windowHandle, OnMouseButtonEvent);
            glfwSetCursorPosCallback(windowHandle, OnMousePositionEvent);
            glfwSetCursorEnterCallback(windowHandle, OnMouseEnter);
            glfwSetScrollCallback(windowHandle, OnScroll);
            glfwSetKeyCallback(windowHandle, OnKeyPress);
            glfwSetCharCallback(windowHandle, OnCharPress);

            //Use reflection to set the value of the handle to the windowHandle created.
            Type type = aWindow->GetType();
            Reflection::MemberInfo memberInfo = type.GetMember("m_Handle");
            void** member = (void**)memberInfo.GetOffsetPointer(aWindow);
            if (member != nullptr)
            {
                (*member) = windowHandle;
            }
            return true;
        }
        return false;
    }
    bool Application::CloseWindow(BaseWindow* aWindow)
    {
        //Validate the window object.

        //Exit: Missing reference
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::CloseWindow");
            error.Log("Gem");
            return false;
        }

        //Exit: Invalid Object
        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::CloseWindow");
            error.Log("Gem");
            return false;
        }

        //Notify the window about its destruction.
        aWindow->OnDestroy();
        //Release window resources.
        glfwDestroyWindow((GLFWwindow*)aWindow->GetHandle());
        //Set window handle to null with Reflection.
        Type type = aWindow->GetType();
        Reflection::MemberInfo memberInfo = type.GetMember("m_Handle");
        void** member = (void**)memberInfo.GetOffsetPointer(aWindow);
        if (member != nullptr)
        {
            (*member) = nullptr;
            return true;
        }
        return false;
    }
    bool Application::SetWindowSize(BaseWindow* aWindow)
    {
        //Exit: Missing reference
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::SetWindowSize");
            error.Log("Gem");
            return false;
        }

        //Exit: Invalid Object
        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::SetWindowSize");
            error.Log("Gem");
            return false;
        }
        GLFWwindow* window = (GLFWwindow*)aWindow->GetHandle();
        glfwSetWindowSize(window, (SInt32)aWindow->GetWidth(), (SInt32)aWindow->GetHeight());
        return true;
    }

    bool Application::ChangeFullscreenWindow(BaseWindow* aWindow)
    {
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::ChangeFullscreenWindow");
            error.Log("Gem");
            return false;
        }

        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::ChangeFullscreenWindow");
            error.Log("Gem");
            return false;
        }

        //To change a window to fullscreen it must be closed and re-opened with the fullscreen flag set.
        //The same applies vice-versa.
        CloseWindow(aWindow);
        OpenWindow(aWindow);
        return false;
    }

    void Application::SwapBuffer(BaseWindow* aWindow)
    {
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::SwapBuffer");
            error.Log("Gem");
            return;
        }

        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::SwapBuffer");
            error.Log("Gem");
            return;
        }

        GLFWwindow* window = (GLFWwindow*)aWindow->GetHandle();
        glfwSwapBuffers(window);
    }
}