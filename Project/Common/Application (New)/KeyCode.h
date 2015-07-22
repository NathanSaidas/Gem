#ifndef GEM_KEY_CODE_H
#define GEM_KEY_CODE_H

#include "glew-1.10.0\include\GL\glew.h"
#include "GLFW_3.0.4\include\GLFW\glfw3.h"

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
		None    = 0,
		Return  =       GLFW_KEY_ENTER,     // 0x0D,
		Escape  =       GLFW_KEY_ESCAPE,    // 0x1B,
		Left    =       GLFW_KEY_LEFT,      // 0x25,
		Up      =       GLFW_KEY_UP,       
		Right   =       GLFW_KEY_RIGHT, 
		Down    =       GLFW_KEY_DOWN,
		Delete  =       GLFW_KEY_DELETE,    // 0x2E,
        Backspace =     GLFW_KEY_BACKSPACE,
		Alpha0  =       GLFW_KEY_0,         // 0x30,
		Alpha1,
		Alpha2,
		Alpha3,
		Alpha4,
		Alpha5,
		Alpha6,
		Alpha7,
		Alpha8,
		Alpha9,
		A       =       GLFW_KEY_A,         // 0x41,
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
		Numpad0 =       GLFW_KEY_KP_0,      // 0x60,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1      =       GLFW_KEY_F1,        // 0x70,
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
		LeftShift       =   GLFW_KEY_LEFT_SHIFT,    //0xA0,
		RightShift      =   GLFW_KEY_RIGHT_SHIFT,   //0xA1,
		LeftControl     =   GLFW_KEY_LEFT_CONTROL,  //0xA2,
		RightControl    =   GLFW_KEY_RIGHT_CONTROL, //0xA3,
		LeftAlt         =   GLFW_KEY_LEFT_ALT,      //0xA4,
		RightAlt        =   GLFW_KEY_RIGHT_ALT,     //0xA5
	};
}

#endif