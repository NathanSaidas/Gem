#ifndef GEM_TRACE_H
#define GEM_TRACE_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains a Trace struct which holds information about a file.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Trace Struct
#pragma endregion

#include "GemAPI.h"

namespace Gem
{
	namespace Debugging
	{

		/**
		* Holds information about a trace.
		*/
		struct GEM_API Trace 
		{
			Trace(const char * aFilename, const int aLine)
				: filename(aFilename), line(aLine)
			{

			}
			~Trace(){}

			/**
			* The name of the file from which the trace is happening.
			*/
			const char * filename;
			/**
			* The line at which the trace is pointing at.
			*/
			const int line;
		};

		/**
		* A helper macro to build a trace. 
		* (Note: It's suggested to be using the Gem::Debugging namespace. Otherwise this macro call will need to be prefixed with Gem::Debugging)
		*/
#define GET_TRACE(N) Trace(__FILE__, __LINE__ - N)



	}
}

#endif // GEM_TRACE_H