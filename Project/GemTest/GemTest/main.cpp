#include <stdlib.h>
#include <utility>
#include <memory>
#include "../../Common/Engine.h"

using namespace Gem;

template<typename CLASS, typename RETURN, typename ... ARGS>
class MethodTest
{
public:
	typedef RETURN(CLASS::*Delegate)(ARGS...);
	MethodTest()
	{
		m_Function = nullptr;
	}
	MethodTest(Delegate aFunction)
	{
		m_Function = aFunction;
	}
	~MethodTest()
	{

	}

	Delegate operator=(Delegate aFunction)
	{
		return m_Function = aFunction;
	}

	RETURN operator()(CLASS& aInstance, ARGS ... args)
	{
		if (m_Function != nullptr)
		{
			return aInstance.*m_Function(args...);
		}
		return RETURN();
	}

	RETURN operator()(CLASS* aInstance, ARGS ... args)
	{
		if (m_Function != nullptr && aInstance != nullptr)
		{
			return aInstance->*m_Function(args...);
		}
		return RETURN();
	}

	RETURN Invoke(CLASS& aInstance, ARGS ... args)
	{
		if (m_Function != nullptr)
		{
			return aInstance.*m_Function(args...);
		}
		return RETURN();
	}
	RETURN Invoke(CLASS* aInstance, ARGS ... args)
	{
		if (m_Function != nullptr && aInstance != nullptr)
		{
			return aInstance->*m_Function(args...);
		}
		return RETURN();
	}

private:
	Delegate m_Function;
};


struct Vec2
{
public:
	Vec2() : x(0.0f), y(0.0f){}
	Vec2(float aX, float aY) :x(aX), y(aY){}
	~Vec2() {}

	float x;
	float y;

};

struct Vec3
{
public:
	Vec3() : x(0.0f), y(0.0f), z(0.0f){}
	Vec3(float aX, float aY, float aZ) :x(aX), y(aY), z(aZ){}
	~Vec3() {}

	float x;
	float y;
	float z;

};

int AddInts(int x, int y)
{
	return x + y;
}

float AddFloats(float x, float y)
{
	return x + y;
}

void Voidtest()
{
	Gem::Debugging::Debug::Log("Test", "I am a test message");
}

class FuncTesterYea
{
public:
	FuncTesterYea(){};
	FuncTesterYea(int x, int y) :a(x), b(y){}
	~FuncTesterYea(){};

	void Add();
	void Set(int x, int y);
	int GetSum();

private:
	int a;
	int b;
	int sum;

};

void FuncTesterYea::Add()
{
	sum = a + b;
}

void FuncTesterYea::Set(int x, int y)
{
	a = x;
	b = y;
}

int FuncTesterYea::GetSum()
{
	return sum;
}



int main(int argc, char ** argv)
{
	Gem::Memory::MemoryManager::Initialize();
	Gem::Reflection::Runtime::Compile(nullptr);

	
	

	
	Func<void> t = Voidtest;
	t();

	

	Method<FuncTesterYea, void> tAdd = &FuncTesterYea::Add;
	Method<FuncTesterYea, void, int, int> tSet = &FuncTesterYea::Set;
	Method<FuncTesterYea, int> tSum = &FuncTesterYea::GetSum;

	Method<FuncTesterYea, void>::Delegate tAddDelegate = &FuncTesterYea::Add;

	//Normal Test
	FuncTesterYea tObject;
	tSet(tObject, 10, 5);
	tAdd(tObject);
	int sum = tSum(tObject);
	Gem::Debugging::Debug::LogFormat("Test", "Sum = %d", sum);

	//Dynamic Test
	FuncTesterYea * tPtrObject = MEM_FRAME_ALLOC_T(FuncTesterYea);
	tSet(tPtrObject, 5, 7);
	tAdd(tPtrObject);
	sum = tSum(tPtrObject);
	Gem::Debugging::Debug::LogFormat("Test", "Sum = %d", sum);
	//MEM_FRAME_DEALLOC_T(tPtrObject, FuncTesterYea);

	//Other Constructor Test
	tPtrObject = MEM_FRAME_ALLOC_T(FuncTesterYea, 20, 15);
	tAdd(tPtrObject);
	sum = tSum(tPtrObject);
	Gem::Debugging::Debug::LogFormat("Test", "Sum = %d", sum);
	//MEM_STACK_DEALLOC_T(tPtrObject, FuncTesterYea);


	//MethodTest<FuncTesterYea, void, int, int> tSet = &FuncTesterYea::Set;

	Gem::Memory::MemoryManager::GetInstance()->ResetFrame();

	Gem::Reflection::Runtime::Terminate();
	Gem::Memory::MemoryManager::Terminate();
	system("pause");

	return EXIT_SUCCESS;
}
