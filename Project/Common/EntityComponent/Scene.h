#ifndef GEM_SCENE_H
#define GEM_SCENE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "SceneGraph.h"
#include "GameObjectMap.h"
#include <vector>


namespace Gem
{
	class GameObject;
	class Component;
	class Application;
	class Window;
	class Graphics;

	class Scene;
	template class GEM_API Reflection::MetaObject<Scene>;

	template class GEM_API std::vector<GameObject*>;

	class GEM_API Scene : public Object
	{
		RDECLARE_CLASS(Scene)
	public:
		Scene();
		~Scene();

		void Save();

		GameObject* Find(const std::string & aName);
		Array<GameObject*> FindAll(const std::string & aName);
		Array<Component*> FindAllOfType(const std::string & aTypename);
		Array<Component*> FindAllOfType(Type & aType);
	

		UInt32 HashString(const std::string & string);

	private:
		static const UInt32 MAX_GAME_OBJECTS;
		SceneGraph m_SceneGraph;
		GameObjectMap m_SceneMap;

		void Update();
		void FixedUpdate();
		void PreRender();
		void Render();
		void PostRender();

		void OnWindowFocus(Window * aWindow);
		void OnWindowUnfocus(Window * aWindow);
		void OnWindowClose(Window * aWindow);
		void OnWindowChangeSize(Window * aWindow, int aWidth, int aHeight);



		

		//GameObject Scene Specific Events.

		/** This gets called when the gameobject is first created. */
		void Register(GameObject * aGameObject);
		/** This gets called when the gameobject is finally destroyed. */
		void Unregister(GameObject * aGameObject);

		/** Gets called before the gameobjects name is changed. This will remove it from the map because a new hash needs to be created.*/
		void OnGameObjectWillChangeName(GameObject* aGameObject);
		/** This gets called after the gameobjects name is changed. This will insert it back into the map rehashing the gameobject.*/
		void OnGameObjectChangedName(GameObject* aGameObject);

		/** This gets called when the object is about to change it's parent. This will remove it from the scene graph.*/
		void OnGameObjectWillChangeParent(GameObject* aGameObject);
		/** This gets called after the gameobject has changed it's parent. This will add it back to the scene graph*/
		void OnGameObjectChangedParent(GameObject* aGameObject);

		inline bool IsRoot(GameObject* aGameObject) const
		{
			return aGameObject == m_SceneGraph.m_Top;
		}


		/** This method is used by SceneFile to get all the root game object.s*/
		GameObject* GetRootGameObject();
		std::vector<GameObject*> GetRootGameObjects();

		friend Application;
		friend Graphics;
		friend class GameObject;
		friend class SceneFile;


	};

	TYPE_DEFINE(Scene)
}

#endif