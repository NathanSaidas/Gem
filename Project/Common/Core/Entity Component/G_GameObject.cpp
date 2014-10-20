#include "G_GameObject.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "G_Component.h"
#include "G_GameObjectManager.h"
#include "../Utilities/G_Utilities.h"

#include "Engine Components\G_Transform.h"


namespace Gem
{
    using namespace Reflection;

    GameObject * GameObject::instantiate(std::string aName)
    {
        return GameObjectManager::instantiate(aName);
    }
    void GameObject::destroy(GameObject * aGameObject)
    {
        GameObjectManager::destroy(aGameObject);
    }
    void GameObject::destroyImmediate(GameObject * aGameObject)
    {
        GameObjectManager::destroyImmediate(aGameObject);
    }
    void GameObject::destroy(Component * aComponent)
    {
		if (aComponent == nullptr)
		{
			return;
		}

		

        //int size = sizeOf(aComponent);
        //TODO: Waiting for Entity Component Manager
        aComponent->onDisable();
        aComponent->onDestroy();
		GameObject * gameObject = aComponent->m_GameObject;
		gameObject->removeComponent(aComponent);
		

        //GameObject * gameObject = aComponent->m_GameObject;
        //gameObject->removeComponent(aComponent);
        // = (Component*)Memory::destroySize(aComponent,size);
    }

    Component * GameObject::addComponent(Component * aComponent)
    {
        if(aComponent == nullptr)
        {
            return nullptr;
        }
        aComponent->m_GameObject = this;
        aComponent->onCreate();
        aComponent->onInit();
        aComponent->onLateInit();
        aComponent->onEnable();
        m_Components.push_back(aComponent);
        return aComponent;
    }
    Component * GameObject::getComponent(Pointer<Reflection::Type> aComponent)
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            if(m_Components[i]->getType() == aComponent)
            {
                return m_Components[i];
            }
        }
        return nullptr;
    }
    Component * GameObject::getComponent(string aTypeName)
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            if(aTypeName == m_Components[i]->getType().data().name())
            {
                return m_Components[i];
            }
        }
        return nullptr;
    }
    std::vector<Component*> GameObject::getComponents(Pointer<Reflection::Type> aComponent)
    {
        std::vector<Component*> components;
        for(int i = 0; i < m_Components.size(); i++)
        {
            if(m_Components[i]->getType() == aComponent)
            {
                components.push_back(m_Components[i]);
            }
        }
        return components;
    }
    std::vector<Component*> GameObject::getComponents(string aTypeName)
    {
        std::vector<Component*> components;
        for(int i = 0; i < m_Components.size(); i++)
        {
            
            if (aTypeName == m_Components[i]->getType().data().name())
            {
                components.push_back(m_Components[i]);
            }
        }
        return components;
    }

    void GameObject::addComponentBypass(Component * aComponent)
    {
        if(aComponent != nullptr)
        {
            aComponent->m_GameObject = this;
            m_Components.push_back(aComponent);
        }
    }
    void GameObject::addComponentBypass(pugi::xml_node_iterator & aComponent)
    {
        //Get Type information
        //string type = aComponent->attribute("TypeName").as_string();
        //pugi::xml_node node(aComponent->internal_object());

        //Add a component based on type ID
        //Initialize component with the internal  object of the iterator node
        
        
    }
    void GameObject::removeComponent(Component * aComponent)
    {

        for(int i = 0; i < m_Components.size(); i++)
        {
            if(m_Components[i] == aComponent)
            {
                m_Components.erase(m_Components.begin() + i);
                break;
            }
        }
    }

    GameObject::GameObject() : Object()
    {
        m_Flag = ECFlag::NONE;
    }
    GameObject::~GameObject()
    {
        for(int i = m_Components.size() -1; i >= 0; i --)
        {
            destroy(m_Components[i]);
        }
    }

    //Creation/Delete Functions
    void GameObject::onCreate()    //Unsafe Reference
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->onCreate();
        }
    }
    void GameObject::onInit()      //Safe Reference 
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->onInit();
        }
    }
    void GameObject::onLateInit()  //Safe Reference
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->onLateInit();
        }
    }

    void GameObject::onEnable()    //Safe Reference
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->onEnable();
        }
    }
    void GameObject::onDisable()   //Safe Reference
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->onDisable();
        }
    }
    void GameObject::onDestroy()   //Safe Reference
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->onDestroy();
        }
    }
    //Update Functions
    void GameObject::preUpdate()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->preUpdate();
        }
    }
    void GameObject::onSceneLoaded(Scene * aScene)
    {

    }
	void GameObject::onApplicationQuit()
	{
		for (int i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->onApplicationQuit();
		}
	}
    void GameObject::update()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->update();
        }
    }
    void GameObject::postUpdate()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->postUpdate();
        }
    }

    void GameObject::slowUpdate()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->slowUpdate();
        }
    }
    void GameObject::fixedUpdate()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->fixedUpdate();
        }
    }

    void GameObject::preRender()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->preRender();
        }
    }
    void GameObject::render()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->render();
        }
    }
    void GameObject::postRender()
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            m_Components[i]->postRender();
        }
    }


    std::string GameObject::name()
    {
        return m_Name;
    }
    bool GameObject::enabled()
    {
        return checkFlag(ECFlag::ENABLED);
    }
    void GameObject::setName(std::string aName)
    {
        m_Name = aName;
        //for(int i = 0; i < m_Components.size(); i++)
        //{
        //    m_Components[i]->m_Name = aName;
        //}
    }
    void GameObject::setEnable(bool aFlag)
    {
        if(enabled() == true && aFlag == false)
        {
            onDisable();
            lowerFlag(ECFlag::ENABLED);
        }
        else if(enabled() == false && aFlag == true)
        {
            onEnable();
            raiseFlag(ECFlag::ENABLED);
        }
    }
    void GameObject::dontDestroyOnLoad()
    {
        raiseFlag(ECFlag::DONT_DESTROY_ON_LOAD);
    }
    void GameObject::raiseFlag(int aFlag)
    {
        m_Flag |= aFlag;
    }
    void GameObject::lowerFlag(int aFlag)
    {
        m_Flag &= ~(aFlag);
    }
    bool GameObject::checkFlag(int aFlag)
    {
        return (m_Flag & aFlag) == aFlag;
    }
    Pointer<Reflection::Type> GameObject::getType()
    {
        return typeOf("GameObject");
    }
}
