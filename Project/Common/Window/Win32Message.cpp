#include "Win32Message.h"


namespace Gem
{
	const std::string Win32Message::NAMES[Values::Invalid + 1] =
	{
		"Resize",
		"Destroy",
		"Active",
		"Invalid",
	};

	RDEFINE_ENUM(Win32Message);
	RENUM_CPP(Win32Message, Values, Values::Invalid + 1, NAMES)
}