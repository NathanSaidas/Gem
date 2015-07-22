#ifndef GEM_FAILED_WINDOW_CREATION_H
#define GEM_FAILED_WINDOW_CREATION_H

#include "../../Core/Error.h"

namespace Gem
{
	namespace Debugging
	{
		FORCE_EXPORT_META(FailedWindowCreation);

		class FailedWindowCreation : public Error
		{
			RDECLARE_CLASS(FailedWindowCreation)
		public:

			FailedWindowCreation() : Error()
			{

			}

			FailedWindowCreation(const Trace aTrace, CString aFullname) : Error("Failed to create a window.", ErrorConstants::FailedWindowCreation, aTrace, aFullname)
			{

			}


		private:

		};
	}
	

	TYPE_DEFINE_NAMED(Debugging::FailedWindowCreation, "FailedWindowCreation")
}

#endif