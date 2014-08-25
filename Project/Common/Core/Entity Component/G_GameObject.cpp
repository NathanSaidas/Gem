#include "G_GameObject.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "G_Component.h"
#include "G_GameObjectManager.h"
#include <pugixml.hpp>
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
        int size = sizeOf(aComponent);
        //TODO: Waiting for Entity Component Manager
        aComponent->onDisable();
        aComponent->onDestroy();
        GameObject * gameObject = aComponent->m_GameObject;
        gameObject->removeComponent(aComponent);
        aComponent = (Component*)Memory::destroySize(aComponent,size);
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
    Component * GameObject::getComponent(Reflection::Type * aComponent)
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
    Component * GameObject::getComponent(std::string aTypeName)
    {
        for(int i = 0; i < m_Components.size(); i++)
        {
            if(aTypeName == nameOf(m_Components[i]->getType()))
            {
                return m_Components[i];
            }
        }
        return nullptr;
    }
    std::vector<Component*> GameObject::getComponents(Reflection::Type * aComponent)
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
    std::vector<Component*> GameObject::getComponents(std::string aTypeName)
    {
        std::vector<Component*> components;
        for(int i = 0; i < m_Components.size(); i++)
        {
            if(aTypeName == nameOf(m_Components[i]->getType()))
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
        int type = aComponent->attribute("TypeID").as_int();
        pugi::xml_node node(aComponent->internal_object());

        //Add a component based on type ID
        //Initialize component with the internal  object of the iterator node
        switch (type)
        {
            //BASIC EXAMPLE
        case TypeID::COMPONENT:
            {
                Component * component = Memory::instantiate<Component>();
                addComponentBypass(component);
                component->deserialize(node);
            }
            break;
        case TypeID::TRANSFORM:
            {
                Transform * transform = Memory::instantiate<Transform>();
                addComponentBypass(transform);
                transform->deserialize(node);
            }
            break;

        default:

            break;
        }
        
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

    pugi::xml_node GameObject::serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo)
    {
        //aNode represents the this gameobject

        //Serialized Fields
        pugi::xml_node fieldsNode = aNode.append_child("Fields");

        pugi::xml_node nameNode = fieldsNode.append_child("Name");
        nameNode.append_attribute("Value")= m_Name.c_str();
        
        pugi::xml_node flagNode = fieldsNode.append_child("Flags");
        flagNode.append_attribute("Value")= m_Flag;

        pugi::xml_node componentsNode = fieldsNode.append_child("Components");
        componentsNode.append_attribute("Count") = m_Components.size();

        std::string componentNodeName = "";

        for(int i = 0; i < m_Components.size(); i++)
        {
            if(m_Components[i] == nullptr)
            {
                continue;
            }
            componentNodeName = "Component_" + I2S(i);
            m_Components[i]->serialize(componentsNode.append_child(componentNodeName.c_str()));
        }


        return aNode;
    }
    bool GameObject::deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo)
    {
        int threshHold = 3;
        int count = 0;
        pugi::xml_node fieldsNode = aNode.child("Fields");
        std::string nodeName = "";
        //Go through all nodes
        for(pugi::xml_node_iterator nodeIter = fieldsNode.begin(); nodeIter != fieldsNode.end(); ++nodeIter)
        {
            nodeName = nodeIter->name();
            if(nodeName == "Name")
            {
                m_Name = nodeIter->attribute("Value").as_string();
                count++;
            }
            else if(nodeName == "Flags")
            {
                m_Flag = nodeIter->attribute("Value").as_int();
                count++;
            }
            else if(nodeName == "Components")
            {
                threshHold = 3 + nodeIter->attribute("Count").as_int();
                for(pugi::xml_node_iterator compIter = nodeIter->begin(); compIter != nodeIter->end(); ++compIter)
                {
                    addComponentBypass(compIter);
                    count ++;
                }
                count ++;
            }
            if(count > 3)
            {
                break;
            }
        }
        return count >= threshHold;
    }
    Type * GameObject::getType()
    {
        return Type::create("GameObject",TypeID::GAME_OBJECT,sizeof(GameObject),Object::getType());
    }
}
