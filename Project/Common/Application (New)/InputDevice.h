#ifndef GEM_INPUT_DEVICE_H
#define GEM_INPUT_DEVICE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "KeyCode.h"
#include "MouseButton.h"
#include "../Math/Vector2.h"

namespace Gem
{
	//FORCE_EXPORT_META(InputDevice);

	class InputDevice : public object
	{
		RDECLARE_CLASS(InputDevice)
	public:
		InputDevice();
		~InputDevice();
		KeyCode keyCode;
		MouseButton mouseButton;
		bool enabled;
	};

	TYPE_DEFINE(InputDevice)
}

#endif
