#include "Scene.h"

#include "GameObject.h"
#include "../Memory/Memory.h"
#include "Errors\EntityComponentErrors.h"

using namespace Gem::Debugging;


namespace Gem
{
    
        const Func<void, GameObject*> PRE_RENDER_FUNC     = [](GameObject* gameObject){gameObject->SendMessage<int>("InternalOnRenderUpdate", 0); };
        const Func<void, GameObject*> RENDER_FUNC         = [](GameObject* gameObject){gameObject->SendMessage<int>("InternalOnRenderUpdate", 1); };
        const Func<void, GameObject*> POST_RENDER_FUNC    = [](GameObject* gameObject){gameObject->SendMessage<int>("InternalOnRenderUpdate", 2); };
        const Func<void, GameObject*> UPDATE_FUNC         = [](GameObject* gameObject){gameObject->SendMessage<int>("InternalOnUpdate", 0); };
        const Func<void, GameObject*> POST_UPDATE_FUNC    = [](GameObject* gameObject){gameObject->SendMessage<int>("InternalOnUpdate", 1); };
        const Func<void, GameObject*> UPDATE_STATE_FUNC   = [](GameObject* gameObject){gameObject->SendMessage<>("InternalOnUpdateState"); };

    	RDEFINE_CLASS(Scene, Object)

		const UInt32 Scene::MAX_GAME_OBJECTS = 65535;

		Scene::Scene()
			: m_SceneMap(MAX_GAME_OBJECTS)
		{
			
		}
		

		Scene::~Scene()
		{
			std::vector<GameObject*> children = m_SceneGraph.m_Top->m_Children;
			for (int i = 0; i < children.size(); i++)
			{
				Memory::AllocatorType type = Memory::MemoryUtils::GetAllocatorType(children[i]);
				switch (type)
				{
				case Memory::AllocatorType::Pool:
					MEM_POOL_DEALLOC_T(children[i], GameObject);
					break;
				case Memory::AllocatorType::Stack:

					break;
				}
			}
			m_SceneMap.Clear();
		}


		void Scene::Register(GameObject * aGameObject)
		{
			m_SceneGraph.Insert(aGameObject);

			if (m_SceneGraph.m_Top == aGameObject)
			{
				return;
			}
			m_SceneMap.InsertElement(aGameObject);
		}
		void Scene::Unregister(GameObject * aGameObject)
		{
			m_SceneGraph.Remove(aGameObject);
			if (m_SceneGraph.m_Top == aGameObject)
			{
				return;
			}
			
			m_SceneMap.RemoveElement(aGameObject);
		}

		void Scene::OnGameObjectWillChangeName(GameObject* aGameObject)
		{
			m_SceneMap.RemoveElement(aGameObject);
		}
		void Scene::OnGameObjectChangedName(GameObject* aGameObject)
		{
			m_SceneMap.InsertElement(aGameObject);
		}

		void Scene::OnGameObjectWillChangeParent(GameObject* aGameObject)
		{
			m_SceneGraph.Remove(aGameObject);
		}
		void Scene::OnGameObjectChangedParent(GameObject* aGameObject)
		{
			m_SceneGraph.Insert(aGameObject);
		}

		GameObject* Scene::GetRootGameObject()
		{
			return m_SceneGraph.m_Top;
		}

		std::vector<GameObject*> Scene::GetRootGameObjects()
		{
			if (m_SceneGraph.m_Top == nullptr)
			{
				SceneGraphNotInitialized("m_SceneGraph.m_Top", GET_TRACE(0), "Scene::GetRootGameObjects").Log("Gem");
				return std::vector<GameObject*>();
			}
			return m_SceneGraph.m_Top->m_Children;
		}


		void Scene::Save()
		{
			//Mark All GameObjects



		}

		GameObject* Scene::Find(const std::string & aName)
		{
			return m_SceneMap.Find(aName);
		}
		Array<GameObject*> Scene::FindAll(const std::string & aName)
		{
			return m_SceneMap.FindAll(aName);
		}
		Array<Component*> Scene::FindAllOfType(const std::string & aTypename)
		{
			std::vector<Component*> components;
			GameObjectMap::Iterator it = m_SceneMap.GetIterator();
			while (it.MoveNext())
			{
				GameObject* gameObject = *it;
				if (gameObject != nullptr)
				{
					std::vector<Component*> foundComponents = gameObject->GetComponents(aTypename);
					if (foundComponents.size() != 0)
					{
						components.insert(components.end(), foundComponents.begin(), foundComponents.end());
					}
				}
			}
			Array<Component*> result;
			Array<Component*>::Copy(components, result);
			return result;
		}
		Array<Component*> Scene::FindAllOfType(Type & aType)
		{
			std::vector<Component*> components;
			GameObjectMap::Iterator it = m_SceneMap.GetIterator();
			while (it.MoveNext())
			{
				GameObject* gameObject = *it;
				if (gameObject != nullptr)
				{
					std::vector<Component*> foundComponents = gameObject->GetComponents(aType);
					if (foundComponents.size() != 0)
					{
						components.insert(components.end(), foundComponents.begin(), foundComponents.end());
					}
				}
			}
			Array<Component*> result;
			Array<Component*>::Copy(components, result);
			return result;
		}

		UInt32 Scene::HashString(const std::string & string)
		{
			return m_SceneMap.HashString(string);
		}

		void Scene::Update()
		{
            m_SceneMap.Foreach(UPDATE_STATE_FUNC);
            m_SceneMap.Foreach(UPDATE_FUNC);
            m_SceneMap.Foreach(POST_UPDATE_FUNC);
		}
		void Scene::FixedUpdate()
		{
			//GameObject * top = m_SceneGraph.m_Top;
			//top->BroadcastMessage<int>("InternalOnPhysicsUpdate", 0);
			//top->BroadcastMessage<int>("InternalOnRenderUpdate", 0);
			//top->BroadcastMessage<int>("InternalOnRenderUpdate", 1);
			//top->BroadcastMessage<int>("InternalOnRenderUpdate", 2);
		}
		void Scene::PreRender()
		{
            m_SceneMap.Foreach(PRE_RENDER_FUNC);
		}
		void Scene::Render()
		{
            m_SceneMap.Foreach(RENDER_FUNC);
		}
		void Scene::PostRender()
		{
            m_SceneMap.Foreach(POST_RENDER_FUNC);
		}

		

		void Scene::OnWindowFocus(Window * aWindow)
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<Window*>("OnWindowFocus", aWindow);
		}
		void Scene::OnWindowUnfocus(Window * aWindow)
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<Window*>("OnWindowUnfocus", aWindow);
		}
		void Scene::OnWindowClose(Window * aWindow)
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<Window*>("OnWindowClose", aWindow);
		}
		void Scene::OnWindowChangeSize(Window * aWindow, int aWidth, int aHeight)
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<Window*,int,int>("OnWindowClose", aWindow, aWidth, aHeight);
		}

	
}