#ifndef GEM_APPLICATION_H
#define GEM_APPLICATION_H


//============================================================
// Date:			May,	12,		2015
// Description:		The root file for the Application Class. Contains the Application Class as well as typedefs and explicit exporting/importing.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented base features of class Application.
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Event.h"
#include "ApplicationType.h"
#include "ApplicationEventType.h"


namespace Gem
{
	/**
	* The type for a callback for an application event.
	*/
	typedef Func<void> ApplicationEventCallback;

	/**
	* The type for an ApplicationEvent;
	*/
	typedef Event<> ApplicationEvent;
	
	/**
	* Export Application Meta data.
	*/
	class Application;
	template class GEM_API Reflection::MetaObject<Application>;

	/**
	* Export ApplicationEvent type.
	*/
	template class GEM_API Event<>;
	
	/**
	* The class that runs the entire application.
	*
	*
	* Call RegisterEvent to register for events ahead of time before calling Execute.
	* It's not necessary to UnregisterEvents the memory is all managed.
	*/
	class GEM_API Application : public object
	{
		//Declare class meta data.
		RDECLARE_CLASS(Application)
	public:
		~Application();

		/**
		* Call this to start the application.
		* Note: This will have no effect if it's already being called and will return EXIT_FAILURE
		* @param aApplicationName The name of the application
		* @param aType The type of application this is.
		* @return returns a UInt32 value which specifys how the Application got terminated.
		*/
		static UInt32 Execute(const std::string & aApplicationName, const ApplicationType & aType);

		/**
		* Register for ApplicationEvents with this method.
		* @param aEventType The type of event to register for.
		* @param aCallback The callback that will receive the event call.
		*/
		static void RegisterEvent(const ApplicationEventType & aEventType, const ApplicationEventCallback & aCallback);
		/**
		* Unregister for ApplicationEvents with this method.
		* @param aEventType The type of event to unregister from.
		* @param aCallback The callback that will receive the event call.
		*/
		static void UnregisterEvent(const ApplicationEventType & aEventType, const ApplicationEventCallback & aCallback);

		/**
		* Retrieves the type of application this is.
		* @return Returns the type of application this is.
		*/
		static const ApplicationType GetApplicationType();

		/**
		* Retrieves the name of the application
		* @return Returns the type of application this is.
		*/
		static const std::string GetApplicationName();

	protected:
		/**
		* The static singleton instance.
		*/
		static Application * s_Instance;
		
		// -- Events
		/**
		* This gets called immediately after StartWindow or StartConsole gets called.
		* Systems have not been initialized and its suggested not to use them.
		*/
		static Event<> s_OnStart;
		/**
		* This gets called immediately before StartWindow or StartConsole returns.
		*/
		static Event<> s_OnStop;

		/**
		* This gets called immediately after all systems have been initialized.
		*/
		static Event<> s_OnSystemsInitialized;

		// -- End Events
		Application();

		/**
		* The name of the application.
		*/
		std::string m_ApplicationName;
		/**
		* The type of application
		*/
		ApplicationType m_ApplicationType;
		/**
		* The code for exit.
		*/
		UInt32 m_ExitCode;

		/**
		* Starts the application as if it were a window.
		*/
		void StartWindow();
		/**
		* Starts the application as if it were a console.
		*/
		void StartConsole();

		

	private:
		friend class Gem::Reflection::MetaObject<Application>;
	};

	TYPE_DEFINE(Application)
	TYPE_DEFINE(Event<>)
}

#endif