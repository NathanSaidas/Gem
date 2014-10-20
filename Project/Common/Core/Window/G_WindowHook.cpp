#include "G_WindowHook.h"
#include "G_Window.h"
#include "G_WindowManager.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Utilities.h"
#include "../Utilities/G_Time.h"
#include "../Input/G_Input.h"
#include "../Math/G_Math.h"
#include "../Entity Component/G_GameObject.h"
#include "../Entity Component/G_Component.h"
#include "../Entity Component/G_GameObjectManager.h"
#include "../Entity Component/Engine Components/G_Transform.h"
#include "../Entity Component/Engine Components/G_DebugComponent.h"
#include "../Utilities/G_IO.h"
#include "G_SceneManager.h"


#include <pugixml.hpp>
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


    }
    bool WindowHook::isFocused()
    {
        if(m_Window == nullptr)
        {
            return false;
        }
        return m_Window->handle() == WindowManager::instance()->getFocusedWindow();
    }
    void setInt(int & aInt)
    {
        aInt = 0;
    }

    void WindowHook::onAttachToWindow(int aHandle)
    {
        //log("Attached to " + I2S(aHandle));
        Input::instance()->createAxis("Forward",AxisCode::UP,AxisCode::DOWN);
        Input::instance()->setAxisPositiveKey("Forward",AxisCode::W,1);
        Input::instance()->setAxisNegativeKey("Forward",AxisCode::S,1);
        Input::instance()->setAxisResetOnRelease("Forward",false);

        GameObject * go = GameObject::instantiate("_Debug");
        go->addComponent(Memory::instantiate<DebugComponent>());
  
    }

    //Mystery Code
    //template<typename Func, typename T>
    //void delegate(Func func, T param)
    //{
    //    func(param);
    //}
    //
    //typedef void(*Foreach_Xml)(pugi::xml_node aNode);
    //Foreach_Xml callback = func;
    //pugi::xml_node node;
    //delegate<Foreach_Xml,pugi::xml_node>(callback,node);


    void WindowHook::onDetachFromWindow(int aHandle)
    {
        
        
        //log("Detached from " + I2S(aHandle));
    }

    Pointer<Reflection::Type> WindowHook::getType()
    {
        return typeOf("WindowHook");
    }


}