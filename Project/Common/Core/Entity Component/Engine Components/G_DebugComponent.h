#ifndef G_DEBUG_COMPONENT_H
#define G_DEBUG_COMPONENT_H

#include "../G_Component.h"
#include "../../Memory/G_Pointer.h"
#include <map>
#include <string>
#include <utility>





namespace Gem
{
    class DebugComponent : public Component
    {
        
    public:
        static bool s_DebugComponentRegistered;

        DebugComponent();
        ~DebugComponent();



        virtual Pointer<Reflection::Type> getType() override;
    private:
        Pointer<integer> m_IntPtr;



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
    };
}

#endif