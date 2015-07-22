#ifndef GEM_INPUT_STATE_H
#define GEM_INPUT_STATE_H

//============================================================
// Date:			May,	24,		2015
// Description:		Defines an enum for InputStates.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented InputStates
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Enum.h"

namespace Gem
{

	//FORCE_EXPORT_META(InputState);

	class InputState : public Enum
	{
	public:
		enum Values
		{
			Down,
			Up,
			Pressed,
			Released
		};
		RDECLARE_ENUM(InputState)
		RENUM_H(InputState, Values, Values::Released + 1)
	};
}

#endif