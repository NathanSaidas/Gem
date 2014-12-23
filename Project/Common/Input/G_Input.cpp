#include "G_Input.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "../Window/G_WindowManager.h"
#include "G_InputAxis.h"
#include <GLFW\glfw3.h>
namespace Gem
{
    using namespace Reflection;
	G_CLASS_IMPLEMENTATION(Input,object)
    Input * Input::s_Instance = nullptr;
    Input * Input::Instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::Instantiate<Input>();
        }
        return s_Instance;
    }
    void Input::Destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::Destroy<Input>(s_Instance);
        }
    }

    Input::Input()
    {
        //For use when allocating memory dynamically.
        //However there is a weird memory glitch.
        //m_Keys = Memory::instantiate<Key*>();
        //m_MouseButtons = Memory::instantiate<Button*>();

        for(int i = 0; i < (int)KeyCode::COUNT; i++)
        {
            m_Keys.push_back(Memory::Instantiate<Key>());
        }
        for(int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            m_MouseButtons.push_back(Memory::Instantiate<Button>());
        }
        m_CurrentWindowID = -1;
    }
    Input::~Input()
    {
        for(int i = 0; i < (int)KeyCode::COUNT; i++)
        {
            m_Keys[i] = Memory::Destroy<Key>(m_Keys[i]);
        }
        for(int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            m_MouseButtons[i] = Memory::Destroy<Button>(m_MouseButtons[i]);
        }

        m_Keys.clear();
        m_MouseButtons.clear();
        //For use when allocating memory dynamically.
        //However there is a weird memory glitch.
        //m_Keys = Memory::Destroy<Key*>(m_Keys,(int)KeyCode::COUNT);
        //m_MouseButtons = Memory::Destroy<Button*>(m_MouseButtons,(int)MouseButton::COUNT);

        for(int i = 0; i < m_Axis.size(); i++)
        {
            m_Axis[i] = Memory::Destroy<InputAxis>(m_Axis[i]);
        }
        m_Axis.clear();
    }

    //int Input::GetKeyState(KeyCode aKeyCode)
    //{
    //    GLFWwindow * currentWindow = WindowManager::Instance()->GetCurrentWindow();
    //    if(currentWindow == nullptr)
    //    {
    //        return -1;
    //    }
    //    return 0;
    //
    //}

    void Input::Update()
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i] != nullptr)
            {
                m_Axis[i]->Update();
            }
        }

        for(int i = 0; i < (int)KeyCode::COUNT; i++)
        {
            if(m_Keys[i] != nullptr)
            {
                m_Keys[i]->Update();
            }
        }
        for(int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            if(m_MouseButtons[i] != nullptr)
            {
                m_MouseButtons[i]->Update();
            }
        }

        m_MouseScrollWheel.x = 0.0f;
        m_MouseScrollWheel.y = 0.0f;

        int currentWindow = WindowManager::Instance()->GetFocusedWindow();
        if(currentWindow != m_CurrentWindowID)
        {
            //Window focus has changed
        }
        m_CurrentWindowID = currentWindow;
        glfwPollEvents();
    }

    void Input::ProcessKeyEvent(int aKey, int aAction, int aMods)
    {
        if(aKey < 0 || aKey >= (int)KeyCode::COUNT)
        {
            //ignore
            return;
        }
        m_Keys[aKey]->OnEvent(aAction);

    }
    void Input::ProcessMouseEvent(int aButton, int aAction, int aMods)
    {
        if(aButton < 0 || aButton >= (int)MouseButton::COUNT)
        {
            return;
        }
        m_MouseButtons[aButton]->OnEvent(aAction);
    }
    void Input::ProcessMouseMove(float aX, float aY)
    {
        m_LastMousePosition = m_MousePosition;

        m_MousePosition.x = aX;
        m_MousePosition.y = aY;
    }
    //x and y represent direction
    void Input::ProcessMouseScroll(float aX, float aY)
    {
        m_MouseScrollWheel.x = aX;
        m_MouseScrollWheel.y = aY;
    }

    void Input::DestroyAxis(std::string & aName)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                InputAxis * axis = m_Axis[i];
                m_Axis.erase(m_Axis.begin() + i);
                axis = Memory::Destroy<InputAxis>(axis);
                return;
            }
        }
    }
    void Input::CreateAxis(std::string & aName)
    {
        CreateAxis(aName,AxisCode::NONE,AxisCode::NONE);
    }
    void Input::CreateAxis(std::string & aName, AxisCode aPositiveKey, AxisCode aNegativeKey)
    {
        InputAxis * inputAxis = Memory::Instantiate<InputAxis>();
        inputAxis->SetName(aName);
        inputAxis->SetPositiveKey(aPositiveKey,0);
        inputAxis->SetNegativeKey(aNegativeKey,0);
        m_Axis.push_back(inputAxis);
    }
    void Input::SetAxisPositiveKey(std::string & aName, AxisCode aKeyCode, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                m_Axis[i]->SetPositiveKey(aKeyCode,aKey);
                return;
            }
        }
    }
    void Input::SetAxisNegativeKey(std::string & aName, AxisCode aKeyCode, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                m_Axis[i]->SetNegativeKey(aKeyCode,aKey);
                return;
            }
        }
    }
    void Input::SetAxisReSetOnRelease(std::string & aName, bool aReSetflag)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                m_Axis[i]->SetResetOnRelease(aReSetflag);
                return;
            }
        }
    }

    float Input::GetAxis(std::string & aName)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                return m_Axis[i]->AxisValue();
            }
        }
        return 0.0f;
    }
    AxisCode Input::GetAxisPositiveKey(std::string & aName, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                return m_Axis[i]->PositiveKey(aKey);
            }
        }
        return AxisCode::NONE;
    }
    AxisCode Input::GetAxisNegativeKey(std::string & aName, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                return m_Axis[i]->NegativeKey(aKey);
            }
        }
        return AxisCode::NONE;
    }
    bool Input::GetAxisReSetOnRelease(std::string & aName)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->Name() == aName)
            {
                return m_Axis[i]->ReSetOnRelease();
            }
        }
        return true;
    }
  

}