#ifndef GEM_FILE_NOT_FOUND_H
#define GEM_FILE_NOT_FOUND_H

//============================================================
// Date:			June,	 2,		2015
// Description:		Contains an extended error class for when a filepath could not be found.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Class.
#pragma endregion

#include "../Error.h"


namespace Gem
{
	namespace Debugging
	{
		FORCE_EXPORT_META(FileNotFound);

		/**
		* An error implementation for Arguments that are null.
		*/
		class GEM_API FileNotFound : public Error
		{
			RDECLARE_CLASS(FileNotFound)
		public:

			FileNotFound() : Error()
			{

			}

			/**
			* @param aArgumentName The variable name of the invalid argument.
			* @param aFilename The name of the file that was not found.
            * @param aErrorCode The error code that represents this error.
			* @param aTrace The file trace of where this error occured.
			* @param aMethodFullname The full name of the method of where this error occured.
			*/
			FileNotFound(CString aArgumentName, CString aFilename, const Trace aErrorTrace, CString aMethodFullName)
				: Error("File was not found.", ErrorConstants::FileNotFound, aErrorTrace, aMethodFullName),
				m_ArgumentName(aArgumentName)
			{

			}
			~FileNotFound()
			{

			}

			void Log(CString aHeader) override;

			/**
			* The name of the argument that is null.
			*/
			CString GetArgumentName() const
			{
				return m_ArgumentName;
			}

            CString GetFilename() const
            {
                return m_Filename;
            }

		private:
			CString m_ArgumentName;
            CString m_Filename;

		};

		/**
		* Define the ErrorType compile time constants for ArgumentNull.
		*/
		template<>
		class ErrorType<FileNotFound>
		{
			static bool IsErrorType() { return true; }
			static bool IsArgumentNull() { return false; }
			static bool IsInvalidArgument(){ return false; }
			static bool IsEntityComponentError(){ return false; }
			static bool IsApplicationError() { return true; }
			static bool IsGraphicsError() { return false; }
			static bool IsMemoryError() { return false; }
			static bool IsWindowError() { return false; }
			static bool IsReflectionError() { return false; }
			static bool IsPhysicsError() { return false; }
			static bool IsAudioError() { return false; }
		};
	}

	TYPE_DEFINE_NAMED(Debugging::FileNotFound, "FileNotFound")
}

#endif // GEM_SCENE_GRAPH_NOT_INITIALIZED_H