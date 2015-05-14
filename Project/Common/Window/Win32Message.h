#ifndef GEM_WIN32_MESSAGE_H
#define GEM_WIN32_MESSAGE_H

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
	class Win32Message;
	template class GEM_API Reflection::MetaObject<Win32Message>;

	/**
	* Specify's the type of application there are.
	*/
	class GEM_API Win32Message : public Enum
	{
	public:
		enum GEM_API Values
		{
			Resize,
			Destroy,
			Active,
			Invalid
		};
		RDECLARE_ENUM(Win32Message)
			RENUM_H(Win32Message, Values)
	private:
		static const std::string NAMES[Values::Invalid + 1];
	};

	TYPE_DEFINE(Win32Message)
}

#endif