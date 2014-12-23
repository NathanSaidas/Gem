#ifndef G_DEBUG_H
#define G_DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include "../G_Object.h"
namespace Gem
{
	class Debug : public object
	{
		G_CLASS_DEF(Debug)
	public:
		///Logs a message out to the console in log format.
		inline static void Log(const char * aMessage, const char * aFile = "", int aLine = -1)
		{
#ifdef _DEBUG_TRACE_ON
			printf("[Log]: %s\nFile: %s\nLine: %d\n", aMessage, aFile, aLine);
#elif _DEBUG_TRACE_OFF
			printf("[Log]: %s\n", aMessage);
#endif
		}
		///Logs a message out to the console in a warning format
		inline static void Warning(const char * aMessage, const char * aFile = "", int aLine = -1)
		{
#ifdef _DEBUG_TRACE_ON
			printf("[Warning]: %s\nFile: %s\nLine: %d\n", aMessage, aFile, aLine);
#elif _DEBUG_TRACE_OFF
			printf("[Warning]: %s\n", aMessage);
#endif
		}
		///Logs a message out to the console in a error format
		inline static void Error(const char * aMessage, const char * aFile = "", int aLine = -1)
		{
#ifdef _DEBUG_TRACE_ON
			printf("[Error]: %s\nFile: %s\nLine: %d\n", aMessage, aFile, aLine);
#elif _DEBUG_TRACE_OFF
			printf("[Error]: %s\n", aMessage);
#endif
		}
	private:
	};


}

#endif