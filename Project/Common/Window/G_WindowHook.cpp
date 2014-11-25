#include "G_WindowHook.h"
#include "G_Window.h"
#include "G_WindowManager.h"
#include "../Input/G_Input.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Time.h"
#include "../Renderer/G_Camera.h"
#include "../Utilities/Utilities.h"
#include "G_Application.h"
///Graphics Testing Includes
#include "../Renderer/G_Graphics.h"
#include "../Renderer/G_Mesh.h"
#include "../Renderer/G_Texture.h"
#include "../Renderer/G_VertexInfo.h"
#include "../Renderer/G_Color.h"
#include "../Renderer/G_Shader.h"
#include "../Renderer/G_DualMesh.h"
#include "../Testing/G_RenderableObject.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Gem
{
    using namespace Reflection;

    WindowHook::WindowHook() :Object()
    {
		
        m_Window = nullptr;
    }
    WindowHook::~WindowHook()
    {
        if(m_Window != nullptr)
        {
            WindowManager::Instance()->DetachHook(this,m_Window->Handle());
            
        }
    }

    void WindowHook::Update()
    {
        if(m_Window == nullptr)
        {
            return;
        }

        if(IsFocused() == false)
        {
            return;
        }

		if (Input::Instance()->GetKey(KeyCode::UP))
		{
			//m_Scale += Time::DeltaTime();
			//m_Scale = Math::Clamp01(m_Scale);
			m_Position.Set(m_Position.x, m_Position.y, m_Position.z + 2.0f * Time::DeltaTime());
			m_Camera->Position(m_Position);
		}
		if (Input::Instance()->GetKey(KeyCode::DOWN))
		{
			//m_Scale -= Time::DeltaTime();
			//m_Scale = Math::Clamp01(m_Scale);
			m_Position.Set(m_Position.x, m_Position.y, m_Position.z - 2.0f * Time::DeltaTime());
			m_Camera->Position(m_Position);
		}
		if (Input::Instance()->GetKey(KeyCode::LEFT))
		{
			m_Position.Set(m_Position.x - 2.0f * Time::DeltaTime(), m_Position.y, m_Position.z);
			m_Camera->Position(m_Position);
		}
		if (Input::Instance()->GetKey(KeyCode::RIGHT))
		{
			m_Position.Set(m_Position.x + 2.0f * Time::DeltaTime(), m_Position.y, m_Position.z);
			m_Camera->Position(m_Position);
		}
		if (Input::Instance()->GetKeyDown(KeyCode::Q))
		{
			system("cls");
			printf("Position: x(%f) y(%f) z(%f)\n", m_Position.x, m_Position.y, m_Position.z);
			printf("Look At: x(%f) y(%f) z(%f)\n", m_Position.x, m_Position.y, m_Position.z + 1.0f);
			printf("Blur Direction: x(%f) y(%f)\n", m_Camera->blurDirection.x, m_Camera->blurDirection.y);
			printf("Samples: %d\n", m_Camera->samples);
		}
		if (Input::Instance()->GetKey(KeyCode::W))
		{
			m_Camera->blurDirection.x += 0.1f * Time::DeltaTime();
			m_Camera->blurDirection.y += 0.1f * Time::DeltaTime();
		}
		if (Input::Instance()->GetKey(KeyCode::S))
		{
			m_Camera->blurDirection.x -= 0.1f * Time::DeltaTime();
			m_Camera->blurDirection.y -= 0.1f * Time::DeltaTime();
		}
		if (Input::Instance()->GetKeyDown(KeyCode::D))
		{
			m_Camera->samples += 1;
		}
		if (Input::Instance()->GetKeyDown(KeyCode::A))
		{
			m_Camera->samples -= 1;
		}
		
		for (std::vector<MemoryHandle<RenderableObject>>::iterator it = m_Objects.begin();
			it != m_Objects.end();
			++it)
		{
			(*it)->OnUpdate();
			Graphics::RenderObject(*it,string(RENDER_PASS_NORMAL));
		}
		//Graphics::Clear();
		//m_Obj->TempRender(m_Camera);
		m_Camera->Position(m_Position);
		m_Camera->Target(m_Position + ClassUtils::Vector3(0.0f, 0.0f, 1.0f));
		
    }
    bool WindowHook::IsFocused()
    {
        if(m_Window == nullptr)
        {
            return false;
        }
        return m_Window->Handle() == WindowManager::Instance()->GetFocusedWindow();
    }
	Vector2 WindowHook::GetWindowSize()
	{
		if (m_Window != nullptr)
		{
			return Vector2(m_Window->Width(), m_Window->Height());
		}
		return Vector2(0.0f, 0.0f);
	}
    void WindowHook::OnAttachToWindow(int aHandle)
    {
        printf("Attached to window %d\n", aHandle);
		//Setup context
		Vector2 windowSize = GetWindowSize();
		glViewport(0, 0, windowSize.x, windowSize.y);
		Graphics::BackgroundColor(255.0f / 255.0f, 99.0f / 255.0f, 71.0f / 255.0f);
		Graphics::EnableBlending();
		m_RenderMode = 0;
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		//Resource Init
		m_Mesh = Memory::Instantiate<Mesh>();
		m_Texture = Memory::Instantiate<Texture>();
		m_Shader = Memory::Instantiate<Shader>();
		m_SimpleBlur = Memory::Instantiate<Shader>();
		m_Plane = Memory::Instantiate<Mesh>();
		if (m_Texture.Memory() != nullptr)
		{
			string filepath = "..\\..\\Assets\\Textures\\Rock.png";
			m_Texture->Load(filepath);
			m_Texture->Upload();
		}
		if (m_Shader.Memory() != nullptr)
		{
			ShaderSource source;
			string shaderName = "Shader";
			m_Shader->Load(shaderName, source);
		}
		if (m_Mesh.Memory() != nullptr)
		{
			string filepath = "..\\..\\Assets\\Models\\Rock.obj";
			m_Mesh->Load(filepath);
			m_Mesh->Upload();
		}
		if (m_SimpleBlur.Memory() != nullptr)
		{
			//string shaderName = "DirectionalBlur";
			string shaderName = "SimpleBlur";
			m_SimpleBlur->Load(shaderName);
		}
		if (m_Plane.Memory() != nullptr)
		{
			string filepath = "..\\..\\Assets\\Models\\Plane.obj";
			m_Plane->Load(filepath);
			m_Plane->Upload();
		}
		//Create Renderables
		m_Position = ClassUtils::Vector3(0.0f, 0.0f, -5.0f);
		m_Camera = Memory::Instantiate<Camera>();
		m_Camera->Position(m_Position);
		m_Camera->Orientation(ClassUtils::Vector3(0.0f, 1.0f, 0.0f));
		m_Camera->Target(m_Position + ClassUtils::Vector3(0.0f, 0.0f, 1.0f));
		m_Camera->Name(string("Main Camera"));
		m_Camera->SetFBO(windowSize.x, windowSize.y);
		m_Camera->SetProjection(60.0f, windowSize.x / windowSize.y, 0.1f, 100.0f);
		Graphics::AddCamera(m_Camera);

		RenderableObject * obj = nullptr;
		
		obj = AddObject(m_Mesh, m_Texture, m_Shader);
		obj->Position(ClassUtils::Vector3(0.0f, 0.0f, 1.0f));
		obj->Scale(ClassUtils::Vector3(1.0f, 1.0f, 1.0f));
		obj->Rotation(ClassUtils::Vector3(0.0f, -90.0f, 0.0f));
		obj->blurAmountX = 1.0f;
		//
		obj = AddObject(m_Mesh, m_Texture, m_Shader);
		obj->Position(ClassUtils::Vector3(0.0f, 0.0f, 0.0f));
		obj->Scale(ClassUtils::Vector3(1.0f, 1.0f, 1.0f));
		obj->Rotation(ClassUtils::Vector3(0.0f, -90.0f, 0.0f));
		obj->RenderTag(string(RENDER_TAG_TRANSPARENT));
		m_Obj = obj;
		//
		obj = AddObject(m_Mesh, m_Texture, m_Shader);
		obj->Position(ClassUtils::Vector3(4.0f, 0.0f, 0.0f));
		obj->Scale(ClassUtils::Vector3(1.0f, 1.0f, 1.0f));
		obj->Rotation(ClassUtils::Vector3(0.0f, 0.0f, 0.0f));
		//
		obj = AddObject(m_Mesh, m_Texture, m_Shader);
		obj->Position(ClassUtils::Vector3(-2.0f, 0.0f, 0.0f));
		obj->Scale(ClassUtils::Vector3(1.0f, 1.0f, 1.0f));
		obj->Rotation(ClassUtils::Vector3(0.0f, 0.0f, 0.0f));
		obj = AddObject(m_Mesh, m_Texture, m_Shader);
		//
		obj->Position(ClassUtils::Vector3(-4.0f, 0.0f, 0.0f));
		obj->Scale(ClassUtils::Vector3(1.0f, 1.0f, 1.0f));
		obj->Rotation(ClassUtils::Vector3(0.0f, 0.0f, 0.0f));

		Graphics::DefaultShader(m_SimpleBlur);
		Graphics::DefaultMesh(m_Plane);
		Graphics::FrameBufferObject()->Setup(windowSize.x, windowSize.y);
    }

    void WindowHook::OnDetachFromWindow(int aHandle)
    {
        printf("Detached to window %d\n", aHandle);
		m_Mesh = Memory::DestroyHandle<Mesh>(m_Mesh);
		m_Texture = Memory::DestroyHandle<Texture>(m_Texture);
		m_Shader = Memory::DestroyHandle<Shader>(m_Shader);

		while (m_Objects.size() > 0)
		{
			MemoryHandle<RenderableObject> obj = m_Objects.front();
			m_Objects.erase(m_Objects.begin());
			obj = Memory::DestroyHandle<RenderableObject>(obj);
		}

    }
	void WindowHook::OnWindowResize(float aWidth, float aHeight)
	{
		glViewport(0, 0, aWidth, aHeight);
		//m_Projection = glm::perspective(60.0f, aWidth / aHeight, 1.0f, 100.0f);
		m_Projection.SetIdentity();
		m_Projection.SetPerspective(60.0f, aWidth / aHeight, 1.0f, 100.0f);
	}
    Pointer<Reflection::Type> WindowHook::GetType()
    {
        return typeOf("WindowHook");
    }

	RenderableObject * WindowHook::AddObject(MemoryHandle<Mesh> aMesh, MemoryHandle<Texture> aTexture, MemoryHandle<Shader> aShader)
	{
		if (aMesh == nullptr || aTexture == nullptr || aShader == nullptr)
		{
			return nullptr;
		}
		MemoryHandle<RenderableObject> obj = Memory::Instantiate<RenderableObject>();
		obj->RenderTag(string(RENDER_TAG_OPAQUE));
		obj->Mesh(aMesh);
		obj->Texture(aTexture);
		obj->Shader(aShader);
		m_Objects.push_back(obj);
		return obj;
	}


}