#ifndef GEM_APPLICATION_EVENT_TYPE_H
#define GEM_APPLICATION_EVENT_TYPE_H

//============================================================
// Date:			May,	12,		2015
// Description:		Defines a reflected enum ApplicationEventType
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented ApplicationEventType
#pragma endregion

#include "../Core/Enum.h"

namespace Gem
{
	/**
	* Export the type.
	*/
	class ApplicationEventType;
	template class GEM_API Gem::Reflection::MetaObject<ApplicationEventType>;

	/**
	* Specify's the type of events there are for the Application Class.
	*/
	class GEM_API ApplicationEventType : public Enum
	{
	public:
		enum GEM_API Values
		{
			Invalid,
			OnStart,
			OnStop,
			OnSystemsInitialized,
		};
		RDECLARE_ENUM(ApplicationEventType)
			RENUM_H(ApplicationEventType, Values)
	private:
		static const std::string NAMES[Values::OnSystemsInitialized + 1];
	};

	TYPE_DEFINE(ApplicationEventType)
}

#endif // GEM_APPLICATION_EVENT_H