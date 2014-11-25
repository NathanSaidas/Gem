#include "G_TestObj.h"
#include "../Reflection/G_Reflection.h"
namespace Gem
{
    TestObj::TestObj()
    {
        someData = 0;
		floatingData = 0.0f;
        operation = 0;
		someName = string::Empty();
		stdSomeName = "";
		memset(&unionGuy, 0, sizeof(SomeUnionThing));
    }
	TestObj::TestObj(const TestObj & aInitializer)
	{
		someData = aInitializer.someData;
		floatingData = aInitializer.floatingData;
		operation = aInitializer.operation;
		someName.Set(aInitializer.someName.C_Str());
		stdSomeName = aInitializer.stdSomeName;
		unionGuy = aInitializer.unionGuy;
	}
    TestObj::~TestObj()
    {

    }
    Pointer<Reflection::Type> TestObj::GetType()
    {
        return typeOf("TestObj");
    }
}