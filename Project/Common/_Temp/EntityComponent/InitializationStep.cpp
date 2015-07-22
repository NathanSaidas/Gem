#include "InitializationStep.h"

namespace Gem
{
	RDEFINE_ENUM(InitializationStep)
	RENUM_CPP(InitializationStep, Values, Values::Complete + 1)
	{
		"Initialization",
		"LateInitialization",
		"Complete"
	};
}