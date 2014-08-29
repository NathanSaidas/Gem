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
#include "../Base Objects/G_integer.h"
#include "../Base Objects/G_real.h"
#include "../Base Objects/G_string.h"
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
        int count = 1000;
        //Spawn 100 GO's
        if(Input::instance()->getKeyDown(KeyCode::ALPHA_1))
        {
            log("Adding 1000 Game Objects [Start]");
            for(int i = 0; i < count; i++)
            {
                GameObject::instantiate("Bobbo");
            }
            log("Adding 1000 Game Objects [End]");
        }
        //Delete 100 GO's
        if(Input::instance()->getKeyDown(KeyCode::ALPHA_2))
        {
            
            log("Flagging 1000 Game Objects [Start]");
            std::vector<GameObject*> gameObjects = GameObjectManager::instance()->getGameObjects();
            int dcount = 0;
            float time = Time::getTime();
            for(int i = gameObjects.size() -1; i >= 0; i--)
            {
                GameObject::destroy(gameObjects[i]);
                dcount ++;
                if(dcount > 1000)
                {
                    break;
                }
                    
            }
            log("Flagging 1000 Game Objects [End]");
            log("Time - " + F2S(Time::getTime() - time));
        }

        if(Input::instance()->getKeyDown(KeyCode::ALPHA_3))
        {
            log("Count = " + I2S(GameObjectManager::instance()->objectCount()));
        }

        if(Input::instance()->getKeyDown(KeyCode::Q))
        {
            log("---Memory Report(Used)---");
            log(" Total      = " + I2S(MemoryManager::instance()->getTotalBytesUsed()));
            log(" Block_8    = " + I2S(MemoryManager::instance()->getBytesUsed(8)));
            log(" Block_16   = " + I2S(MemoryManager::instance()->getBytesUsed(16)));
            log(" Block_32   = " + I2S(MemoryManager::instance()->getBytesUsed(32)));
            log(" Block_64   = " + I2S(MemoryManager::instance()->getBytesUsed(64)));
            log(" Block_128  = " + I2S(MemoryManager::instance()->getBytesUsed(128)));
            log(" Block_256  = " + I2S(MemoryManager::instance()->getBytesUsed(256)));
            log(" Block_512  = " + I2S(MemoryManager::instance()->getBytesUsed(512)));
            log(" Block_1024 = " + I2S(MemoryManager::instance()->getBytesUsed(1024)));
            log(" Block_User = " + I2S(MemoryManager::instance()->getBytesUsed(1025)));
        }

        if(Input::instance()->getKeyDown(KeyCode::E))
        {
            log("---Memory Report(Free)(Kb)---");
            log(" Total      = " + I2S(MemoryManager::instance()->getTotalBytesFree(Memory::ByteSize::KILOBYTE)));
            log(" Block_8    = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_8,Memory::ByteSize::KILOBYTE)));
            log(" Block_16   = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_16,Memory::ByteSize::KILOBYTE)));
            log(" Block_32   = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_32,Memory::ByteSize::KILOBYTE)));
            log(" Block_64   = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_64,Memory::ByteSize::KILOBYTE)));
            log(" Block_128  = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_128,Memory::ByteSize::KILOBYTE)));
            log(" Block_256  = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_256,Memory::ByteSize::KILOBYTE)));
            log(" Block_512  = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_512,Memory::ByteSize::KILOBYTE)));
            log(" Block_1024 = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_1024,Memory::ByteSize::KILOBYTE)));
            log(" Block_User = " + I2S(MemoryManager::instance()->getBytesFree(Memory::BlockSize::BLOCK_BIG,Memory::ByteSize::KILOBYTE)));
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
        log("Attached to " + I2S(aHandle));
        Input::instance()->createAxis("Forward",AxisCode::UP,AxisCode::DOWN);
        Input::instance()->setAxisPositiveKey("Forward",AxisCode::W,1);
        Input::instance()->setAxisNegativeKey("Forward",AxisCode::S,1);
        Input::instance()->setAxisResetOnRelease("Forward",false);

        GameObject * go = GameObject::instantiate("_Debug");
        go->addComponent(Memory::instantiate<DebugComponent>());
        
        //m_CoolField = 0;
        //Field<void*> * fields[3];
        //fields[0] = (Field<void*>*)(new Field<int>(m_CoolField));
        //fields[0]->setValue((void*)1);

        //Field<int> field(&m_CoolField);
        //field.setValue(30);
        //pugi::xml_document doc;
        //pugi::xml_parse_result result = doc.load_file("TestFile.xml");

        //if(result.status != pugi::xml_parse_status::status_ok)
        //{
        //    log("Problem parsing file");
        //}

        //pugi::xml_node root = doc.first_child();

        

        //Save - begin
        //pugi::xml_node root = doc.append_child("Root");
        //pugi::xml_node gameObject = root.append_child("GameObjects");
        //
        //GameObject * go = GameObject::instantiate("Base God");
        //go->addComponent(Memory::instantiate<Component>());
        //go->serialize(gameObject.append_child("GameObject_A"));
        //
        //go = GameObject::instantiate("DLevel God");
        //go->addComponent(Memory::instantiate<Component>());
        //go->addComponent(Memory::instantiate<Transform>());
        //go->serialize(gameObject.append_child("GameObject_B"));
        
        //end

        //pugi::xml_node fields = gameObject.append_child("Fields");
        //
        //Vector2 vec(30.0f,50.0f);
        //vec.serialize(fields.append_child("Vector2"));
        //
        //vec.x = 15.0f;
        //vec.y = 60.0f;
        //vec.serialize(fields.append_child("Vector2"));

        //load - begin
        //GameObject * go = GameObject::instantiate("Default");
        //pugi::xml_parse_result result = doc.load_file("TestFile.xml");
        //if(result.status != pugi::xml_parse_status::status_ok)
        //{
        //    log("Problem parsing file");
        //}
        //pugi::xml_node root = doc.child("Root");
        //pugi::xml_node gameObject = root.child("GameObjects");
        //go->deserialize(gameObject.child("GameObject_A"));
        //
        //go = GameObject::instantiate("Default");
        //go->deserialize(gameObject.child("GameObject_B"));

        //end

        //attribute.set_value("int");
        //SceneManager::instance()->loadFromMasterList();
        //
        //SceneManager::instance()->addNewSceneName("Scene_A");
        //SceneManager::instance()->addNewSceneName("Scene_B");
        //SceneManager::instance()->addNewSceneName("Scene_C");
        //
        //SceneManager::instance()->saveToMasterList();
        //
        //SceneManager::destroy();

        //STMBlock localStringBlock;
        //string * localString = (string*)Memory::instantiateSTM<string>(localStringBlock);
        //localStringBlock.registerListener(this);

        
        //Memory::destroySTM<string>(localString);

        //GameObject * go = (GameObject*)Memory::instantiateSTM<GameObject>(this);
        //go = (GameObject*)Memory::destroySTM<GameObject>(go);

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
        
        
        log("Detached from " + I2S(aHandle));
    }

    Type * WindowHook::getType()
    {
        return Type::create("WindowHook",TypeID::WINDOW_HOOK,sizeof(WindowHook),Object::getType());
    }

    void WindowHook::onMemoryClear(Object * aPtr)
    {
        if(aPtr == nullptr)
        {
            return;
        }
        Type * type = aPtr->getType();

        if(typeID(type) == TypeID::STRING)
        {
            string * lString = (string*)aPtr;
            log("Freeing Memory");
        }

        Type::freeType(type);
    }
    void WindowHook::onMemoryMove(Object * aOldPtr)
    {

    }

}