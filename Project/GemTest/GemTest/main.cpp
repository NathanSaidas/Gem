#include <stdlib.h>
#include <utility>
#include <memory>
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
}

int main(int argc, char ** argv)
{

	Application::RegisterEvent(ApplicationEventType::OnStart, OnStart);
	Application::RegisterEvent(ApplicationEventType::OnStop, OnStop);
	Application::RegisterEvent(ApplicationEventType::OnSystemsInitialized, OnInitialized);
	int code = Gem::Application::Execute("Test App", Gem::ApplicationType::Console);

	//Gem::Memory::MemoryManager::Initialize();
	//Gem::Reflection::Runtime::Compile(nullptr);
	//
	//Gem::Vector2 vec1;
	//vec1.Set(72.0f, 16.0f);
	//Gem::Vector2 vec2 = vec1.Normalized();
	//
	//Gem::Debugging::Debug::LogFormat("Math", "Vec1 = %s", vec1.ToString().c_str());
	//Gem::Debugging::Debug::LogFormat("Math", "Vec2 = %s", vec2.ToString().c_str());
	//
	//Gem::Reflection::Runtime::Terminate();
	//Gem::Memory::MemoryManager::Terminate();
	system("pause");

	return code;
}
