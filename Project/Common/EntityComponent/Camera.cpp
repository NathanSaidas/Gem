#include "Camera.h"
#include "GameObject.h"
#include "../Application/Application.h"
#include "../Graphics/Graphics.h"
#include "../Window/Window.h"

using namespace Gem::Debugging;

namespace Gem
{
	std::vector<Camera*> Camera::s_Cameras = std::vector<Camera*>();

	RDEFINE_CLASS(Camera,Component)
	Camera::Camera() : Component()
	{
		AddCamera(this);

		Window * window = Application::GetDefaultWindow();

		SetViewport(0.0f, 0.0f, (Float32)window->GetWidth(), (Float32)window->GetHeight());
		SetFieldOfView(60.0f);
		SetSize(5.0f);
		SetNear(0.1f);
		SetFar(100.0f);
		SetPerspective();

	}
	Camera::~Camera()
	{
		if (IsRegistered())
		{
			Graphics::UnregisterCamera(this);
			m_IsRegistered = false;
		}
		RemoveCamera(this);
	}

	void Camera::Update()
	{
		Render();
	}
	void Camera::OnPostRender()
	{
		m_IsRegistered = false;
	}
	///Will register to the graphics engine to render the scene.
	void Camera::Render()
	{
		if (!IsRegistered())
		{
			Graphics::RegisterCamera(this);
			m_IsRegistered = true;
		}
	}

    

	Matrix4x4 Camera::GetViewMatrix()
	{
		Quaternion rotation = GetGameObject()->GetRotation();
        Vector3 lookDirection = rotation * Vector3::Forward();
		return Matrix4x4::LookAt(GetGameObject()->GetPosition(),lookDirection);
	}

	Matrix4x4 Camera::GetProjectionMatrix()
	{
		Matrix4x4 matrix = Matrix4x4::Identity();
		Window * defaultWindow = Application::GetDefaultWindow();
		if (defaultWindow == nullptr)
		{
			Debug::Error("Graphics", "Failed to get projection matrix. Missing default window.");
			return matrix;
		}

		Float32 windowWidth = (Float32)defaultWindow->GetWidth();
		Float32 windowHeight = (Float32)defaultWindow->GetHeight();

		if (m_IsOrtho)
		{
			//Calculate Width / Height
			//h = w/h * size => 1.6 * size
			//w = h/w * size => 0.6 * size
			//Left -w/2
			//Right w/2
			//Top h/2
			//Bottom -h/2

			float width = windowWidth / windowHeight * m_Size * 0.5f;
			float height = windowHeight / windowWidth * m_Size * 0.5f;
			matrix = Matrix4x4::Ortho(-width, width, height, -height, m_Far, m_Near);
		}
		else
		{
			float fieldOfView = m_FieldOfView * 3.14f / 180.0f;

			matrix = Matrix4x4::Perspective(fieldOfView, windowWidth / windowHeight, m_Near, m_Far);
		}
		return matrix;
	}

	void Camera::SetViewport(Float32 x, Float32 y, Float32 aWidth, Float32 aHeight)
	{
		m_ViewX = x;
		m_ViewY = y;
		m_ViewWidth = aWidth;
		m_ViewHeight = aHeight;
	}
	void Camera::GetViewport(Float32 & x, Float32 & y, Float32 & aWidth, Float32 & aHeight)
	{
		x = m_ViewX;
		y = m_ViewY;
		aWidth = m_ViewWidth;
		aHeight = m_ViewHeight;
	}

	void Camera::SetFieldOfView(Float32 aFieldOfView)
	{
		m_FieldOfView = aFieldOfView;
	}

	Float32 Camera::GetFieldOfView()
	{
		return m_FieldOfView;
	}

	void Camera::SetSize(Float32 aSize)
	{
		m_Size = aSize;
	}
	Float32 Camera::GetSize()
	{
		return m_Size;
	}

	void Camera::SetNear(Float32 aNear)
	{
		m_Near = aNear;
	}
	Float32 Camera::GetNear()
	{
		return m_Near;
	}

	void Camera::SetFar(Float32 aFar)
	{
		m_Far = aFar;
	}
	Float32 Camera::GetFar()
	{
		return m_Far;
	}

	void Camera::SetOrtho()
	{
		m_IsOrtho = true;
	}
	bool Camera::IsOrtho()
	{
		return m_IsOrtho;
	}

	void Camera::SetPerspective()
	{
		m_IsOrtho = false;
	}

	bool Camera::IsPerspective()
	{
		return !m_IsOrtho;
	}

	bool Camera::IsRegistered()
	{
		return m_IsRegistered;
	}

    Vector2 Camera::WorldToScreenSpace(const Vector3 & aPosition)
    {
        Matrix4x4 projection = GetProjectionMatrix();
        Matrix4x4 view = GetViewMatrix();
        Vector4 clipSpacePos = projection * (view * Vector4(aPosition.x, aPosition.y, aPosition.z, 1.0f));
        Vector3 ndcSpacePos = Vector3(clipSpacePos.x, clipSpacePos.y, clipSpacePos.z) / clipSpacePos.w;
        Vector2 viewSize;
        Vector2 viewOffset;
        GetViewport(viewOffset.x, viewOffset.y, viewSize.x, viewSize.y);
        Vector2 windowSpacePos = ((Vector2(ndcSpacePos.x, ndcSpacePos.y) + 1.0f) / 2.0f) * viewSize + viewOffset;
        return windowSpacePos;
    }

	Array<Camera*> Camera::GetCameras()
	{
		Array<Camera*> cameras;
		Array<Camera*>::Copy(s_Cameras, cameras);
		return cameras;
	}

	void Camera::AddCamera(Camera * aCamera)
	{
		if (!Utilities::Exists<Camera*>(s_Cameras, aCamera))
		{
			s_Cameras.push_back(aCamera);
		}
	}
	
	void Camera::RemoveCamera(Camera * aCamera)
	{
		Utilities::Remove<Camera*>(s_Cameras, aCamera);
	}
}