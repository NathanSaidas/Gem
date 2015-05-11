#ifndef GEM_DEBUG_H
#define GEM_DEBUG_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains a helper class and methods for logging information.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Stubbed Out Class Methods
// -- Nathan Hanlan - Implemented console output features.
#pragma endregion

#include "GemAPI.h"

namespace Gem
{
	namespace Debugging
	{

		class GEM_API Debug
		{
		public:
			~Debug();
			/**
			* Writes a message to the console in Log format.
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log about.
			* @param aFilename [OPT] The name of the file to log to.
			*/
			static void Log(const char * aHeader, const char * aMessage, const char * aFilename = nullptr);

			/**
			* Writes a message to the console in Warning format.
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log a warning about.
			* @param aFilename [OPT] The name of the file to log a warning to.
			*/
			static void Warning(const char * aHeader, const char * aMessage, const char * aFilename = nullptr);

			/**
			* Writes a message to the console in Error format.
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log a error about.
			* @param aFilename [OPT] The name of the file to log a error to.
			*/
			static void Error(const char * aHeader, const char * aMessage, const char * aFilename = nullptr);

			/**
			* Writes a message to the console in Log format, allows the caller to specify a format to log with. (Just like printf)
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log about.
			* @param aFilename The name of the file to log to.
			* @param ... The arguments associated with aFormat.
			*/
			static void LogFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...);

			/**
			* Writes a message to the console in Log format, allows the caller to specify a format to log with. (Just like printf)
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log about.
			* @param ... The arguments associated with aFormat.
			*/
			static void LogFormat(const char * aHeader, const char * aFormat, ...);

			/**
			* Writes a message to the console in Warning format, allows the caller to specify a format to log with. (Just like printf)
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log a warning about.
			* @param aFilename The name of the file to log a warning to.
			* @param ... The arguments associated with aFormat.
			*/
			static void WarningFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...);

			/**
			* Writes a message to the console in Warning format, allows the caller to specify a format to log with. (Just like printf)
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log a warning about.
			* @param ... The arguments associated with aFormat.
			*/
			static void WarningFormat(const char * aHeader, const char * aFormat, ...);

			/**
			* Writes a message to the console in Error format, allows the caller to specify a format to log with. (Just like printf)
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log a error about.
			* @param aFilename The name of the file to log a error to.
			* @param ... The arguments associated with aFormat.
			*/
			static void ErrorFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...);

			/**
			* Writes a message to the console in Error format, allows the caller to specify a format to log with. (Just like printf)
			* @param aHeader The header of the logging. This can be null, it will just result in not writing a header.
			* @param aMessage The message to log a error about.
			* @param ... The arguments associated with aFormat.
			*/
			static void ErrorFormat(const char * aHeader, const char * aFormat, ...);

		private:
			Debug();
			
			/**
			* The maximum length of the debug string for [LogType]Format methods.. (Currently set to 2048).
			*/
			static const int MAX_DEBUG_STRING_LENGTH;
			

		};
	}
}

#endif // GEM_DEBUG_H