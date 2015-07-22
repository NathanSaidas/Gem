#include "AppHandler.h"
#include "ComponentZero.h"
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
		"Freddy Williams"
	};

	RDEFINE_CLASS(AppHandler, BaseAppHandler)

	AppHandler::AppHandler()
		: m_MTE()
	{
		m_MTE.seed(time(nullptr));
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
		Input::CreateButton("Load", KeyCode::L);
	}

	void AppHandler::Update()
	{
		if (Input::GetButtonDown("Create"))
		{
			GameObject * gameObject = MEM_POOL_ALLOC_T(GameObject, RANDOM_NAMES[RandomRange(0,4)]);
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

			ComponentZero* componentZero = dynamic_cast<ComponentZero*>(gameObject->AddComponent("ComponentZero"));
			if (componentZero != nullptr)
			{
				componentZero->SetDamage(RandomRange(5.0f, 350.0f));
				componentZero->SetHealth(RandomRange(4300.0f, 6300.0f));
				componentZero->SetLevel(RandomRange(40, 45));
			}
		}
		if (Input::GetButtonDown("Find"))
		{
			std::string searchname = RANDOM_NAMES[RandomRange(0,4)];
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

		if (Input::GetButtonDown("Load"))
		{
			try
			{
				SceneFile sceneFile;
				sceneFile.LoadScene("SceneSaveTest.txt");
				Debug::Log("AppHandler", "Successfully loaded scene");

				Scene * scene = Application::GetCurrentScene();
				if (scene != nullptr)
				{
					for (int i = 0; i < 5; i++)
					{
						Array<GameObject*> gameobjects = scene->FindAll(RANDOM_NAMES[i]);
						for (int j = 0; j < gameobjects.GetCount(); j++)
						{
							ComponentZero * componentZero = gameobjects[j]->GetComponent<ComponentZero>();
							if (componentZero != nullptr)
							{
								Debug::LogFormat("AppHandler", nullptr, "Found a gameobject with the name %s\n%s\nHealth = %f\nDamage = %f\nLevel = %d\n%s",
									componentZero->GetName().c_str(),
									"=========== STATS ==============",
									componentZero->GetHealth(),
									componentZero->GetDamage(),
									componentZero->GetLevel(),
									"=========== STATS ==============");
							}
						}
					}
				}
				

			}
			catch (...)
			{
				Debug::Log("AppHandler", "Failed to load scene");
			}
				
		}
		
	}


	float AppHandler::RandomRange(float aMin, float aMax)
	{
		m_Float = std::uniform_real_distribution<float>(aMin, aMax);
		return m_Float(m_MTE);
	}
	int AppHandler::RandomRange(int aMin, int aMax)
	{
		m_Int = std::uniform_int_distribution<int>(aMin, aMax);
		return m_Int(m_MTE);
	}
}