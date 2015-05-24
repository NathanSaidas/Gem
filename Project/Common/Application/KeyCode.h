#ifndef GEM_KEY_CODE_H
#define GEM_KEY_CODE_H

//============================================================
// Date:			May,	12,		2015
// Description:		Defines constants for KeyCodes. These are platform dependant.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented KeyCode
#pragma endregion

namespace Gem
{
	//These are keycodes as defined by windows.
	enum class KeyCode
	{
		Return = 0x0D,
		Escape = 0x1B,
		Left = 0x25,
		Up,
		Right,
		Down,
		Delete = 0x2E,
		Alpha0 = 0x30,
		Alpha1,
		Alpha2,
		Alpha3,
		Alpha4,
		Alpha5,
		Alpha6,
		Alpha7,
		Alpha8,
		Alpha9,
		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Numpad0 = 0x60,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1 = 0x70,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		LeftShift = 0xA0,
		RightShift = 0xA1,
		LeftControl = 0xA2,
		RightControl = 0xA3,
		LeftAlt = 0xA4,
		RightAlt = 0xA5
	};
}

#endif