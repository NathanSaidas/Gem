#include "InputAxis.h"
#include "Input.h"
#include "../Core/Time.h"
#include "../Core/Debug.h"

using namespace Gem::Debugging;


namespace Gem
{
	RDEFINE_CLASS(InputAxis,Object)
	InputAxis::InputAxis() : Object()
	{

	}
	InputAxis::InputAxis(const std::string & aName, KeyCode aPositiveKey, KeyCode aNegativeKey, Float32 aSpeed, bool aResetOnRelease)
		: Object(),
		m_Speed(aSpeed),
		m_ResetOnRelease(aResetOnRelease),
		m_MousePosition(Vector2::Zero()),
		m_Value(0.0f),
		m_InputDeviceType(InputDeviceType::Key),
		m_MouseX(false)
	{
		SetName(aName);
		m_Positive.enabled = aPositiveKey != KeyCode::None;
		m_Positive.keyCode = aPositiveKey;
		m_Positive.mouseButton = MouseButton::None;

		m_Negative.enabled = aNegativeKey != KeyCode::None;
		m_Negative.keyCode = aNegativeKey;
		m_Negative.mouseButton = MouseButton::None;
	}
	InputAxis::InputAxis(const std::string & aName, MouseButton aPositiveKey, MouseButton aNegativeKey, Float32 aSpeed, bool aResetOnRelease)
		: Object(),
		m_Speed(aSpeed),
		m_ResetOnRelease(aResetOnRelease),
		m_MousePosition(Vector2::Zero()),
		m_Value(0.0f),
		m_InputDeviceType(InputDeviceType::MouseButton),
		m_MouseX(false)
	{
		SetName(aName);

		m_Positive.enabled = aPositiveKey != MouseButton::None;
		m_Positive.keyCode = KeyCode::None;
		m_Positive.mouseButton = aPositiveKey;

		m_Negative.enabled = aNegativeKey != MouseButton::None;
		m_Negative.keyCode = KeyCode::None;
		m_Negative.mouseButton = aNegativeKey;
	}
	InputAxis::InputAxis(const std::string & aName, bool aMouseX)
		: Object(),
		m_Speed(0.0f),
		m_ResetOnRelease(false),
		m_MousePosition(Input::GetMousePosition()),
		m_Value(0.0f),
		m_InputDeviceType(InputDeviceType::MousePosition),
		m_MouseX(aMouseX)
	{
		SetName(aName);

		m_Positive.enabled = false;
		m_Positive.keyCode = KeyCode::None;
		m_Positive.mouseButton = MouseButton::None;

		m_Negative.enabled = false;
		m_Negative.keyCode = KeyCode::None;
		m_Negative.mouseButton = MouseButton::None;
	}
	InputAxis::~InputAxis()
	{

	}
	void InputAxis::Update()
	{
		switch (m_InputDeviceType)
		{
		case InputDeviceType::Key:
			UpdateKey();
			break;
		case InputDeviceType::MouseButton:
			UpdateMouseButton();
			break;
		case InputDeviceType::MousePosition:
			UpdateMousePosition();
			break;
		}
	}

	void InputAxis::UpdateKey()
	{
		bool positiveDown = m_Positive.enabled ? Input::GetKey(m_Positive.keyCode) : false;
		bool negativeDown = m_Negative.enabled ? Input::GetKey(m_Negative.keyCode) : false;
		//They cancel each other out.
		if (positiveDown && negativeDown)
		{
			if (m_Value > 0.0f)
			{
				m_Value = Math::Clamp(m_Value - Time::GetDeltaTime() *m_Speed, 0.0f, 1.0f);
			}
			else if (m_Value < 0.0f)
			{
				m_Value = Math::Clamp(m_Value + Time::GetDeltaTime() *m_Speed, -1.0f, 0.0f);
			}
			return;
		}
		if (positiveDown)
		{
			m_Value = Math::Clamp(m_Value + Time::GetDeltaTime() * m_Speed, -1.0f, 1.0f);
		}
		else if (negativeDown)
		{
			m_Value = Math::Clamp(m_Value - Time::GetDeltaTime() * m_Speed, -1.0f, 1.0f);
		}
		else
		{
			if (m_ResetOnRelease)
			{
				m_Value = 0.0f;
			}
			else
			{
				if (m_Value > 0.0f)
				{
					m_Value = Math::Clamp(m_Value - Time::GetDeltaTime() *m_Speed, 0.0f, 1.0f);
				}
				else if (m_Value < 0.0f)
				{
					m_Value = Math::Clamp(m_Value + Time::GetDeltaTime() *m_Speed, -1.0f, 0.0f);
				}
			}
		}
	}
	void InputAxis::UpdateMouseButton()
	{
		bool positiveDown = m_Positive.enabled ? Input::GetMouseButton(m_Positive.mouseButton) : false;
		bool negativeDown = m_Negative.enabled ? Input::GetMouseButton(m_Negative.mouseButton) : false;
		//They cancel each other out.
		if (positiveDown && negativeDown)
		{
			return;
		}
		if (positiveDown)
		{
			m_Value = Math::Clamp(m_Value + Time::GetDeltaTime() * m_Speed, -1.0f, 1.0f);
		}
		else if (negativeDown)
		{
			m_Value = Math::Clamp(m_Value - Time::GetDeltaTime() * m_Speed, -1.0f, 1.0f);
		}
		else
		{
			if (m_ResetOnRelease)
			{
				m_Value = 0.0f;
			}
			else
			{
				if (m_Value > 0.0f)
				{
					m_Value = Math::Clamp(m_Value - Time::GetDeltaTime() *m_Speed, 0.0f, 1.0f);
				}
				else if (m_Value < 0.0f)
				{
					m_Value = Math::Clamp(m_Value + Time::GetDeltaTime() *m_Speed, -1.0f, 0.0f);
				}
			}
		}
	}
	void InputAxis::UpdateMousePosition()
	{
		Vector2 mousePosition = Input::GetMousePosition();

		if (m_MouseX)
		{
			m_Value = mousePosition.x - m_MousePosition.x; // Math::Abs(mousePosition - m_MousePosition).Normalized().x;
		}
		else
		{
			m_Value = mousePosition.y - m_MousePosition.y; // Math::Abs(mousePosition - m_MousePosition).Normalized().y;
		}
		m_MousePosition = mousePosition;
	}
}