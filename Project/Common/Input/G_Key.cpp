#include "G_Key.h"
#include "../Reflection/G_Reflection.h"
#include "../Window/G_WindowManager.h"
#include <GLFW\glfw3.h>
namespace Gem
{
    using namespace Reflection;

    Key::Key()
    {
        m_CurrentState = ButtonState::UP;
    }
    Key::~Key()
    {

    }

    void Key::Update()
    {
        if(m_CurrentState == ButtonState::PRESS)
        {
            m_CurrentState = ButtonState::DOWN;
        }
        else if(m_CurrentState == ButtonState::RELEASE)
        {
            m_CurrentState = ButtonState::UP;
        }
    }
    void Key::OnEvent(int aAction)
    {
        if(aAction == GLFW_PRESS)
        {
            m_CurrentState = ButtonState::PRESS;
        }
        else if(aAction == GLFW_RELEASE)
        {
            m_CurrentState = ButtonState::RELEASE;
        }
    }
    bool Key::IsDown()
    {
        if(m_CurrentState == ButtonState::DOWN || m_CurrentState == ButtonState::PRESS)
        {
            return true;
        }
        return false;
    }
    bool Key::IsUp()
    {
        if(m_CurrentState == ButtonState::UP || m_CurrentState == ButtonState::RELEASE)
        {
            return true;
        }
        return false;
    }
    ButtonState Key::CurrentState()
    {
        return m_CurrentState;
    }

    Pointer<Reflection::Type> Key::GetType()
    {
        return typeOf("Key");
    }

}