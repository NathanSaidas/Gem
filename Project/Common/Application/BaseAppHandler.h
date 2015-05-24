#ifndef GEM_BASE_APP_HANDLER_H
#define GEM_BASE_APP_HANDLER_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"

namespace Gem
{
	/** Force the BaseAppHandler class export its meta data. (This is not necessary).*/
	FORCE_EXPORT_META(BaseAppHandler);

	/**
	*	This is the base class for all application handlers.
	*/
	class GEM_API BaseAppHandler : public object
	{
		/** Declare the meta class data of BaseAppHandler */
		RDECLARE_CLASS(BaseAppHandler)
	public:
		
		BaseAppHandler();
		virtual ~BaseAppHandler();

		/** This method gets called when the application has started. Before any systems are initialized.*/
		virtual void OnStart();
		/** This method gets called when the application has stopped. Before any systems are terminated.*/
		virtual void OnStop();
		/** This method gets called when the application has had its systems initialized. */
		virtual void OnSystemsInitialized();
		/** This method gets called when the application is updating.*/
		virtual void Update();
	};

	/** Define additional meta data for the class.*/
	TYPE_DEFINE(BaseAppHandler)
}

#endif