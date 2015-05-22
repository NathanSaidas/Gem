#ifndef GEM_SCENE_H
#define GEM_SCENE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "SceneGraph.h"
#include <vector>


namespace Gem
{
	class GameObject;
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



	private:

		SceneGraph m_SceneGraph;

		void Update();
		void FixedUpdate();
		void PreRender();
		void Render();
		void PostRender();

		void OnWindowFocus(Window * aWindow);
		void OnWindowUnfocus(Window * aWindow);
		void OnWindowClose(Window * aWindow);
		void OnWindowChangeSize(Window * aWindow, int aWidth, int aHeight);

		void Insert(GameObject * aGameObject);
		void Remove(GameObject * aGameObject);

		void Register(GameObject * aGameObject);
		void Unregister(GameObject * aGameObject);

		friend Application;
		friend class GameObject;
		friend Graphics;

	};

	TYPE_DEFINE(Scene)
}

#endif