#include "G_WindowHook.h"
#include "G_Window.h"
#include "G_WindowManager.h"
#include "../Input/G_Input.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Time.h"
#include "../Renderer/G_Camera.h"
#include "../Utilities/G_Utilities.h"
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
#include "../Utilities/G_Debug.h"
#include "../Testing/G_TestObj.h"

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

		if (Input::Instance()->GetKeyDown(KeyCode::A))
		{
			TestObj * objA = Memory::Instantiate<TestObj>();
			TestObj * objB = nullptr;
			TestObj objC;
			int testObjSize = sizeof(TestObj);
			int reflectedSize = objA->GetType()->size();
			string formatted = string("Object: ") + Utilities::I2S(testObjSize) + "\nReflected:" + Utilities::I2S(reflectedSize);
			Debug::Log(formatted.C_Str() , __FILE__, __LINE__);

			objA->floatingData = 45.0f;
			objA->operation = 20;
			objA->someData = 10.0f;
			objA->someName = "This text changed";
			objA->stdSomeName = "Not the same";
			SomeUnionThing data;
			data.x = 0.5f;
			data.y = 32.0f;
			data.z = -123.0f;
			data.dataA = -2; // what am i doing to that poor z...
			objA->unionGuy = data;
			
			const int TEST_OBJ_SIZE = sizeof(TestObj);

			
			//unsigned char buffer[TEST_OBJ_SIZE];
			//memset(buffer, 0, TEST_OBJ_SIZE);
			//memcpy(buffer, objA, TEST_OBJ_SIZE);
			sbyte * serializedBytes = nullptr;//Utilities::Serialize<TestObj>(objA);
			FILE * file = fopen("..\\..\\Assets\\SomeTextFile.txt", "rb");
			//fwrite(serializedBytes, sizeof(sbyte), serializedBytes[0] + 4, file);

			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			rewind(file);
			serializedBytes = new char[size];
			fread(serializedBytes, 1, size, file);
			//fread(serializedBytes, sizeof(sbyte),)
			fclose(file);
			objB = Utilities::Deserialize<TestObj>(serializedBytes);
			//memcpy(&objC, buffer, TEST_OBJ_SIZE);


			
			Memory::Destroy<TestObj>(objA);
		}
		
		
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
		Graphics::BackgroundColor(255.0f / 255.0f, 99.0f / 255.0f, 71.0f / 255.0f);
		Graphics::EnableBlending();
		m_Camera = Memory::Instantiate<Camera>();
		m_Camera->Position(ClassUtils::Vector3(0.0f,0.0f,0.0f));
		m_Camera->Orientation(ClassUtils::Vector3(0.0f, 1.0f, 0.0f));
		m_Camera->Target(ClassUtils::Vector3(0.0f, 0.0f, 1.0f));
		m_Camera->Name(string("Main Camera"));
		m_Camera->SetFBO(windowSize.x, windowSize.y);
		m_Camera->SetProjection(60.0f, windowSize.x / windowSize.y, 0.1f, 100.0f);
		Graphics::AddCamera(m_Camera);
		m_SimpleBlur = Memory::Instantiate<Shader>();
		m_SimpleBlur->Load(string("SimpleBlur"));
		Graphics::DefaultShader(m_SimpleBlur);
		m_Plane = Memory::Instantiate<Mesh>();
		m_Plane->Load(string("..\\..\\Assets\\Models\\Plane.obj"));
		m_Plane->Upload();
		Graphics::DefaultMesh(m_Plane);
		Graphics::FrameBufferObject()->Setup((int)windowSize.x, (int)windowSize.y);
#ifdef WIN32
		system("cls");
#endif
		if (m_Plane->IsStatic() && m_SimpleBlur->IsLoaded())
		{
			printf("Load Successful");
		}
    }

    void WindowHook::OnDetachFromWindow(int aHandle)
    {
        printf("Detached to window %d\n", aHandle);

		while (m_Objects.size() > 0)
		{
			MemoryHandle<RenderableObject> obj = m_Objects.front();
			m_Objects.erase(m_Objects.begin());
			obj = Memory::DestroyHandle<RenderableObject>(obj);
		}

		Graphics::RemoveCamera(m_Camera->Name());
		Graphics::DefaultShader(nullptr);
		Graphics::DefaultMesh(nullptr);

		m_Camera = Memory::DestroyHandle<Camera>(m_Camera);
		m_SimpleBlur = Memory::DestroyHandle<Shader>(m_SimpleBlur);
		m_Plane = Memory::DestroyHandle<Mesh>(m_Plane);
    }
	void WindowHook::OnWindowResize(float aWidth, float aHeight)
	{
		glViewport(0, 0, (GLsizei)aWidth, (GLsizei)aHeight);
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