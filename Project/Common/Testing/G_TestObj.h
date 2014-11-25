#ifndef G_TEST_OBJ_H
#define G_TEST_OBJ_H

#include "../Primitives/G_PrimitiveObjects.h"

//#define MAGIC_CLASS(TYPE,BASECLASS,NAME) Gem::Reflection::RegistryEntry<TYPE>::Instance(#NAME, #BASECLASS);


namespace Gem
{
	struct SomeUnionThing
	{
		union
		{
			float x;
			float r;
		};
		union
		{
			char charA;
			char charB;
			char charC;
			char charD;
			float y;
			float g;
		};

		union
		{
			short dataA;
			short dataB;
			float z;
			float b;
		};

		
	};

    class TestObj : public Object
    {
    public:
        int someData;
        string someName;
		std::string stdSomeName;
		float floatingData;
        int operation;
		SomeUnionThing unionGuy;
        TestObj();
		TestObj(const TestObj & aInitializer);
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