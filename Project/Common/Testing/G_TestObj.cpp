#include "G_TestObj.h"
#include "../Reflection/G_Reflection.h"
namespace Gem
{
    TestObj::TestObj()
    {
        someData = 0;
		floatingData = 0.0f;
        operation = 0;
    }
    TestObj::~TestObj()
    {

    }
    Pointer<Reflection::Type> TestObj::GetType()
    {
        return typeOf("TestObj");
    }
}