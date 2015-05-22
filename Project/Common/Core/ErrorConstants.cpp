#include "ErrorConstants.h"

namespace Gem
{
	namespace Debugging
	{
		RDEFINE_ENUM(ErrorConstants2);
		RENUM_CPP(ErrorConstants2, Values, Values::Empty + 1)
		{
			//Arguments
			"Invalid Argument",
			"Argument was null",

			//Memory
			"Failed to create a window.",
			"Possibly Leaked Window Memory.",

			//Entity Component
			"The gameobject was not allocated using a stack or pool allocator.",

			//Empty Argument used for count.
			"Empty Argument",

		};
	}
	
}