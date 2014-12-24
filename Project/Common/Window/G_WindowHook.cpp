#include "G_WindowHook.h"
#include "G_Window.h"
#include "G_WindowManager.h"
#include "../Input/G_Input.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Time.h"
#include "../Utilities/G_Utilities.h"
#include "G_Application.h"
///Graphics Testing Includes
#include <glm\gtc\matrix_transform.hpp>
#include "../Utilities/G_Debug.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "../Testing/G_GameObject.h"

namespace Gem
{
    using namespace Reflection;
	G_CLASS_IMPLEMENTATION(WindowHook,object)
    WindowHook::WindowHook() :object()
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
		
		TestTemplate<int> intTemplate;
		TestTemplate<float> floatTemplate;
		TestTemplate<Rect> rectTemplate;

		Type intType = intTemplate.GetType();
		Type floatType = floatTemplate.GetType();
		Type rectType = rectTemplate.GetType();


    }

    void WindowHook::OnDetachFromWindow(int aHandle)
    {
        printf("Detached to window %d\n", aHandle);
    }
	void WindowHook::OnWindowResize(float aWidth, float aHeight)
	{
		glViewport(0, 0, (GLsizei)aWidth, (GLsizei)aHeight);
	}



}