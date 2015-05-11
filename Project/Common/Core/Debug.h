#ifndef GEM_DEBUG_H
#define GEM_DEBUG_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains a helper class and methods for logging information.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Stubbed Out Class Methods
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
			static void Log(const char * aHeader, const char * aMessage, const char * aFilename = nullptr);
			static void Warning(const char * aHeader, const char * aMessage, const char * aFilename = nullptr);
			static void Error(const char * aHeader, const char * aMessage, const char * aFilename = nullptr);

			static void LogFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...);
			static void LogFormat(const char * aHeader, const char * aFormat, ...);
			static void WarningFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...);
			static void WarningFormat(const char * aHeader, const char * aFormat, ...);
			static void ErrorFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...);
			static void ErrorFormat(const char * aHeader, const char * aFormat, ...);

		private:
			Debug();
			

		};
	}
}

#endif // GEM_DEBUG_H