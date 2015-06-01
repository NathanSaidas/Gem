#include "InputDeviceType.h"

namespace Gem
{
	RDEFINE_ENUM(InputDeviceType)
	RENUM_CPP(InputDeviceType, Values, Values::MousePosition + 1)
	{
		"Key",
		"MouseButton",
		"MousePosition"
	};
}