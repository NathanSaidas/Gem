#include "G_DebugComponent.h"
#include <pugixml.hpp>
#include "../../Utilities/G_Utilities.h"
#include "../../Reflection/G_Reflection.h"
#include "../../Memory/G_Memory.h"
#include "../../Math/G_Math.h"
#include "../../Utilities/G_Debug.h"
#include "../../Utilities/G_Debug.cpp"

DumbStruct::DumbStruct()
{

}
DumbStruct::~DumbStruct()
{

}

//const DumbStruct instance;

namespace Gem
{
    /*Fruit * Gem::fruit::create(const std::string & name)
    {
        detail::FruitRegistry & reg = detail::getFruitRegistry();
        detail::FruitRegistry::iterator it = reg.find(name);

        if(it == reg.end())
        {
            return nullptr;
        }

        detail::CreateFruitFunc func = it->second;
        return func();
    }
    void Gem::fruit::destroy(const Fruit * comp)
    {
        delete comp;
    }*/


    using namespace Reflection;
    bool DebugComponent::s_DebugComponentRegistered = false;
    DebugComponent::DebugComponent()
    {
        DebugComponent::s_DebugComponentRegistered = true;
    }
    DebugComponent::~DebugComponent()
    {
        
    }


    pugi::xml_node DebugComponent::serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo)
    {
        return Component::serialize(aNode);
    }
    bool DebugComponent::deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo)
    {
        return Component::deserialize(aNode);
    }
    Pointer<Reflection::Type> getType()
    {
        return typeOf("DebugComponent");
    }

    //Creation/Delete Functions
    void DebugComponent::onCreate()   //Unsafe Reference
    {
        log("Create");
    }
    void DebugComponent::onInit()     //Safe Reference 
    {
        log("Init");

        //bool isEqual = instanceOf(objType.ref(),compType.ref());
        //isEqual = instanceOf(compType.ref(),objType.ref());
        //isEqual = instanceOf(type.ref(), objType.ref());


    }
    void DebugComponent::onLateInit() //Safe Reference
    {
        log("Late Init");
    }

    void DebugComponent::onEnable()   //Safe Reference
    {
        log("Enable");
    }
    void DebugComponent::onDisable()  //Safe Reference  
    {

    }
    void DebugComponent::onDestroy()  //Safe Reference  
    {
        log("Destroy");
        m_IntPtr.release();
    }


    void DebugComponent::preUpdate() 
    {
        

    }
    void DebugComponent::update()
    {

    }
    void DebugComponent::postUpdate()
    {

    }

    void DebugComponent::slowUpdate()
    {
        log("Slow Update");
    }

    void DebugComponent::fixedUpdate()
    {

    }

    void DebugComponent::preRender()
    {

    }
    void DebugComponent::render()
    {

    }
    void DebugComponent::postRender()
    {

    }

}