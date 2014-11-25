#ifndef G_TEST_OBJ_H
#define G_TEST_OBJ_H

#include "../Primitives/G_PrimitiveObjects.h"

//#define MAGIC_CLASS(TYPE,BASECLASS,NAME) Gem::Reflection::RegistryEntry<TYPE>::Instance(#NAME, #BASECLASS);


namespace Gem
{
    class TestObj : public Object
    {
    public:
        int someData;
        //string someName;
		float floatingData;
        int operation;
        TestObj();
        ~TestObj();
        virtual Pointer<Reflection::Type> GetType() override;
    };

    template<class T>
    class TestTemplate
    {
    public:
        TestTemplate(const char * aName, const char * aBaseClass)
        {
            std::cout << "Doing some pre init\n";
            instance = new T();
            Gem::Reflection::RegistryEntry<T>::Instance(aName, aBaseClass);
        }
        ~TestTemplate()
        {
    
        }
        T * instance;
    };

}
GEM_CLASS(TestObj,Object)

#endif