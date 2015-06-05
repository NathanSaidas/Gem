#include <glew-1.10.0\include\GL\glew.h>
#include "Application.h"
#include <mutex>
#include "../Engine.h"
#include <Windows.h>
#include "Input.h"



using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(Application, object)
		Application * Application::s_Instance = nullptr;
	Event<> Application::s_OnStart = Event<>();
	Event<> Application::s_OnStop = Event<>();
	Event<> Application::s_OnSystemsInitialized = Event<>();

	Application::Application()
		: m_DefaultWindow(nullptr),
		m_ShouldQuit(false),
		m_CurrentScene(nullptr)
	{

	}

	Application::~Application()
	{

	}
#ifdef _WIN32
	SInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType, void * aHandleInstance)
	{
		return Execute(aApplicationName, aType, aHandleInstance, nullptr);
	}
	SInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType, void * aHandleInstance, BaseAppHandler * aAppHandler)
#else
	SInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType)
	{
		Execute(aApplicationName, aType, nullptr);
	}
	SInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType, BaseAppHandler * aAppHandler);
#endif
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Application();
			s_Instance->m_ApplicationName = aApplicationName;
			s_Instance->m_ApplicationType = aType;
			s_Instance->m_ExitCode = EXIT_SUCCESS;
#ifdef _WIN32
			s_Instance->m_HandleInstance = aHandleInstance;
#endif
			s_Instance->m_AppHandler = aAppHandler;
			switch (aType)
			{
			case ApplicationType::Console:
				s_Instance->StartConsole();
				break;
			case ApplicationType::Window:
				s_Instance->StartWindow();
				break;
			}
			int exitCode = s_Instance->m_ExitCode;
			delete s_Instance;
			if (aAppHandler != nullptr)
			{
				delete aAppHandler;
			}
			return exitCode;
		}
		return EXIT_FAILURE;
	}

	void Application::RegisterEvent(const ApplicationEventType & aEventType, const ApplicationEventCallback & aCallback)
	{
		switch (aEventType)
		{
		case ApplicationEventType::OnStart:
			s_OnStart.AddListener(aCallback);
			break;
		case ApplicationEventType::OnStop:
			s_OnStop.AddListener(aCallback);
			break;
		case ApplicationEventType::OnSystemsInitialized:
			s_OnSystemsInitialized.AddListener(aCallback);
			break;
		default:
#ifdef _DEBUG
			{
				   InvalidArgument error = InvalidArgument("aEventType", GET_TRACE(0), "Application::RegisterEvent");
				   Debug::Error(Constants::GEM_APPLICATION_LOG_HEADER, &error);
			}
#endif
			break;
		}
	}
	void Application::UnregisterEvent(const ApplicationEventType & aEventType, const ApplicationEventCallback & aCallback)
	{
		switch (aEventType)
		{
		case ApplicationEventType::OnStart:
			s_OnStart.RemoveListener(aCallback);
			break;
		case ApplicationEventType::OnStop:
			s_OnStop.RemoveListener(aCallback);
			break;
		case ApplicationEventType::OnSystemsInitialized:
			s_OnSystemsInitialized.RemoveListener(aCallback);
			break;
		default:
#ifdef _DEBUG
			{
				   InvalidArgument error = InvalidArgument("aEventType", GET_TRACE(0), "Application::UnregisterEvent");
				   Debug::Error(Constants::GEM_APPLICATION_LOG_HEADER, &error);
			}
#endif
			break;
		}
	}

	const ApplicationType Application::GetApplicationType()
	{
		if (s_Instance == nullptr)
		{
			return ApplicationType::Invalid;
		}
		return s_Instance->m_ApplicationType;
	}
	const std::string Application::GetApplicationName()
	{
		if (s_Instance == nullptr)
		{
			return "";
		}
		return s_Instance->m_ApplicationName;
	}

	void Application::Quit()
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_ShouldQuit = true;
		}
	}

	Scene * Application::GetCurrentScene()
	{
		return s_Instance != nullptr ? s_Instance->m_CurrentScene : nullptr;
	}

	Window * Application::GetDefaultWindow()
	{
		return s_Instance != nullptr ? s_Instance->m_DefaultWindow : nullptr;
	}

	void Application::StartThread(ThreadEntryCallback aCallback)
	{
		if (s_Instance == nullptr)
		{
			return;
		}

		Pointer<Thread> thread;
		s_Instance->m_Threads.push_back(thread);
		thread->Start(aCallback);
	}

#ifdef _WIN32
#ifdef CreateWindow
#undef CreateWindow
#endif

	void Application::CreateWindow(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return;
		}
#ifdef _WIN32
		Pointer<Win32Window> win32Window = Pointer<Win32Window>(aName, (HINSTANCE)s_Instance->m_HandleInstance);
		Pointer<Window>  window = win32Window.Cast<Window>();
		//
		if (window->Open())
		{
			s_Instance->m_Windows.push_back(window);
		}
#else

#endif
	}

#ifdef _WIN32
#define CreateWindow CreateWindowA
#endif
#endif

	Window * Application::GetWindow(void * aHandle)
	{
		if (s_Instance == nullptr)
		{
			return nullptr;
		}

		for (std::vector<Pointer<Window>>::iterator it = s_Instance->m_Windows.begin();
			it != s_Instance->m_Windows.end();
			it++)
		{
			if ((*it)->GetHandle() == aHandle)
			{
				return (*it).Raw();
			}
		}
		return nullptr;
	}

	void Application::Win32SendMessage(const Win32Message & aMessage, Window * aWindow)
	{
		if (s_Instance == nullptr)
		{
			return;
		}

		switch (aMessage)
		{
		case Win32Message::Destroy:
			{
				//Handle window unregister.
				for (std::vector<Pointer<Window>>::iterator it = s_Instance->m_Windows.begin();
					it != s_Instance->m_Windows.end();
					it++)
				{
					if ((*it).Raw() == aWindow)
					{
						s_Instance->m_Windows.erase(it);
						break;
					}
				}
			}
			break;
		case Win32Message::Resize:
			if (aWindow != nullptr)
			{
				
			}
			break;
		}
	}

	void Application::Win32SendMessage(const Win32Message & aMessage, Float32 aParam0, Float32 aParam1)
	{
		if (s_Instance == nullptr)
		{
			return;
		}
		KeyCode keyCode = (KeyCode)((SInt32)aParam0);
		MouseButton button = (MouseButton)((SInt32)aParam0);
		Vector2 mousePosition = Vector2(aParam0, aParam1);
		Float32 axis = aParam0;

		switch (aMessage)
		{
			case Win32Message::KeyUp:
				Input::GetInstance()->ProcessKeyUp(keyCode);
			break;
			case Win32Message::KeyDown:
				Input::GetInstance()->ProcessKeyDown(keyCode);
			break;
			case Win32Message::MouseDown:
				Input::GetInstance()->ProcessMouseDown(button);
			break;
			case Win32Message::MouseUp:
				Input::GetInstance()->ProcessMouseUp(button);
			break;
			case Win32Message::MouseMove:
				Input::GetInstance()->ProcessMouseMove(mousePosition.x, mousePosition.y);
			break;
			case Win32Message::MouseWheel:
				Input::GetInstance()->ProcessMouseScroll(axis);
			break;
			default:
				Debug::WarningFormat("Gem::Application", nullptr, "Win32Message %s is not a valid message for this method.", (CString)aMessage);
				break;
		}
	}


	void Application::StartWindow()
	{
		//Invoke callback OnStartWindow

		if (m_AppHandler != nullptr)
		{
			m_AppHandler->OnStart();
		}
		s_OnStart.Invoke();
		//Initialize Systems
		Memory::MemoryManager::Initialize();
		Reflection::Runtime::Compile(nullptr);
		Input::Initialize();
		//Invoke callback for OnSystemsInitialized.
		if (m_AppHandler != nullptr)
		{
			m_AppHandler->OnSystemsInitialized();
		}
		s_OnSystemsInitialized.Invoke();

		Pointer<Win32Window> win32Window = Pointer<Win32Window>(m_ApplicationName, (HINSTANCE)m_HandleInstance);
		win32Window->Open();

		m_DefaultWindow = win32Window.Raw();
		m_CurrentScene = MEM_POOL_ALLOC_T(Scene);

		//m_CurrentScene = MEM_POOL_ALLOC_T(Scene);

		Time::Initialize();
		MSG msg;
		msg.message = ~WM_QUIT;
		while (!m_ShouldQuit)
		{
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Internal Update
				Time::Update();
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//Normal Updates...
				if (m_AppHandler != nullptr)
				{
					m_AppHandler->Update();
				}

				m_CurrentScene->Update();


				//Internal Post Update
				Input::GetInstance()->Update();
				win32Window->SwapBuffer();
				CheckThreads();
				Memory::MemoryManager::GetInstance()->ResetFrame();
			}
		}
		
		win32Window->Close();
		MEM_POOL_DEALLOC_T(m_CurrentScene, Scene);
		win32Window.Terminate();
		m_DefaultWindow = nullptr;
		//MEM_POOL_DEALLOC_T(window, Win32Window);

		//m_ExitCode = static_cast<int>(msg.wParam);

		while (m_Threads.size() > 0)
		{
			//Waiting for threads to quit.
			CheckThreads();
		}
		//Invoke callback OnStopWindow
		if (m_AppHandler != nullptr)
		{
			m_AppHandler->OnStop();
		}
		s_OnStop.Invoke();


		//Terminate Systems
		Input::Terminate();
		Reflection::Runtime::Terminate();
		Memory::MemoryManager::Terminate();
		

	}
	void Application::StartConsole()
	{
		//Invoke callback OnStartWindow
		s_OnStart.Invoke();
		//Initialize Systems
		Memory::MemoryManager::Initialize();
		Reflection::Runtime::Compile(nullptr);
		//Invoke callback for OnSystemsInitialized.
		s_OnSystemsInitialized.Invoke();

		while (m_Threads.size() > 0)
		{
			CheckThreads();
		}

		//Terminate Systems
		Reflection::Runtime::Terminate();
		Memory::MemoryManager::Terminate();
		//Invoke callback OnStopWindow
		s_OnStop.Invoke();
	}

	bool Application::FindThread(const size_t & aID, Pointer<Thread> & aThread) const
	{
		for (std::vector<Pointer<Thread>>::const_iterator it = m_Threads.begin(); it != m_Threads.end(); it++)
		{
			if ((*it)->GetID() == aID)
			{
				aThread = *it;
				return true;
			}
		}
		return false;
	}
	void Application::CheckThreads()
	{
		std::mutex mu;
		std::lock_guard<std::mutex> guard(mu);
		for (SInt32 i = (SInt32)m_Threads.size() - 1; i >= 0; i--)
		{
			if (!m_Threads[i]->IsExecuting())
			{
				m_Threads.erase(m_Threads.begin() + i);
			}
		}
	}

}