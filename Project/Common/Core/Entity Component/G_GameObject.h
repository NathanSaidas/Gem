#ifndef G_GAME_OBJECT_H
#define G_GAME_OBJECT_H

#include "../Base Objects/G_Object.h"
#include "../Utilities/G_IXmlSerializable.h"
#include <string>
#include <vector>

namespace Gem
{
    class Component;

    namespace ECFlag
    {
        const int NONE                  = 1 << 0; //1
        const int ENABLED               = 1 << 1; //2
        const int DONT_DESTROY_ON_LOAD  = 1 << 2; //4
        const int FLAGGED_FOR_DESTROY   = 1 << 3; //8
    }

    class GameObject : public Object, IXmlSerializable
    {
    public:
        GameObject();
        virtual ~GameObject();

        static GameObject * instantiate(std::string aName);
        static void destroy(GameObject * aGameObject);
        static void destroyImmediate(GameObject * aGameObject);
        static void destroy(Component * aComponent);

        Component * addComponent(Component * aComponent);
        Component * getComponent(Reflection::Type * aComponent);
        Component * getComponent(std::string aTypeName);
        std::vector<Component*> getComponents(Reflection::Type * aComponent);
        std::vector<Component*> getComponents(std::string aTypeName);

        std::string name();
        bool enabled();
        void setName(std::string aName);
        void setEnable(bool aFlag);
        void dontDestroyOnLoad();

        virtual pugi::xml_node serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo = false);
        virtual bool deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo = false);

        virtual Reflection::Type * getType();
    private:
        

        //Functions to call on components
        
        //void addComponentBypass(
        void addComponentBypass(Component * aComponent);
        void addComponentBypass(pugi::xml_node_iterator & aComponent);
        void removeComponent(Component * aComponent);

        //Creation/Delete Functions
        void onCreate();   //Unsafe Reference
        void onInit();     //Safe Reference 
        void onLateInit(); //Safe Reference

        void onEnable();   //Safe Reference
        void onDisable();  //Safe Reference
        void onDestroy();  //Safe Reference

        //Update Functions
        //Called Every Frame
        void preUpdate(); 
        void update();
        void postUpdate();

        //Called every second
        void slowUpdate();
        //Called every 1/timescale 
        void fixedUpdate();

        //Called before a render call is made
        void preRender();
        void render();
        void postRender();

        //TODO: Collision & Network Calls


        std::string m_Name;
        std::vector<Component*>m_Components;


        int m_Flag;
        void raiseFlag(int aFlag);
        void lowerFlag(int aFlag);
        bool checkFlag(int aFlag);

        friend class GameObjectManager;
    };
}

#endif