#include "Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include "Time.h"

namespace Gem
{
	namespace Debugging
	{
		Debug::Debug()
		{

		}

		Debug::~Debug()
		{

		}

		void Debug::Log(const char * aHeader, const char * aMessage, const char * aFilename)
		{
			TimeStruct time = Time::GetTimeStruct();
			printf("[%d:%d:%d][Log][%s]: %s\n",time.hours,time.minutes,time.seconds,aHeader,aMessage);
		}
		void Debug::Warning(const char * aHeader, const char * aMessage, const char * aFilename )
		{

		}
		void Debug::Error(const char * aHeader, const char * aMessage, const char * aFilename)
		{

		}

		void Debug::LogFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...)
		{

		}
		void Debug::LogFormat(const char * aHeader, const char * aFormat, ...)
		{

		}
		void Debug::WarningFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...)
		{

		}
		void Debug::WarningFormat(const char * aHeader, const char * aFormat, ...)
		{

		}
		void Debug::ErrorFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...)
		{

		}
		void Debug::ErrorFormat(const char * aHeader, const char * aFormat, ...)
		{

		}
	}
}