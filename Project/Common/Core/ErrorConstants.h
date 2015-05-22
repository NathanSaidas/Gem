#ifndef GEM_ERROR_CONSTANTS_H
#define GEM_ERROR_CONSTANTS_H

#include "GemAPI.h"
#include "Enum.h"

namespace Gem
{
	namespace Debugging
	{
		class ErrorConstants
		{
		public:
			//Arguments
			static const int INVALID_ARGUMENT = 1;
			static const int ARGUMENT_NULL = 2;

			//Memory
			
			//Reflection

			//Graphics

			//Window
			static const int FAILED_WINDOW_CREATION = 3;
			static const int LEAKED_WINDOW_MEMORY = 4;

			//Entity Component
		};

		//TODO(Nathan) Implement this method of constants insttead of the other.
		class GEM_API ErrorConstants2 : public Enum
		{
		public:
			enum GEM_API Values
			{
				InvalidArgument,
				ArgumentNull,

				FailedWindowCreation,
				LeakedWindowMemory,

				InvalidGameObjectInstantiation,

				Empty
			};

			RDECLARE_ENUM(ErrorConstants2)
			RENUM_H(ErrorConstants2, Values, Values::Empty + 1)

		};

	}
}

#endif