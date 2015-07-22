#ifndef GEM_SCENE_GRAPH_NOT_INITIALIZED_H
#define GEM_SCENE_GRAPH_NOT_INITIALIZED_H

//============================================================
// Date:			June,	 2,		2015
// Description:		Contains an extended error class for when scene graphs are not initialized.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Class.
#pragma endregion

#include "../../Core/ValueTypes.h"
#include "../../Core/GemAPI.h"
#include "../../Core/Error.h"
#include "../../Core/ErrorConstants.h"
#include "../../Core/Debug.h"

namespace Gem
{
	namespace Debugging
	{

		/**
		* An error implementation for Arguments that are null.
		*/
		class GEM_API SceneGraphNotInitialized : public Error
		{

		public:
			/** 
			* @param aArgumentName The name of the scene graph variable that is not initialized.
			* @param aErrorCode The error code that represents this error. 
			* @param aTrace The file trace of where this error occured.
			* @param aMethodFullname The full name of the method of where this error occured.
			*/
			SceneGraphNotInitialized(CString aArgumentName, const Trace aErrorTrace, CString aMethodFullName)
				: Error("Scene graph has not been initialized. Member m_Top is null.", ErrorConstants::SceneGraphNotInitialized, aErrorTrace, aMethodFullName),
				m_ArgumentName(aArgumentName)
			{

			}
			~SceneGraphNotInitialized()
			{

			}

			void Log(CString aHeader)
			{
				Debug::LogFormat(aHeader, nullptr, "Error[%d]: %s at %s\nFile: %s\nLine%d",
					GetErrorCode(),
					GetErrorString(),
					GetMethodFullName(),
					GetErrorTrace().filename,
					GetErrorTrace().line);
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
		class ErrorType<SceneGraphNotInitialized>
		{
			static bool IsErrorType() { return true; }
			static bool IsArgumentNull() { return false; }
			static bool IsInvalidArgument(){ return false; }
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

#endif // GEM_SCENE_GRAPH_NOT_INITIALIZED_H