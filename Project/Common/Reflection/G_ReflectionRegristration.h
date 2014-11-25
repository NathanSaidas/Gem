#ifndef G_REFLECTION_REGRISTRATION_H
#define G_REFLECTION_REGRISTRATION_H


#include <string>

#include <new.h>
#include "G_TypeList.h"

namespace Gem
{
    namespace Reflection
    {
        //This is where the type registry object is held
        inline TypeList & GetTypeRegistry()
        {
            static TypeList registry;
            return registry;
        }

		template<class T>
		class ReflectionFactory
		{
		public:
			//Invokes Constructor of Type
			static Primitive * createPrimitive(Primitive * aAddress)
			{
				return new(aAddress)T();
			}
			//Invokes Destroy of Type
			
			static Primitive * DestroyPrimitive(Primitive * aAddress)
			{
				T * typeObj = (T*)aAddress;
				typeObj->T::~T();
				return nullptr;
			}
		};

        
    }
}



namespace Gem
{
    namespace Reflection
    {
        template<class T>
        class RegistryEntry
        {
          public:
            static RegistryEntry<T>& Instance(const std::string& name,const std::string & baseclass)
            {
                static RegistryEntry<T> inst(name,baseclass);
                return inst;
            }

          private:
            RegistryEntry(const std::string& name,const std::string & baseclass)
            {
                TypeList & reg = GetTypeRegistry();
                reg.addKey(TypeInfo::create(name, reg.GetTypeCount(), sizeof(T), __alignof(T), ReflectionFactory<T>::createPrimitive, ReflectionFactory<T>::DestroyPrimitive, baseclass));
                std::cout << "Registering " << name << std::endl;
            }
        };

    }

} 



#define CLASS_REGISTER(TYPE, NAME,BASECLASS)                                  \
    namespace Gem {                                                           \
    namespace Reflection {                                                    \
    namespace                                                                 \
    {                                                                         \
        template<class T>                                                     \
        class ClassReg;                                                       \
                                                                              \
        template<>                                                            \
        class ClassReg<TYPE>                                                  \
        {                                                                     \
            static const RegistryEntry<TYPE>& reg;                            \
        };                                                                    \
                                                                              \
        const RegistryEntry<TYPE>& ClassReg<TYPE>::reg =                      \
                RegistryEntry<TYPE>::Instance(NAME,BASECLASS);                \
    }}}                                                                       \



#define GEM_CLASS(CLASSNAME,BASECLASS) CLASS_REGISTER(CLASSNAME, #CLASSNAME,#BASECLASS)


//#define TEST_CLASS(TYPE,NAME,BASECLASS)                                                                                             \
//template<class T>                                                                                                                   \
//class TestReg;                                                                                                                      \
//                                                                                                                                    \
//template<>                                                                                                                          \
//class TestReg < TYPE >                                                                                                              \
//{                                                                                                                                   \
//    static const Gem::Reflection::RegistryEntry<TYPE> & reg;                                                                        \
//};                                                                                                                                  \
//const Gem::Reflection::RegistryEntry<TYPE> & TestReg<TYPE>::reg = Gem::Reflection::RegistryEntry<TYPE>::Instance(NAME, BASECLASS);  \

#endif