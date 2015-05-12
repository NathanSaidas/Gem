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

	UInt32 Application::Execute(const std::string & aApplicationName, const ApplicationType & aType)
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Application();
			s_Instance->m_ApplicationName = aApplicationName;
			s_Instance->m_ApplicationType = aType;
			s_Instance->m_ExitCode = EXIT_SUCCESS;
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