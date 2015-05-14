#ifndef GEM_SCENE_H
#define GEM_SCENE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include <vector>


namespace Gem
{
	class GameObject;
	class Application;
	class OpenGLWindow;
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


		

		void Register(GameObject * aGameObject);
		void Unregister(GameObject * aGameObject);


		



	private:
		std::vector<GameObject*> m_GameObjects;
		std::vector<GameObject*> m_InitializationQueue;
		std::vector<GameObject*> m_LateInitializationQueue;


		void Update();
		void FixedUpdate();
		void PreRender();
		void Render();
		void PostRender();

		void OnWindowFocus(OpenGLWindow * aWindow);
		void OnWindowUnfocus(OpenGLWindow * aWindow);
		void OnWindowClose(OpenGLWindow * aWindow);
		void OnWindowChangeSize(OpenGLWindow * aWindow, int aWidth, int aHeight);

		friend Application;
		friend GameObject;
		friend Graphics;

	};

	TYPE_DEFINE(Scene)
}

#endif