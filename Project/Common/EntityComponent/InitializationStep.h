#ifndef GEM_INITIALIZATION_STEP_H
#define GEM_INITIALIZATION_STEP_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Enum.h"

namespace Gem
{

	FORCE_EXPORT_META(InitializationStep);

	class GEM_API InitializationStep : public Enum
	{
	public:
		enum GEM_API Values
		{
			Initialized,
			LateInitialization,
			Complete
		};
		RDECLARE_ENUM(InitializationStep)
		RENUM_H(InitializationStep, Values, Values::Complete + 1)
	};

	TYPE_DEFINE(InitializationStep)
}

#endif