#ifndef GEM_TEST_COMPONENT_H
#define GEM_TEST_COMPONENT_H

#include "Component.h"
#include "../Engine.h"

namespace Gem
{
	class TestComponent : public Component
	{
		RDECLARE_CLASS(TestComponent)
	public:
		TestComponent();
		~TestComponent();
	protected:
		void OnRegister();
		void OnInitialize();
		void OnLateInitialize();
		
		void OnDestroy();
		void OnLateDestroy();
		
		void Update();
		void LateUpdate();
		void FixedUpdate();
		
		void OnPreRender();
		void OnRender();
		void OnPostRender();

	private:
		Renderer * m_Renderer = nullptr;
		Camera * m_Camera = nullptr;

		Pointer<Mesh> m_Mesh;
		Pointer<Material> m_Material;

		GameObject * m_Light;
		GameObject * m_LightTarget;

		GameObject * m_Ground;
		GameObject * m_Wall;

		float m_FieldOfView;

	};


	TYPE_DEFINE(TestComponent)

}

#endif