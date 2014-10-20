#ifndef G_DEBUG_COMPONENT_H
#define G_DEBUG_COMPONENT_H

#include "../G_Component.h"
#include "../../Memory/G_Pointer.h"
#include <map>
#include <string>
#include <utility>

const struct DumbStruct
{
    DumbStruct();
    ~DumbStruct();
};





namespace Gem
{

    /*class Fruit;

    namespace fruit
    {
        namespace detail
        {
            typedef Fruit *(*CreateFruitFunc)();
            typedef std::map<std::string, CreateFruitFunc> FruitRegistry;

            inline FruitRegistry & getFruitRegistry()
            {
                static FruitRegistry reg;
                return reg;
            }

            template<class T>
            Fruit * createFruit()
            {
                return new T;
            }

            template<class T>
            struct RegistryEntry
            {
            public:
                static RegistryEntry<T>&Instance(const std::string & name)
                {
                    static RegistryEntry<T> inst(name);
                    return inst;
                }
            private:
                RegistryEntry(const std::string & name)
                {
                    FruitRegistry & reg = getFruitRegistry();
                    CreateFruitFunc func = createFruit<T>;

                    std::pair<FruitRegistry::iterator, bool> ret = reg.insert(FruitRegistry::value_type(name,func));
                    if(ret.second == false)
                    {

                    }

                    RegistryEntry(const RegistryEntry<T>&) = delete;
                    RegistryEntry & operator=(const RegistryEntry<T>&) = delete;
                }
            };
        }
    }



    class Fruit
    {
        //fruit implementation
    };

    namespace fruit
    {
        Fruit * create(const std::string & name);
        void destroy(const Fruit * val);
    }

#define COMPONENT_REGISTER(TYPE, NAME)                                                                                                                        \    
    namespace fruit {                                                                                                                                         \                                                                                                                                                 \
    namespace detail {                                                                                                                                        \                                                                                                                                             \
    namespace                                                                                                                                                 \
    {                                                                                                                                                         \
                template<class T>                                                                                                                             \ 
                class FruitRegistration;                                                                                                                      \ 
                                                                                                                                                              \ 
                template<>                                                                                                                                    \ 
                class FruitRegistration<TYPE>                                                                                                                 \ 
                {                                                                                                                                             \ 
                                                                                                                                                              \ 
                    static const ::Gem::fruit::detail::RegistryEntry<TYPE>&reg;                                                                               \ 
                };                                                                                                                                            \ 
                                                                                                                                                              \ 
                const ::Gem::fruit::detail::RegistryEntry<TYPE>& FruitRegistration<TYPE>::reg = ::Gem::fruit::detail::RegistryEntry<TYPE>::Instance(NAME);    \ 
    }}}*/



    
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