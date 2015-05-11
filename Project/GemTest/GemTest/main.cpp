#include <stdlib.h>
#include "../../Common/Core/Debug.h"

using namespace Gem;

void FuncTest();

int main(int argc, char ** argv)
{

	FuncTest();


	system("pause");

	return EXIT_SUCCESS;
}

void TryOperation( float aFloat)
{
	if (aFloat < 0.0f)
	{
		Debugging::Debug::InvalidArgument(GENERATE_INVALID_ARGUMENT("aFloat", "TryOperation", 4));
	}
}

void FuncTest()
{
	int * arrayOfData = nullptr;
	if (arrayOfData == nullptr)
	{
		Debugging::Debug::ArgumentNull(GENERATE_ARGUMENT_NULL("arrayOfData","FuncTest",3));
	}

	TryOperation(-1.0f);


}