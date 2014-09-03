#include "G_Input.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include <GLFW\glfw3.h>
#include "../Window/G_WindowManager.h"
#include "G_InputAxis.h"

namespace Gem
{
    using namespace Reflection;

    Input * Input::s_Instance = nullptr;
    Input * Input::instance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = Memory::instantiate<Input>();
        }
        return s_Instance;
    }
    void Input::destroy()
    {
        if(s_Instance != nullptr)
        {
            s_Instance = Memory::destroy<Input>(s_Instance);
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
            m_Keys.push_back(Memory::instantiate<Key>());
        }
        for(int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            m_MouseButtons.push_back(Memory::instantiate<Button>());
        }
        m_CurrentWindowID = -1;
    }
    Input::~Input()
    {
        for(int i = 0; i < (int)KeyCode::COUNT; i++)
        {
            m_Keys[i] = Memory::destroy<Key>(m_Keys[i]);
        }
        for(int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            m_MouseButtons[i] = Memory::destroy<Button>(m_MouseButtons[i]);
        }

        m_Keys.clear();
        m_MouseButtons.clear();
        //For use when allocating memory dynamically.
        //However there is a weird memory glitch.
        //m_Keys = Memory::destroy<Key*>(m_Keys,(int)KeyCode::COUNT);
        //m_MouseButtons = Memory::destroy<Button*>(m_MouseButtons,(int)MouseButton::COUNT);

        for(int i = 0; i < m_Axis.size(); i++)
        {
            m_Axis[i] = Memory::destroy<InputAxis>(m_Axis[i]);
        }
        m_Axis.clear();
    }

    //int Input::getKeyState(KeyCode aKeyCode)
    //{
    //    GLFWwindow * currentWindow = WindowManager::instance()->getCurrentWindow();
    //    if(currentWindow == nullptr)
    //    {
    //        return -1;
    //    }
    //    return 0;
    //
    //}

    void Input::update()
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i] != nullptr)
            {
                m_Axis[i]->update();
            }
        }

        for(int i = 0; i < (int)KeyCode::COUNT; i++)
        {
            if(m_Keys[i] != nullptr)
            {
                m_Keys[i]->update();
            }
        }
        for(int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            if(m_MouseButtons[i] != nullptr)
            {
                m_MouseButtons[i]->update();
            }
        }

        m_MouseScrollWheel.x = 0.0f;
        m_MouseScrollWheel.y = 0.0f;

        int currentWindow = WindowManager::instance()->getFocusedWindow();
        if(currentWindow != m_CurrentWindowID)
        {
            //Window focus has changed
        }
        m_CurrentWindowID = currentWindow;
        glfwPollEvents();
    }

    void Input::processKeyEvent(int aKey, int aAction, int aMods)
    {
        if(aKey < 0 || aKey >= (int)KeyCode::COUNT)
        {
            //ignore
            return;
        }
        m_Keys[aKey]->onEvent(aAction);

    }
    void Input::processMouseEvent(int aButton, int aAction, int aMods)
    {
        if(aButton < 0 || aButton >= (int)MouseButton::COUNT)
        {
            return;
        }
        m_MouseButtons[aButton]->onEvent(aAction);
    }
    void Input::processMouseMove(float aX, float aY)
    {
        m_LastMousePosition = m_MousePosition;

        m_MousePosition.x = aX;
        m_MousePosition.y = aY;
    }
    //x and y represent direction
    void Input::processMouseScroll(float aX, float aY)
    {
        m_MouseScrollWheel.x = aX;
        m_MouseScrollWheel.y = aY;
    }

    void Input::destroyAxis(std::string aName)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                InputAxis * axis = m_Axis[i];
                m_Axis.erase(m_Axis.begin() + i);
                axis = Memory::destroy<InputAxis>(axis);
                return;
            }
        }
    }
    void Input::createAxis(std::string aName)
    {
        createAxis(aName,AxisCode::NONE,AxisCode::NONE);
    }
    void Input::createAxis(std::string aName, AxisCode aPositiveKey, AxisCode aNegativeKey)
    {
        InputAxis * inputAxis = Memory::instantiate<InputAxis>();
        inputAxis->setName(aName);
        inputAxis->setPositiveKey(aPositiveKey,0);
        inputAxis->setNegativeKey(aNegativeKey,0);
        m_Axis.push_back(inputAxis);
    }
    void Input::setAxisPositiveKey(std::string aName, AxisCode aKeyCode, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                m_Axis[i]->setPositiveKey(aKeyCode,aKey);
                return;
            }
        }
    }
    void Input::setAxisNegativeKey(std::string aName, AxisCode aKeyCode, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                m_Axis[i]->setNegativeKey(aKeyCode,aKey);
                return;
            }
        }
    }
    void Input::setAxisResetOnRelease(std::string aName, bool aResetflag)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                m_Axis[i]->setResetOnRelease(aResetflag);
                return;
            }
        }
    }

    float Input::getAxis(std::string aName)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                return m_Axis[i]->axisValue();
            }
        }
        return 0.0f;
    }
    AxisCode Input::getAxisPositiveKey(std::string aName, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                return m_Axis[i]->positiveKey(aKey);
            }
        }
        return AxisCode::NONE;
    }
    AxisCode Input::getAxisNegativeKey(std::string aName, int aKey)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                return m_Axis[i]->negativeKey(aKey);
            }
        }
        return AxisCode::NONE;
    }
    bool Input::getAxisResetOnRelease(std::string aName)
    {
        for(int i = 0; i < m_Axis.size(); i++)
        {
            if(m_Axis[i]->name() == aName)
            {
                return m_Axis[i]->resetOnRelease();
            }
        }
        return true;
    }

    Pointer<Reflection::Type> Input::getType()
    {
        return typeOf("Input");
    }
        

}