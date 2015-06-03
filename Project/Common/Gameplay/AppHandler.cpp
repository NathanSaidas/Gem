#include "AppHandler.h"
#include <random>

using namespace Gem::Reflection;
using namespace Gem::Memory;
using namespace Gem::Debugging;


namespace Gem
{

	const std::string RANDOM_NAMES[] =
	{
		"Richard",
		"Wally",
		"YouKnowIt",
		"Ruther",
		"MOTHER FUCKER"
	};

	RDEFINE_CLASS(AppHandler, BaseAppHandler)

	AppHandler::AppHandler()
		: m_MTE(),
		m_Distribution(0, 4)
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
		Input::CreateButton("Create", KeyCode::I);
		Input::CreateButton("Find", KeyCode::F);
		Input::CreateButton("Save", KeyCode::S);
	}

	void AppHandler::Update()
	{
		if (Input::GetButtonDown("Create"))
		{
			GameObject * gameObject = MEM_POOL_ALLOC_T(GameObject, RANDOM_NAMES[m_Distribution(m_MTE)]);
			Debug::LogFormat("AppHandler", nullptr, "Creating gameobject %s", gameObject->GetName().c_str());

			if (gameObject->GetName() == RANDOM_NAMES[2])
			{
				Debug::Log("AppHandler", "Searching for parent");
				Scene * scene = Application::GetCurrentScene();
				GameObject* parent = scene->Find(RANDOM_NAMES[1]);
				if (parent != nullptr)
				{
					Debug::Log("AppHandler", "Found parent");
					gameObject->SetParent(parent);
				}
			}

		}
		if (Input::GetButtonDown("Find"))
		{
			std::string searchname = RANDOM_NAMES[m_Distribution(m_MTE)];
			GameObject * gameObject = Application::GetCurrentScene()->Find(searchname);
			if (gameObject == nullptr)
			{
				Debug::LogFormat("AppHandler", nullptr, "Failed to find gameobject %s", searchname.c_str());
			}
			else
			{
				Debug::LogFormat("AppHandler", nullptr, "Found gameobject %s", searchname.c_str());
			}
		}

		if (Input::GetButtonDown("Save"))
		{
			try
			{
				Scene * scene = Application::GetCurrentScene();
				SceneFile sceneFile;
				sceneFile.SaveScene(scene, "SceneSaveTest.txt");
				Debug::Log("AppHandler", "Successfully saved scene.");
			}
			catch (...)
			{
				Debug::Log("AppHandler", "Failed to save scene");
			}
		}
	}
}