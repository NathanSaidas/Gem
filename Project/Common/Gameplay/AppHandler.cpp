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

		Input::CreateAxis("Mouse X", true);
		Input::CreateAxis("Mouse Y", false);


		Input::CreateAxis("AlphaNum", KeyCode::Alpha1, KeyCode::Alpha2, 2.0f, false);
		Input::CreateButton("ShowLog", KeyCode::S);
		Input::CreateButton("ShowLog", KeyCode::B);
		Input::CreateButton("ShowLog", KeyCode::V);

		m_MousePosition = Input::GetMousePosition();
	}

	void AppHandler::Update()
	{
		if (Input::GetButtonDown("ShowLog"))
		{

			Vector2 mp = Input::GetMousePosition();

			Debug::LogFormat("AppHandler", nullptr, "X = %f Y = %f", mp.x, mp.y);

			//float mouseX = Input::GetAxis("Mouse X");
			//float mouseY = Input::GetAxis("Mouse Y");
			//float alphaNum = Input::GetAxis("AlphaNum");
			//
			//Debug::LogFormat("AppHandler", nullptr, "Mouse X = %f, Mouse Y = %f, AlphaNum = %f", mouseX, mouseY, alphaNum);
		}

		Vector2 mousePosition = Input::GetMousePosition();

		float deltaX = Input::GetAxis("AlphaNum");

		bool log = deltaX != 0.0f; // mousePosition != m_MousePosition;
		if (log)
		{
			Debug::LogFormat("AppHandler", nullptr, "Delta X = %f", deltaX);
		}
		m_MousePosition = mousePosition;
	}
}