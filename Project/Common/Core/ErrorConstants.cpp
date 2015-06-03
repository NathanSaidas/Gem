#include "ErrorConstants.h"

namespace Gem
{
	namespace Debugging
	{
		RDEFINE_ENUM(ErrorConstants);
		RENUM_CPP(ErrorConstants, Values, Values::Empty + 1)
		{
			//Arguments
			"Invalid Argument",
			"Argument was null",

			//Windows
			"FailedWindowCreation",
			"LeakedWindowMemory",

			//Entity Component
			"InvalidGameObjectInstantiation",
			"SceneGraphNotInitialized",
			//Empty Argument used for count.
			"Empty",

		};
	}
	
}