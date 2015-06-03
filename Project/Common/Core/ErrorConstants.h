#ifndef GEM_ERROR_CONSTANTS_H
#define GEM_ERROR_CONSTANTS_H

#include "GemAPI.h"
#include "Enum.h"

namespace Gem
{
	namespace Debugging
	{

		//TODO(Nathan) Implement this method of constants insttead of the other.
		class GEM_API ErrorConstants : public Enum
		{
		public:
			enum GEM_API Values
			{
				//Arguments
				InvalidArgument,
				ArgumentNull,

				//Windows
				FailedWindowCreation,
				LeakedWindowMemory,

				//Entity Component
				InvalidGameObjectInstantiation,
				SceneGraphNotInitialized,
				Empty
			};

			RDECLARE_ENUM(ErrorConstants)
			RENUM_H(ErrorConstants, Values, Values::Empty + 1)

		};

	}
}

#endif