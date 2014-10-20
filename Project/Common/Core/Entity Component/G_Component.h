#ifndef G_COMPONENT_H
#define G_COMPONENT_H
#include "../Primitives/G_PrimitiveObjects.h"
#include "../Utilities/G_IXmlSerializable.h"
#include <string>
#include <vector>

namespace Gem
{
    class GameObject;

    class Component : public Object
    {
    public:
        Component();
        virtual ~Component();
        static void destroy(GameObject * aGameObject);
        static void destroy(Component * aComponent);

        Component * getComponent(Reflection::Type * aComponent);
        Component * getComponent(std::string aTypeName);
        std::vector<Component*> getComponents(Reflection::Type * aComponent);
        std::vector<Component*> getComponents(std::string aTypeName);

        std::string name();
        bool enabled();
        void setName(std::string aName);
        void setEnabled(bool aFlag);
        void dontDestroyOnLoad();
        

        virtual Pointer<Reflection::Type> getType() override;
    private:
        
        GameObject * m_GameObject;

        //Functions to call on components

        //Creation/Delete Functions
        virtual void onCreate();   //Unsafe Reference
        virtual void onInit();     //Safe Reference 
        virtual void onLateInit(); //Safe Reference

        virtual void onEnable();   //Safe Reference
        virtual void onDisable();  //Safe Reference
        virtual void onDestroy();  //Safe Reference

		virtual void onApplicationQuit();
        //Update Functions
        //Called Every Frame
        virtual void preUpdate(); 
        virtual void update();
        virtual void postUpdate();

        //Called every second
        virtual void slowUpdate();
        //Called every 1/timescale 
        virtual void fixedUpdate();

        //Called before a render call is made
        virtual void preRender();
        virtual void render();
        virtual void postRender();

        int m_Flag;
        void raiseFlag(int aFlag);
        void lowerFlag(int aFlag);
        bool checkFlag(int aFlag);

        friend class GameObject;
        friend class PoolAllocator;
    };

}
GEM_CLASS(Component,Object)

#endif