#include "Scene.h"

#include "GameObject.h"
#include "../Memory/Memory.h"

using namespace Gem::Debugging;


namespace Gem
{
	RDEFINE_CLASS(Scene, Object)

		Scene::Scene()
		{

		}
		Scene::~Scene()
		{
			//Scene Graph Destroys the Top GameObject,  therefore destroying every other gameobject created.
		}

		void Scene::Register(GameObject * aGameObject)
		{
			m_SceneGraph.Insert(aGameObject);
		}
		void Scene::Unregister(GameObject * aGameObject)
		{
			m_SceneGraph.Remove(aGameObject);
		}

		void Scene::Save()
		{
			//Mark All GameObjects



		}


		void Scene::Update()
		{
			GameObject * top = m_SceneGraph.m_Top;

			top->BroadcastMessage<>("InternalOnUpdateState");
			top->BroadcastMessage<int>("InternalOnUpdate", 0);
			top->BroadcastMessage<int>("InternalOnUpdate", 1);
		}
		void Scene::FixedUpdate()
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<int>("InternalOnPhysicsUpdate", 0);
			top->BroadcastMessage<int>("InternalOnRenderUpdate", 0);
			top->BroadcastMessage<int>("InternalOnRenderUpdate", 1);
			top->BroadcastMessage<int>("InternalOnRenderUpdate", 2);
		}
		void Scene::PreRender()
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<int>("InternalOnRenderUpdate", 0);
		}
		void Scene::Render()
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<int>("InternalOnRenderUpdate", 1);
		}
		void Scene::PostRender()
		{
			GameObject * top = m_SceneGraph.m_Top;
			top->BroadcastMessage<int>("InternalOnRenderUpdate", 2);
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

		void Scene::Insert(GameObject * aGameObject)
		{
			m_SceneGraph.Insert(aGameObject);
		}
		void Scene::Remove(GameObject * aGameObject)
		{
			m_SceneGraph.Remove(aGameObject);
		}
	
}