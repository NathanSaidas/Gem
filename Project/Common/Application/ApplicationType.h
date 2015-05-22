#ifndef GEM_APPLICATION_TYPE_H
#define GEM_APPLICATION_TYPE_H

//============================================================
// Date:			May,	12,		2015
// Description:		Defines the reflected enum ApplicationType
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented ApplicationType
#pragma endregion

#include "../Core/Enum.h"

namespace Gem
{
	/**
	* Export the type.
	*/
	class ApplicationType;
	template class GEM_API Gem::Reflection::MetaObject<ApplicationType>;

	/**
	* Specify's the type of application there are.
	*/
	class GEM_API ApplicationType : public Enum
	{
	public:
		enum GEM_API Values
		{
			Invalid,
			Console,
			Window,
		};
		RDECLARE_ENUM(ApplicationType)
		RENUM_H(ApplicationType,Values, Values::Window + 1)
	//private:
	//	static const std::string NAMES[Values::Window + 1];
	};

	TYPE_DEFINE(ApplicationType)
}

#endif