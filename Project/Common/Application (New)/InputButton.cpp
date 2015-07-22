#include "InputButton.h"
#include "Input.h"

namespace Gem
{
	RDEFINE_CLASS(InputButton,Object)

	InputButton::InputButton()
		: m_InputDeviceType(InputDeviceType::Key)
	{
		m_InputDevice.enabled = false;
		m_InputDevice.keyCode = KeyCode::None;
		m_InputDevice.mouseButton = MouseButton::None;
		SetName("");
	}
	InputButton::InputButton(const std::string & aName, KeyCode aKey)
		: m_InputDeviceType(InputDeviceType::Key)
	{
		m_InputDevice.enabled = aKey != KeyCode::None;
		m_InputDevice.keyCode = aKey;
		m_InputDevice.mouseButton = MouseButton::None;
		SetName(aName);
	}
	InputButton::InputButton(const std::string & aName, MouseButton aButton)
		: m_InputDeviceType(InputDeviceType::Key)
	{
		m_InputDevice.enabled = aButton != MouseButton::None;
		m_InputDevice.keyCode = KeyCode::None;
		m_InputDevice.mouseButton = aButton;
		SetName(aName);

	}
	InputButton::InputButton(const std::string & aName)
		: m_InputDeviceType(InputDeviceType::Key)
	{
		SetName(aName);
		m_InputDevice.enabled = false;
		m_InputDevice.keyCode = KeyCode::None;
		m_InputDevice.mouseButton = MouseButton::None;
	}
	InputButton::~InputButton()
	{

	}

	InputState InputButton::GetState()
	{
		if (m_InputDeviceType == InputDeviceType::Key)
		{
			if (Input::GetKeyDown(m_InputDevice.keyCode))
			{
				return InputState::Pressed;
			}
			else if (Input::GetKeyUp(m_InputDevice.keyCode))
			{
				return InputState::Released;
			}
			else if (Input::GetKey(m_InputDevice.keyCode))
			{
				return InputState::Down;
			}
			else
			{
				return InputState::Up;
			}
		}
		else if (m_InputDeviceType == InputDeviceType::MouseButton)
		{
			if (Input::GetMouseButtonDown(m_InputDevice.mouseButton))
			{
				return InputState::Pressed;
			}
			else if (Input::GetMouseButtonUp(m_InputDevice.mouseButton))
			{
				return InputState::Released;
			}
			else if (Input::GetMouseButton(m_InputDevice.mouseButton))
			{
				return InputState::Down;
			}
			else
			{
				return InputState::Up;
			}
		}
		return InputState::Up;
	}


}