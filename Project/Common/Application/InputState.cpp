#include "InputState.h"

namespace Gem
{
	RDEFINE_ENUM(InputState)
	RENUM_CPP(InputState, Values, Values::Released + 1)
	{
		"Down",
		"Up",
		"Pressed",
		"Released"
	};
}