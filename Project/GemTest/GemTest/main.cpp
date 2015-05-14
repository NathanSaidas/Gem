#include <stdlib.h>
#include <utility>
#include <memory>
#include <chrono>
#include "../../Common/Engine.h"
#include <Windows.h>

using namespace Gem;

void RenderThread(Thread & aThread)
{
	for (int i = 0; i < 10; i++)
	{
		Debugging::Debug::Log("Render Thread", "Slow Render Frame");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void OnStart();
void OnStop();
void OnInitialized();

void OnStart()
{
	Application::RegisterEvent(ApplicationEventType::OnStop, OnStop);
	Application::RegisterEvent(ApplicationEventType::OnSystemsInitialized, OnInitialized);
	Gem::ApplicationType appType = Gem::Application::GetApplicationType();
	Gem::Debugging::Debug::LogFormat("Test", nullptr, "Starting Application Type %s", appType.ToString().c_str());
}

void OnStop()
{
	Gem::Debugging::Debug::LogFormat("Test", nullptr, "Stopping Application %s", Gem::Application::GetApplicationName().c_str());
}


void OnInitialized()
{
	Gem::Application::StartThread(RenderThread);

}



int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLineArgs, int aShowCommand)
{
	try
	{
		Application::RegisterEvent(ApplicationEventType::OnStart, OnStart);
		int code = Gem::Application::Execute("Test App", Gem::ApplicationType::Window, aCurrentInstance);
		system("pause");
		return code;
	}
	catch (std::exception & aException)
	{
		MessageBox(NULL, aException.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	system("pause");
	return EXIT_FAILURE;
}

//int main(int argc, char ** argv)
//{
//
//	Application::RegisterEvent(ApplicationEventType::OnStart, OnStart);
//	Application::RegisterEvent(ApplicationEventType::OnStop, OnStop);
//	Application::RegisterEvent(ApplicationEventType::OnSystemsInitialized, OnInitialized);
//	int code = Gem::Application::Execute("Test App", Gem::ApplicationType::Console, NULL);
//	system("pause");
//
//	return code;
//}
