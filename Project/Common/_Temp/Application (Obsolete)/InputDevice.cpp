#include "InputDevice.h"

namespace Gem
{
	RDEFINE_CLASS(InputDevice, object)
	InputDevice::InputDevice() : object(),
	keyCode(KeyCode::None),
	mouseButton(MouseButton::Left),
	enabled(false)
	{

	}
	InputDevice::~InputDevice()
	{

	}
}