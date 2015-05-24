#ifndef GEM_TIME_H
#define GEM_TIME_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains the TimeStruct typedef and a Time utility class which provides time information about the application.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Stubbed and Implemented class methods.
// -- Nathan Hanlan - Moved Update method to private, Application is now a friend of Time.
#pragma endregion

#include "GemAPI.h"

namespace Gem
{
	/**
	* TimeStruct definition.
	* Provides data about the current time in seconds, minutes and hours.
	*/
	typedef GEM_API struct 
	{
		int hours;
		int minutes;
		int seconds;
	} TimeStruct;
	
	/**
	* Utility class that provides time information such as delta time between frames
	* and total time running.
	*/
	class GEM_API Time
	{
	public:
		~Time();

		/**
		* @return Returns the delta time between frames. Relys on Initialize and Update being called.
		*/
		static float GetDeltaTime();
		/**
		* @return Returns the total execution time in seconds. Relys on Initialize and Update being called.
		*/
		static float GetTime();
		/**
		* @return Returns the TimeStruct which contains information about the total execution time in seconds, minutes and hours.
		*/
		static TimeStruct GetTimeStruct();
		/**
		* @return Returns a status on whether or not the class initialized properly.
		*/
		static bool Initialize();
		
	private:
		Time();

		/**
		* Call this method once per frame to update the timer variables.
		*/
		static void Update();
		
		//-- Static members used by the functions. They are cached each frame to reduce function calls and repeated calculations.
		static bool s_Initialized;
		static float s_DeltaTime;
		static float s_CurrentTime;
		static TimeStruct s_CurrentTimeStruct;

		friend class Application;
		
	};
}

#endif // GEM_TIME_H