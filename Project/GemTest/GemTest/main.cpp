#include <stdlib.h>
#include <utility>
#include <memory>
#include <Windows.h>
#include "../../Common/Engine.h"

using namespace Gem;

void OnStart()
{
	Gem::ApplicationType appType = Gem::Application::GetApplicationType();
	Gem::Debugging::Debug::LogFormat("Test", nullptr, "Starting Application Type %s", appType.ToString().c_str());
}

void OnStop()
{
	Gem::Debugging::Debug::LogFormat("Test", nullptr, "Stopping Application %s", Gem::Application::GetApplicationName().c_str());
}


void OnInitialized()
{
	Type type = Reflection::Runtime::TypeOf<Application>();
	Debugging::Debug::Log("Test","Initialize Stuff Here...");

	IniFileStream iniFile;
	Array<int> ints(5);
	
	iniFile.AddSection("Types");
	iniFile.BindSection("Types");
	Array<Type> types = Reflection::Runtime::GetAllTypes();
	for (UInt32 i = 0; i < types.GetCount(); i++)
	{
		iniFile.AddString(types[i].GetName().append("_Name"), types[i].GetName());
		iniFile.AddInt(types[i].GetName().append("_TypeID"), types[i].GetTypeID());
	}
	iniFile.LogConsole();
}

int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLineArgs, int aShowCommand)
{
	try
	{
		Application::RegisterEvent(ApplicationEventType::OnStart, OnStart);
		Application::RegisterEvent(ApplicationEventType::OnStop, OnStop);
		Application::RegisterEvent(ApplicationEventType::OnSystemsInitialized, OnInitialized);
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
