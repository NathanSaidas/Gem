#include "G_Application.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "../Input/G_Input.h"
#include "../Utilities/G_Time.h"
#include "G_WindowManager.h"
#include "G_WindowHook.h"
#include "../Renderer/G_Graphics.h"



namespace Gem
{
    using namespace Reflection;

    Application * Application::s_Instance = nullptr;
    Application * Application::Instance()
    {
        if(s_Instance == nullptr)
        {
			s_Instance = new Application();
        }
        return s_Instance;
    }
    void Application::Destroy()
    {
        if(s_Instance != nullptr)
        {
			delete s_Instance;
			s_Instance = nullptr;
        }
    }
    Application::Application()
    {
		m_ShouldQuit = false;
		m_ExitStatus = 0;
    }
    Application::~Application()
    {

    }
    int Application::Execute()
    {
		OnApplicationStart();
		OnExecute();
		OnApplicationClose();
		return m_ExitStatus;
    }
	void Application::Exit(int aStatus)
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_ExitStatus = aStatus;
			s_Instance->m_ShouldQuit = true;
		}
	}
	void Application::PauseOnExit()
	{
		Instance()->m_PauseOnExit = true;
	}
    //called before run
    void Application::OnApplicationStart()
    {
		MemoryManager::Instance();
        Runtime::Instance();
        //Initialize things
        if(WindowManager::Init() == false)
        {
            m_ShouldQuit = true;
            m_ExitStatus = -1;
            return;
        }
		WindowManager::Instance();
		Input::Instance();
		Graphics::Instance();

    }
    //called after init
    void Application::OnExecute()
    {
        WindowManager * windowManager = WindowManager::Instance();
        Input * input = Input::Instance();
        
        int mainWindowHandle = windowManager->CreateWindow("Main Window", 1280, 768);
        if (mainWindowHandle == INVALID_WINDOW_HANDLE)
        {
            m_ShouldQuit = true;
            m_ExitStatus = -1;
            return;
        }
        MemoryHandle<WindowHook> windowHookHandle = Memory::Instantiate<WindowHook>();
        windowManager->AttachHook(windowHookHandle, mainWindowHandle);

        while (WindowManager::Instance()->HasWindows() && m_ShouldQuit == false)
		{
			float currentTime = Time::GetTime();
			float lastTime = Time::s_CurrentTime;
			Time::s_DeltaTime = currentTime - lastTime;
			Time::s_CurrentTime = currentTime;

			Input::Instance()->Update();
            WindowManager::Instance()->Update();
			Graphics::Instance()->Render();
			

			//Give time for memory manager to update.
			MemoryManager::Instance()->Update();
		}

        WindowManager::Instance()->DestroyWindow(mainWindowHandle);
        windowHookHandle = Memory::DestroyHandle(windowHookHandle);
    }
    //called after run
    void Application::OnApplicationClose()
    {
		Graphics::Destroy();
        Input::Destroy();
        WindowManager::Destroy();
		WindowManager::DeInit();
		Runtime::Destroy();
		MemoryManager::Destroy();
		if (m_PauseOnExit == true)
		{
#ifdef WIN32
			system("pause");
#endif
		}
    }

	void Application::OnWindowResize(float aWidth, float aHeight)
	{
		
	}

    Pointer<Reflection::Type> Application::GetType()
    {
        return typeOf("Application");
    }
    
}