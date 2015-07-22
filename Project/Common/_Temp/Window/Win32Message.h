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
	class Win32Message : public Enum
	{
	public:
		enum Values
		{
			Resize,
			Destroy,
			Active,
			KeyDown,
			KeyUp,
			MouseDown,
			MouseUp,
			MouseMove,
			MouseWheel,
			Invalid
		};
		RDECLARE_ENUM(Win32Message)
		RENUM_H(Win32Message, Values, Values::Invalid + 1)
	//private:
	//	static const std::string NAMES[Values::Invalid + 1];
	};

	TYPE_DEFINE(Win32Message)
}

#endif