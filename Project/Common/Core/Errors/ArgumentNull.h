#ifndef GEM_ARGUMENT_NULL_H
#define GEM_ARGUMENT_NULL_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains an extended error class, ArgumentNull. 
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Class.
#pragma endregion

#include "../GemAPI.h"
#include "../Error.h"

namespace Gem
{
	namespace Debugging
	{
		FORCE_EXPORT_META(ArgumentNull);

		/**
		* An error implementation for Arguments that are null.
		*/
		class GEM_API ArgumentNull : public Error
		{
			RDECLARE_CLASS(ArgumentNull)
		public:
			ArgumentNull() : Error()
			{

			}

			ArgumentNull(const char * aArgumentName, const Trace aErrorTrace, const char * aMethodFullName)
				: Error("Argument is null", ErrorConstants::ArgumentNull, aErrorTrace, aMethodFullName), m_ArgumentName(aArgumentName)
			{

			}
			~ArgumentNull()
			{

			}

			void Log(CString aHeader) override;
			
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
			static bool IsEntityComponentError(){ return false; }
			static bool IsApplicationError() { return false; }
			static bool IsGraphicsError() { return false; }
			static bool IsMemoryError() { return false; }
			static bool IsWindowError() { return false; }
			static bool IsReflectionError() { return false; }
			static bool IsPhysicsError() { return false; }
			static bool IsAudioError() { return false; }
		};
	}

	TYPE_DEFINE_NAMED(Debugging::ArgumentNull, "ArgumentNull")
}

#endif // GEM_ARGUMENT_NULL_H