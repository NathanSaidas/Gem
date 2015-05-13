#include <glew-1.10.0\include\GL\glew.h>
#include "Application.h"
#include "../Engine.h"


using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(Application,object)
	Application * Application::s_Instance = nullptr;
	Event<> Application::s_OnStart = Event<>();
	Event<> Application::s_OnStop = Event<>();
	Event<> Application::s_OnSystemsInitialized = Event<>();

	Application::Application()
	{

	}

	Application::~Application()
	{

	}
#ifdef _WIN32
	UInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType, HINSTANCE aHandleInstance)
#else
	UInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType)
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
				   InvalidArgument error = GENERATE_INVALID_ARGUMENT("aEventType", "Application::RegisterEvent", 16);
				   Debug::InvalidArgument(error);
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
				   InvalidArgument error = GENERATE_INVALID_ARGUMENT("aEventType", "Application::RegisterEvent", 16);
				   Debug::InvalidArgument(error);
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

	void Application::StartWindow()
	{
		//Invoke callback OnStartWindow
		s_OnStart.Invoke();
		//Initialize Systems
		Memory::MemoryManager::Initialize();
		Reflection::Runtime::Compile(nullptr);
		//Invoke callback for OnSystemsInitialized.
		s_OnSystemsInitialized.Invoke();

		//TODO(Nathan): Create Window, begin game loop

		Win32Window * window = MEM_POOL_ALLOC_T(Win32Window,m_ApplicationName, m_HandleInstance);
		window->Open();

		Time::Initialize();
		MSG msg;
		msg.message = ~WM_QUIT;
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Time::Update();
				//Update
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				window->SwapBuffer();

				Memory::MemoryManager::GetInstance()->ResetFrame();
			}
		}
		window->Close();
		MEM_POOL_DEALLOC_T(window, Win32Window);

		m_ExitCode = static_cast<int>(msg.wParam);

		//Terminate Systems
		Reflection::Runtime::Terminate();
		Memory::MemoryManager::Terminate();
		//Invoke callback OnStopWindow
		s_OnStop.Invoke();
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



		//Terminate Systems
		Reflection::Runtime::Terminate();
		Memory::MemoryManager::Terminate();
		//Invoke callback OnStopWindow
		s_OnStop.Invoke();
	}

}