#include "Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include "Time.h"

namespace Gem
{
	namespace Debugging
	{

		const int Debug::MAX_DEBUG_STRING_LENGTH = 2048;

		Debug::Debug()
		{

		}

		Debug::~Debug()
		{

		}

		void Debug::Log(const char * aHeader, const char * aMessage, const char * aFilename)
		{
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Log][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, aMessage);
			}
			else
			{
				printf("[%d:%d:%d][Log]: %s\n", time.hours, time.minutes, time.seconds, aMessage);
			}
		}
		void Debug::Warning(const char * aHeader, const char * aMessage, const char * aFilename )
		{
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Warning][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, aMessage);
			}
			else
			{
				printf("[%d:%d:%d][Warning]: %s\n", time.hours, time.minutes, time.seconds, aMessage);
			}
		}
		void Debug::Error(const char * aHeader, const char * aMessage, const char * aFilename)
		{
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Error][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, aMessage);
			}
			else
			{
				printf("[%d:%d:%d][Error]: %s\n", time.hours, time.minutes, time.seconds, aMessage);
			}
		}

		void Debug::LogFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...)
		{
			char tempbuffer[MAX_DEBUG_STRING_LENGTH];
			va_list args;
			va_start(args, aFormat);
			vsprintf_s(tempbuffer, MAX_DEBUG_STRING_LENGTH,aFormat, args);
			va_end(args);
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Log][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, tempbuffer);
			}
			else
			{
				printf("[%d:%d:%d][Log]: %s\n", time.hours, time.minutes, time.seconds, tempbuffer);
			}
		}
		void Debug::LogFormat(const char * aHeader, const char * aFormat, ...)
		{
			char tempbuffer[MAX_DEBUG_STRING_LENGTH];
			va_list args;
			va_start(args, aFormat);
			vsprintf_s(tempbuffer, MAX_DEBUG_STRING_LENGTH, aFormat, args);
			va_end(args);
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Log][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, tempbuffer);
			}
			else
			{
				printf("[%d:%d:%d][Log]: %s\n", time.hours, time.minutes, time.seconds, tempbuffer);
			}
		}
		void Debug::WarningFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...)
		{
			char tempbuffer[MAX_DEBUG_STRING_LENGTH];
			va_list args;
			va_start(args, aFormat);
			vsprintf_s(tempbuffer, MAX_DEBUG_STRING_LENGTH, aFormat, args);
			va_end(args);
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Warning][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, tempbuffer);
			}
			else
			{
				printf("[%d:%d:%d][Warning]: %s\n", time.hours, time.minutes, time.seconds, tempbuffer);
			}
		}
		void Debug::WarningFormat(const char * aHeader, const char * aFormat, ...)
		{
			char tempbuffer[MAX_DEBUG_STRING_LENGTH];
			va_list args;
			va_start(args, aFormat);
			vsprintf_s(tempbuffer, MAX_DEBUG_STRING_LENGTH, aFormat, args);
			va_end(args);
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Warning][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, tempbuffer);
			}
			else
			{
				printf("[%d:%d:%d][Warning]: %s\n", time.hours, time.minutes, time.seconds, tempbuffer);
			}
		}
		void Debug::ErrorFormat(const char * aHeader, const char * aFormat, const char * aFilename, ...)
		{
			char tempbuffer[MAX_DEBUG_STRING_LENGTH];
			va_list args;
			va_start(args, aFormat);
			vsprintf_s(tempbuffer, MAX_DEBUG_STRING_LENGTH, aFormat, args);
			va_end(args);
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Error][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, tempbuffer);
			}
			else
			{
				printf("[%d:%d:%d][Error]: %s\n", time.hours, time.minutes, time.seconds, tempbuffer);
			}
		}
		void Debug::ErrorFormat(const char * aHeader, const char * aFormat, ...)
		{
			char tempbuffer[MAX_DEBUG_STRING_LENGTH];
			va_list args;
			va_start(args, aFormat);
			vsprintf_s(tempbuffer, MAX_DEBUG_STRING_LENGTH, aFormat, args);
			va_end(args);
			TimeStruct time = Time::GetTimeStruct();
			if (aHeader != nullptr)
			{
				printf("[%d:%d:%d][Error][%s]: %s\n", time.hours, time.minutes, time.seconds, aHeader, tempbuffer);
			}
			else
			{
				printf("[%d:%d:%d][Error]: %s\n", time.hours, time.minutes, time.seconds, tempbuffer);
			}
		}
	}
}