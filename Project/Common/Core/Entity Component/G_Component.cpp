#include "G_Component.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "G_GameObject.h"

namespace Gem
{
    using namespace Reflection;

    void Component::destroy(GameObject * aGameObject)
    {
        GameObject::destroy(aGameObject);
    }

    void Component::destroy(Component * aComponent)
    {
        GameObject::destroy(aComponent);
    }

    Component * Component::getComponent(Reflection::Type * aComponent)
    {
        return m_GameObject->getComponent(aComponent);
    }
    Component * Component::getComponent(std::string aTypeName)
    {
        return m_GameObject->getComponent(aTypeName);
    }
    std::vector<Component*> Component::getComponents(Reflection::Type * aComponent)
    {
        return m_GameObject->getComponents(aComponent);
    }
    std::vector<Component*> Component::getComponents(std::string aTypeName)
    {
        return m_GameObject->getComponents(aTypeName);
    }
    Pointer<Reflection::Type> Component::getType()
    {
        return typeOf("Component");
    }

    Component::Component()
    {
        m_Flag = ECFlag::NONE;
    }
    Component::~Component()
    {

    }

    //Creation/Delete Functions
    void Component::onCreate()   //Unsafe Reference
    {

    }
    void Component::onInit()    //Safe Reference 
    {

    }
    void Component::onLateInit() //Safe Reference
    {

    }

    void Component::onEnable()   //Safe Reference
    {

    }
    void Component::onDisable()  //Safe Reference
    {

    }
    void Component::onDestroy()  //Safe Reference
    {

    }

    //Update Functions
    //Called Every Frame
    void Component::preUpdate()
    {

    }
    void Component::update()
    {

    }
    void Component::postUpdate()
    {

    }

    //Called every second
    void Component::slowUpdate()
    {

    }
    //Called every 1/timescale 
    void Component::fixedUpdate()
    {

    }

    //Called before a render call is made
    void Component::preRender()
    {

    }
    void Component::render()
    {

    }
    void Component::postRender()
    {

    }

    std::string Component::name()
    {
        return m_GameObject->name();
    }
    bool Component::enabled()
    {
        if(m_GameObject->enabled() == false)
        {
            return false;
        }
        return checkFlag(ECFlag::ENABLED);
    }
    void Component::setName(std::string aName)
    {
        m_GameObject->setName(aName);
    }
    void Component::setEnabled(bool aFlag)
    {
        if(m_GameObject->enabled() == false)
        {
            return;
        }
        if(enabled() == true  && aFlag == false)
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
    void Component::dontDestroyOnLoad()
    {
        raiseFlag(ECFlag::DONT_DESTROY_ON_LOAD);
    }
    void Component::raiseFlag(int aFlag)
    {
        m_Flag |= aFlag;
    }
    void Component::lowerFlag(int aFlag)
    {
        m_Flag &= ~(aFlag);
    }
    bool Component::checkFlag(int aFlag)
    {
        return (m_Flag & aFlag) == aFlag;
    }
}