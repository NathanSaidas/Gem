#ifndef GEM_INVALID_ARGUMENT_H
#define GEM_INVALID_ARGUMENT_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains an extended error class, InvalidArgument. 
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Class.
#pragma endregion

#include "GemAPI.h"
#include "Error.h"

namespace Gem
{
	namespace Debugging
	{
		/**
		* An error implementation for invalid arguments.
		*/
		class GEM_API InvalidArgument : public Error
		{
		public:
			InvalidArgument(const char * aArgumentName, const char * aErrorString, const int aErrorCode, const Trace aErrorTrace, const char * aMethodFullName)
				: Error(aErrorString, aErrorCode, aErrorTrace, aMethodFullName), m_ArgumentName(aArgumentName)
			{

			}
			~InvalidArgument()
			{

			}

			/**
			* The name of the argument that is invalid.
			*/
			const char * GetArgumentName() const
			{
				return m_ArgumentName;
			}
		private:
			const char * m_ArgumentName;

		};

		/**
		* Define the ErrorType compile time constants for InvalidArgument.
		*/
		template<>
		class ErrorType<InvalidArgument>
		{
			static bool IsErrorType() { return true; }
			static bool IsArgumentNull() { return false; }
			static bool IsInvalidArgument(){ return true; }
			static bool IsEntityComponentError(){ return true; }
			static bool IsApplicationError() { return false; }
			static bool IsGraphicsError() { return false; }
			static bool IsMemoryError() { return false; }
			static bool IsWindowError() { return false; }
			static bool IsReflectionError() { return false; }
			static bool IsPhysicsError() { return false; }
			static bool IsAudioError() { return false; }
		};
	}
}

#endif