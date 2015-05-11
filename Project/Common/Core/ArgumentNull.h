#ifndef GEM_ARGUMENT_NULL_H
#define GEM_ARGUMENT_NULL_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains an extended error class, ArgumentNull. 
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
		* An error implementation for Arguments that are null.
		*/
		class GEM_API ArgumentNull : public Error
		{
		public:
			ArgumentNull(const char * aArgumentName, const char * aErrorString, const int aErrorCode, const Trace aErrorTrace, const char * aMethodFullName)
				: Error(aErrorString, aErrorCode, aErrorTrace, aMethodFullName), m_ArgumentName(aArgumentName)
			{

			}
			~ArgumentNull()
			{

			}
			
			/**
			* The name of the argument that is null.
			*/
			const char * GetArgumentName() const
			{
				return m_ArgumentName;
			}

		private:
			const char * m_ArgumentName;

		};

		/**
		* Define the ErrorType compile time constants for ArgumentNull.
		*/
		template<>
		class ErrorType<ArgumentNull>
		{
			static bool IsErrorType() { return true; }
			static bool IsArgumentNull() { return true; }
			static bool IsInvalidArgument(){ return false; }
		};
	}
}

#endif // GEM_ARGUMENT_NULL_H