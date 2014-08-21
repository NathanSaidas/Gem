#include "G_WindowHook.h"
#include "G_Window.h"
#include "G_WindowManager.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Utilities.h"
#include "../Utilities/G_Time.h"
#include "../Input/G_Input.h"
#include "../Math/G_Math.h"
namespace Gem
{
    using namespace Reflection;

    WindowHook::WindowHook() :Object()
    {
        m_Window = nullptr;
        m_Timer = 0.15f;
        m_CurrentTime = 0.0f;
    }
    WindowHook::~WindowHook()
    {
        if(m_Window != nullptr)
        {
            WindowManager::instance()->detachHook(this,m_Window->handle());
            
        }
    }

    void WindowHook::update()
    {
        if(m_Window == nullptr)
        {
            log("Updating without a window");
            return;
        }

        if(isFocused() == false)
        {
            return;
        }

        Vector2 mousePosition = Input::instance()->mousePoisition();
        
        
        //Vector2 scrollDirection = Input::instance()->scrollDirection();
        //
        //if(scrollDirection.y > 0)
        //{
        //    log("Scroll Up");
        //}
        //else if(scrollDirection.y < 0)
        //{
        //    log("Scroll down");
        //}
            

        if(Input::instance()->getKeyDown(KeyCode::A))
        {
            log("X(" + F2S(mousePosition.x) + ")        Y(" + F2S(mousePosition.y) + ")");
        }

        float forwardAxis = Input::instance()->getAxis("Forward");
        m_CurrentTime += Time::deltaTime();
        if(m_CurrentTime >= m_Timer)
        {
            //log("Timer ticked");
            log("Forward Axis: " + F2S(forwardAxis));
            log("Delta Time: " + F2S(Time::deltaTime()));
            m_CurrentTime = 0.0f;
        }

        //if(Input::instance()->getKeyUp(KeyCode::A))
        //{
        //    log("A key released");
        //}
        //if(Input::instance()->getKey(KeyCode::A))
        //{
        //    m_CurrentTime += Time::deltaTime();
        //    if(m_CurrentTime >= m_Timer)
        //    {
        //        log("Timer ticked");
        //        m_CurrentTime = 0.0f;
        //    }
        //}

        //if(Input::instance()->getMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
        //{
        //    log("Left Mouse Pressed");
        //}
        //if(Input::instance()->getMouseButtonUp(MouseButton::MOUSE_BUTTON_LEFT))
        //{
        //    log("Left Mouse Released");
        //}
        //if(Input::instance()->getMouseButton(MouseButton::MOUSE_BUTTON_MIDDLE))
        //{
        //    log("Middle Mouse button Down");
        //}

    }
    bool WindowHook::isFocused()
    {
        if(m_Window == nullptr)
        {
            return false;
        }
        return m_Window->handle() == WindowManager::instance()->getFocusedWindow();
    }
    void WindowHook::onAttachToWindow(int aHandle)
    {
        log("Attached to " + I2S(aHandle));
        Input::instance()->createAxis("Forward",AxisCode::UP,AxisCode::DOWN);
        Input::instance()->setAxisPositiveKey("Forward",AxisCode::W,1);
        Input::instance()->setAxisNegativeKey("Forward",AxisCode::S,1);
        Input::instance()->setAxisResetOnRelease("Forward",false);

        

    }
    void WindowHook::onDetachFromWindow(int aHandle)
    {
        log("Detached from " + I2S(aHandle));
    }

    Type WindowHook::getType()
    {
        return TypeFactory::create("WindowHook",TypeID::WINDOW_HOOK,sizeof(WindowHook));
    }
    Type WindowHook::baseType()
    {
        return Object::getType();
    }
    Type * WindowHook::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
    }
}