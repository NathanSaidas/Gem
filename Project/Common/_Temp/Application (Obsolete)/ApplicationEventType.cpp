#include "ApplicationEventType.h"

#include "ApplicationType.h"

namespace Gem
{
	//const std::string ApplicationEventType::NAMES[Values::OnSystemsInitialized + 1] =
	//{
	//	"Invalid",
	//	"OnStart",
	//	"OnStop",
	//	"OnSystemsInitialized"
	//};

	RDEFINE_ENUM(ApplicationEventType);
	RENUM_CPP(ApplicationEventType, Values, Values::OnSystemsInitialized + 1)
	{
		"Invalid",
		"OnStart",
		"OnStop",
		"OnSystemsInitialized"
	};
}