#include "AppHandler.h"

using namespace Gem::Reflection;
using namespace Gem::Memory;
using namespace Gem::EntityComponent;
using namespace Gem::Debugging;


namespace Gem
{

	RDEFINE_CLASS(AppHandler, BaseAppHandler)

	AppHandler::AppHandler()
	{

	}
	AppHandler::~AppHandler()
	{

	}


	void AppHandler::OnStart()
	{
		Debug::Log("AppHandler", "OnStart");
	}

	void AppHandler::OnStop()
	{
		Debug::Log("AppHandler", "OnStop");
	}

	void AppHandler::OnSystemsInitialized()
	{
		Debug::Log("AppHandler", "OnSystemsInitialized");
	}

	void AppHandler::Update()
	{

		if (Input::GetKeyDown(KeyCode::Alpha1))
		{
			Debug::Log("AppHandler", "Update");
		}
	}
}