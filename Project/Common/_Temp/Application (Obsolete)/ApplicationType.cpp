#include "ApplicationType.h"

namespace Gem
{
	//const std::string ApplicationType::NAMES[Values::Window + 1] =
	//{
	//	"Invalid",
	//	"Console",
	//	"Window"
	//};

	RDEFINE_ENUM(ApplicationType);
	RENUM_CPP(ApplicationType, Values, Values::Window + 1)
	{
		"Invalid",
		"Console",
		"Window"
	};
}
