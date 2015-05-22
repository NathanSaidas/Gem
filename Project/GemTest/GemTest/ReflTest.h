#ifndef REFL_TEST_H
#define REFL_TEST_H

#include "../Engine.h"

using namespace Gem;


class BaseTest : public object
{
	RDECLARE_CLASS(BaseTest)
public:
	BaseTest();
	virtual ~BaseTest();

	
	void SomeRegularMethod();
	RDECLARE_PUBLIC_FUNCTION(BaseTest, SomeRegularMethod)
	virtual void SomeVirtualMethod();
	RDECLARE_PUBLIC_FUNCTION(BaseTest, SomeVirtualMethod)

	virtual void OtherVirtualMethod();
	RDECLARE_PUBLIC_FUNCTION(BaseTest, OtherVirtualMethod)

private:

	int m_HiddenData;
	RDECLARE_PRIVATE_MEMBER(BaseTest, m_HiddenData)

};

class ExtendedClass : public BaseTest
{
	RDECLARE_CLASS(ExtendedClass)
public:
	ExtendedClass();
	~ExtendedClass();
	
	void SomeVirtualMethod() override;
	RDECLARE_PUBLIC_FUNCTION(ExtendedClass, SomeVirtualMethod)

};


#endif