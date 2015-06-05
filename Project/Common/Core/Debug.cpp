#include "Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include "Time.h"
#include "Error.h"

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#endif

namespace Gem
{
	namespace Debugging
	{

		const int Debug::MAX_DEBUG_STRING_LENGTH = 2048;
		bool Debug::s_IsCreated = false;

		Debug::Debug()
		{

		}

		Debug::~Debug()
		{

		}

		void Debug::Log(const char * aHeader, const char * aMessage, const char * aFilename)
		{
			CreateConsole();
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
			CreateConsole();
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
			CreateConsole();
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

		void Debug::LogFormat(const char * aHeader, const char * aFilename, const char * aFormat, ...)
		{
			CreateConsole();
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
			CreateConsole();
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
		void Debug::WarningFormat(const char * aHeader, const char * aFilename, const char * aFormat, ...)
		{
			CreateConsole();
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
			CreateConsole();
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
		void Debug::ErrorFormat(const char * aHeader, const char * aFilename, const char * aFormat, ...)
		{
			CreateConsole();
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
			CreateConsole();
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

		void Debug::Error(const char * aHeader, Debugging::Error * aError)
		{
			if (aError != nullptr)
			{
				aError->Log(aHeader);
			}
		}

		void Debug::CreateConsole()
		{
			if (s_IsCreated)
			{
				return;
			}
#ifdef WIN32
			AllocConsole();
			SetConsoleTitle("Debug Console");

			int consoleHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x400);
			FILE * filePointer = _fdopen(consoleHandle, "w");
			*stdout = *filePointer;
			setvbuf(stdout, NULL, _IONBF, 0);
#endif

			s_IsCreated = true;
		}
	}
}