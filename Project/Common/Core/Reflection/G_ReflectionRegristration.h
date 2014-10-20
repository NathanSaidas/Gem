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
        inline TypeList & getTypeRegistry()
        {
            static TypeList registry;
            return registry;
        }

		//October,20,2014 - Use ReflectionFactory instead.
        ////Invokes Constructor of Type
        //template<class T>
        //Primitive * createPrimitive(Primitive * aAddress)
        //{
        //    return new(aAddress)T();
        //}
        ////Invokes Destroy of Type
        //template<class T>
        //Primitive * destroyPrimitive(Primitive * aAddress)
        //{
        //    T * typeObj = (T*)aAddress;
        //    typeObj->T::~T();
        //    return nullptr;
        //}

#pragma region ChangeLog
		/* October,20,2014 - Nathan Hanlan - Added Reflection Factory. Friend this class to house private constructors / destructors
		*
		*/
#pragma endregion EndChangeLog
		class ReflectionFactory
		{
		public:
			template<class T>
			static Primitive * createPrimitive(Primitive * aAddress)
			{
				return new(aAddress)T();
			}

			template<class T>
			static Primitive * destroyPrimitive(Primitive * aAddress)
			{
				T * typeObj = (T*)aAddress;
				typeObj->T::~T();
				return nullptr;
			}
		};


        //Functions get the function pointer of type
        //Primitive * createPrimitive(Primitive * aAddress, std::string & aTypeName);
        //Primitive * destroyPrimitive(Primitive * aAddress, std::string & aTypeName);
    }
}



namespace Gem
{
    namespace Reflection
    {
        template<class T>
        struct RegistryEntry
        {
          public:
            static RegistryEntry<T>& Instance(const std::string& name,const std::string & baseclass)
            {
                // Because I use a singleton here, even though `COMPONENT_REGISTER`
                // is expanded in multiple translation units, the constructor
                // will only be executed once. Only this cheap `Instance` function
                // (which most likely gets inlined) is executed multiple times.

                static RegistryEntry<T> inst(name,baseclass);
                return inst;
            }

          private:
            RegistryEntry(const std::string& name,const std::string & baseclass)
            {
                TypeList & reg = getTypeRegistry();
				reg.addKey(TypeInfo::create(name, reg.getTypeCount(), sizeof(T), __alignof(T), ReflectionFactory::createPrimitive<T>, ReflectionFactory::destroyPrimitive<T>, baseclass));
                std::cout << "Registering " << name << std::endl;
                //ComponentRegistry& reg = getComponentRegistry();
                //CreateComponentFunc func = createComponent<T>;
                //int i = g_Ung.getUniqueNumber();

                //std::pair<ComponentRegistry::iterator, bool> ret =
                    //reg.insert(ComponentRegistry::value_type(name, func));

                //if (ret.second == false) {
                    // This means there already is a component registered to
                    // this name. You should handle this error as you see fit.
                //}
            }


            //RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
            //RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
        };

    } // namespace detail

} // namespace component



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

#endif