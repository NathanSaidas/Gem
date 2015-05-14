#ifndef GEM_CAMERA_H
#define GEM_CAMERA_H

#pragma region
/*

*/
#pragma endregion

#include "Component.h"
#include "../Math/Math.h"
#include "../Utilities/Utilities.h"

namespace Gem
{

	class Camera;
	template class GEM_API Reflection::MetaObject<Camera>;

	template class GEM_API std::vector<Camera*>;
	template class GEM_API Array<Camera>;

	///This component will invoke render calls.
	class GEM_API Camera : public Component
	{
		RDECLARE_CLASS(Camera)
	public:
		Camera();
		~Camera(); 

		void Update();
		void OnPostRender();
		///Will register to the graphics engine to render the scene.
		void Render();
		Matrix4x4 GetViewMatrix();
		Matrix4x4 GetProjectionMatrix();

		void SetViewport(Float32 x, Float32 y, Float32 aWidth, Float32 aHeight);
		void GetViewport(Float32 & x, Float32 & y, Float32 & aWidth, Float32 & aHeight);

		void SetFieldOfView(Float32 aFieldOfView);
		Float32 GetFieldOfView();

		void SetSize(Float32 aSize);
		Float32 GetSize();
		
		void SetNear(Float32 aNear);
		Float32 GetNear(); 

		void SetFar(Float32 aFar);
		Float32 GetFar();

		void SetOrtho();
		bool IsOrtho();

		void SetPerspective();
		bool IsPerspective();

		bool IsRegistered();

        Vector2 WorldToScreenSpace(const Vector3 & aPosition);

		static Array<Camera*> GetCameras();

	private:
		Float32 m_ViewX;
		Float32 m_ViewY;
		Float32 m_ViewWidth;
		Float32 m_ViewHeight;
		Float32 m_FieldOfView;
		Float32 m_Size;


		Float32 m_Near;
		Float32 m_Far;
		bool m_IsOrtho;

		bool m_IsRegistered;

		static std::vector<Camera*> s_Cameras;
		static void AddCamera(Camera * aCamera);
		static void RemoveCamera(Camera * aCamera);
	
	};

	TYPE_DEFINE(Camera)

}


#endif