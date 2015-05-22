#include "ReflTest.h"

RDEFINE_CLASS(BaseTest, object)

RDEFINE_PUBLIC_MEMBER(BaseTest, m_HiddenData, int)

BaseTest::BaseTest() : object(),
m_HiddenData(0)
{

}
BaseTest::~BaseTest()
{
	
}


void BaseTest::SomeRegularMethod()
{

}
RDEFINE_PUBLIC_FUNCTION(BaseTest, SomeRegularMethod, void)
void BaseTest::SomeVirtualMethod()
{
	int j = 5;
	int k = 5;
	float sum = (float)j + 0.5f + (float)k;
}
RDEFINE_PUBLIC_FUNCTION(BaseTest, SomeVirtualMethod, void)

void BaseTest::OtherVirtualMethod()
{

}
RDEFINE_PUBLIC_FUNCTION(BaseTest, OtherVirtualMethod, void)

RDEFINE_CLASS(ExtendedClass,BaseTest)

ExtendedClass::ExtendedClass() : BaseTest()
{

}
ExtendedClass::~ExtendedClass()
{

}
void ExtendedClass::SomeVirtualMethod()
{
	int j = 0;
	int k = 5;
	float sum = (float)j + 0.5f + (float)k;
}
RDEFINE_PUBLIC_FUNCTION(ExtendedClass, SomeVirtualMethod, void)