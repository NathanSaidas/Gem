#include "G_Key.h"
#include "../Reflection/G_Reflection.h"
#include "../Window/G_WindowManager.h"
#include <GLFW\glfw3.h>

#include "../Utilities/G_Utilities.h"

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

    void Key::update()
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
    void Key::onEvent(int aAction)
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
    bool Key::isDown()
    {
        if(m_CurrentState == ButtonState::DOWN || m_CurrentState == ButtonState::PRESS)
        {
            return true;
        }
        return false;
    }
    bool Key::isUp()
    {
        if(m_CurrentState == ButtonState::UP || m_CurrentState == ButtonState::RELEASE)
        {
            return true;
        }
        return false;
    }
    ButtonState Key::currentState()
    {
        return m_CurrentState;
    }

    Pointer<Reflection::Type> Key::getType()
    {
        return typeOf("Key");
    }

}