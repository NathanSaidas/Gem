#include "InstructionTokenID.h"

namespace Gem
{
	//const std::string InstructionTokenID::NAMES[Values::Invalid + 1] =
	//{
	//	"AddGameObject",
	//	"LinkGameObject",
	//	"AddComponent",
	//	"LinkComponent",
	//	"Invalid"
	//};

	RDEFINE_ENUM(InstructionTokenID);
	RENUM_CPP(InstructionTokenID, Values, Values::Invalid + 1) 
	{
		"AddGameObject",
		"LinkGameObject",
		"AddComponent",
		"LinkComponent",
		"Invalid"
	};
}