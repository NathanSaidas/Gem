#ifndef GEM_ERROR_H
#define GEM_ERROR_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains the base class for all future error types.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented base class features.
#pragma endregion

#include "Core.h"
#include "Trace.h"
#include "ErrorConstants.h"

namespace Gem
{
	namespace Debugging
	{
		//Base template class for detecting what type of error type each object is.
		template<typename ERROR_TYPE>
		class ErrorType
		{
			static bool IsErrorType() { return false; }
			static bool IsArgumentNull() { return false; }
			static bool IsInvalidArgument(){ return false; }
			static bool IsEntityComponentError() { return false; }
			static bool IsApplicationError() { return false; }
			static bool IsGraphicsError() { return false; }
			static bool IsMemoryError() { return false; }
			static bool IsWindowError() { return false; }
			static bool IsReflectionError() { return false; }
			static bool IsPhysicsError() { return false; }
			static bool IsAudioError() { return false; }
		};

		FORCE_EXPORT_META(Error);

		/**
		* Base class for error objects.
		*/
		class GEM_API Error : public object
		{
			RDECLARE_CLASS(Error)
		public:
			Error() : object(), m_ErrorString(""), m_ErrorCode(ErrorConstants::Empty), m_ErrorTrace(Trace("", 0)), m_MethodFullname("")
			{

			}

			Error(const char * aErrorString, const int aErrorCode, const Trace aErrorTrace, const char * aMethodFullname)
				: object(), m_ErrorString(aErrorString), m_ErrorCode(aErrorCode), m_ErrorTrace(aErrorTrace), m_MethodFullname(aMethodFullname)
			{

			}
			Error(const Error & aError, const Trace aErrorTrace)
				: m_ErrorString(aError.GetErrorString()), m_ErrorCode(aError.GetErrorCode()), m_ErrorTrace(aErrorTrace), m_MethodFullname(aError.GetMethodFullName())
			{

			}

			virtual ~Error(){}

			virtual void Log(CString aHeader);

			/**
			* @ return Returns a string containing information about the error.
			*/
			const char * GetErrorString() const
			{
				return m_ErrorString;
			}

			/**
			* @ return Returns an error code associated with the error.
			*/
			const int& GetErrorCode() const
			{
				return m_ErrorCode;
			}

			/**
			* @ return Returns a trace from which the error was generated.
			*/
			const Trace& GetErrorTrace() const
			{
				return m_ErrorTrace;
			}

			/**
			* @ return Returns the full name of the class and method associated with the error.
			*/
			const char * GetMethodFullName() const
			{
				return m_MethodFullname;
			}

		private:
			const char * m_ErrorString;
			const int m_ErrorCode;
			const Trace m_ErrorTrace;
			const char * m_MethodFullname;
		};									


		/**
		* Define the ErrorType compile time constants for Error.
		*/
		template<>
		class ErrorType<Error>
		{
			static bool IsErrorType() { return true; }
			static bool IsArgumentNull() { return false; }
			static bool IsInvalidArgument(){ return false; }
		};

		
	}

	TYPE_DEFINE_NAMED(Debugging::Error, "Error")
}

#endif // GEM_ERROR_H