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
		Debugging::Trace trace = Debugging::GET_TRACE(4);
		Debugging::InvalidArgument error = Debugging::InvalidArgument("aFloat", nullptr, 38, trace, "TryOperation");
		Debugging::Debug::InvalidArgument(error);
	}
}

void FuncTest()
{
	int * arrayOfData = nullptr;
	if (arrayOfData == nullptr)
	{
		Debugging::Trace trace = Debugging::GET_TRACE(3);
		Debugging::ArgumentNull error = Debugging::ArgumentNull("arrayOfData", nullptr, 37, trace, "FuncTest");
		Debugging::Debug::ArgumentNull(error);
	}

	TryOperation(-1.0f);


}