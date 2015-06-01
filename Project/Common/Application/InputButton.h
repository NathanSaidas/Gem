#ifndef GEM_INPUT_BUTTON_H
#define GEM_INPUT_BUTTON_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "InputDevice.h"
#include "InputDeviceType.h"
#include "InputState.h"


namespace Gem
{
	FORCE_EXPORT_META(InputButton);

	class InputButton : public Object
	{
		RDECLARE_CLASS(InputButton)
	public:

		InputButton();
		InputButton(const std::string & aName, KeyCode aKey);
		InputButton(const std::string & aName, MouseButton aButton);
		InputButton(const std::string & aName);
		~InputButton();


		inline InputDeviceType GetDeviceType() const
		{
			return m_InputDeviceType;
		}

		inline InputDevice GetInputDevice() const
		{
			return m_InputDevice;
		}

		InputState GetState();

	private:
		InputDeviceType m_InputDeviceType;
		InputDevice m_InputDevice;

	};

	TYPE_DEFINE(InputButton)
}

#endif