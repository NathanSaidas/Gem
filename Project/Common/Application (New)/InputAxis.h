#ifndef GEM_INPUT_AXIS_H
#define GEM_INPUT_AXIS_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "InputDeviceType.h"
#include "InputDevice.h"

namespace Gem
{

	//FORCE_EXPORT_META(InputAxis);
	/**
	* This class is used to map input from hardware to strings for games and other applications.
	* The axis can be categorized into two types (axis or button). Axis have a value of  input class has  GetAxis, GetButton, GetButtonDown, GetButtonUp
	*/
	class InputAxis : public Object
	{
		RDECLARE_CLASS(InputAxis)
	public:
		InputAxis();
		InputAxis(const std::string & aName, KeyCode aPositiveKey, KeyCode aNegativeKey, Float32 aSpeed, bool aResetOnRelease);
		InputAxis(const std::string & aName, MouseButton aPositiveKey, MouseButton aNegativeKey, Float32 aSpeed, bool aResetOnRelease);
		InputAxis(const std::string & aName, bool aMouseX);
		~InputAxis();
		void Update();

		inline bool GetResetOnRelease() const
		{
			return m_ResetOnRelease;
		}
		
		inline Float32 GetSpeed() const
		{
			return m_Speed;
		}

		inline KeyCode GetPositiveKey() const
		{
			return m_Positive.keyCode;
		}

		inline KeyCode GetNegativeKey() const
		{
			return m_Negative.keyCode;
		}

		inline MouseButton GetPositiveButton() const
		{
			return m_Positive.mouseButton;
		}

		inline MouseButton GetNegativeButton() const
		{
			return m_Negative.mouseButton;
		}

		inline InputDeviceType GetInputDeviceType() const
		{
			return m_InputDeviceType;
		}

		inline Float32 GetValue() const
		{
			return m_Value;
		}

		inline void SetResetOnRelease(bool aResetOnRelease)
		{
			m_ResetOnRelease = aResetOnRelease;
		}

		inline void SetSpeed(Float32 aSpeed)
		{
			m_Speed = aSpeed;
		}


	private:
		/** Whether or not to reset to 0 when the there is no input*/
		bool m_ResetOnRelease;
		/** The speed at which the value changes at.*/
		Float32 m_Speed;
		/** The positive axis*/
		InputDevice m_Positive;
		/** The negative axis*/
		InputDevice m_Negative;
		/** The position of the mouse */
		Vector2 m_MousePosition;
		/** The type of axis to use.*/
		InputDeviceType m_InputDeviceType;
		/** The value of this axis.*/
		Float32 m_Value;
		/** A flag used for determining whether or not to track delta mouse x position or mouse y*/
		bool m_MouseX;

		void UpdateKey();
		void UpdateMouseButton();
		void UpdateMousePosition();

		
	};

	TYPE_DEFINE(InputAxis)
}

#endif