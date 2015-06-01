#ifndef GEM_INPUT_H
#define GEM_INPUT_H

//============================================================
// Date:			May,	24,		2015
// Description:		Defines the class used for Input Operations.
// Use the static methods provided to get access to input states.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented basic features of an Input Class.
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Math/Math.h"
#include "../Utilities/Array.h"
#include "KeyCode.h"
#include "MouseButton.h"
#include "InputState.h"
#include "InputAxis.h"
#include "InputButton.h"

namespace Gem
{
	FORCE_EXPORT_META(Input);
	FORCE_EXPORT(Array<InputState>);
	FORCE_EXPORT(std::vector<InputAxis*>);
	FORCE_EXPORT(std::vector<InputButton*>);

	class GEM_API Input : public object
	{
		RDECLARE_CLASS(Input)
	public:
		~Input();
		/** 
		* Gets the instance of the Input class. This will not initialize it.
		* @return Returns an instance of the Input class.
		*/
		static Input * GetInstance();
		/**
		* Initialize the Input class setting up default states.
		*/
		static void Initialize();
		/**
		* Terminate the resources of the Input class.
		*/
		static void Terminate();

		/**
		* Gets the mouse position.
		* @return Returns the mouse position.
		*/
		inline static Vector2 GetMousePosition()
		{
			return s_Instance != nullptr ? s_Instance->m_MousePosition : Vector2::Zero();
		}

		/**
		* Gets the mouse scroll axis.
		* @return Returns the scroll axis of the mouse
		*/
		inline static Float32 GetMouseScrollAxis()
		{
			return s_Instance != nullptr ? s_Instance->m_MouseScrollAxis : 0.0f;
		}

		/**
		* Checks if the state of key.
		* @return Returns true if the state is down or pressed, false otherwise.
		*/
		static bool GetKey(KeyCode aKeyCode);
		/**
		* Checks if the state of the key is pressed.
		* @return Returns true if the key is pressed.
		*/
		static bool GetKeyDown(KeyCode aKeyCode);
		/** 
		* Checks if the state of the key is released.
		* @return Returns true if the key is released.
		*/
		static bool GetKeyUp(KeyCode aKeyCode);

		/**
		* Checks the state of the button
		* @return Returns true if the state is down or pressed, false otherwise.
		*/
		static bool GetMouseButton(MouseButton aButton);
		/**
		* Checks if the state of the button is pressed.
		* @return Returns true if the button is pressed.
		*/
		static bool GetMouseButtonDown(MouseButton aButton);
		/**
		* Checks if the state of the button is released
		* @return Returns true if the button is released.
		*/
		static bool GetMouseButtonUp(MouseButton aButton);
		

		/**
		* This method creates an input axis that is managed by the game's input system. This method specifically creates an axis that uses keyboard input.
		* @param aName The name of the axis to create.
		* @param aPostiiveKey The keycode of the positive axis.
		* @param aNegativeKey The keycode of the negative axis.
		* @param aSpeed The rate at which the input accelerates.
		* @param aResetOnRelease If this is true the axis value will snap back to 0 when there is no input. Otherwise it slowly moves to 0.
		*/
		static void CreateAxis(const std::string & aName, KeyCode aPositiveKey, KeyCode aNegativeKey, float aSpeed, bool aResetOnRelease);
		/**
		* This method creates an input axis that is managed by the game's input system. This method specifically creates an axis that uses mouse buttons as input.
		* @param aName The name of the axis to create.
		* @param aPositiveButton The mouse button code for the positive axis.
		* @param aNegativeButton The mouse button code for the negative axis.
		* @param aSpeed The rate at which the input accelerates.
		* @param aResetOnRelease  If this is true the axis value will snap back to 0 when there is no input. Otherwise it slowly moves to 0.
		*/
		static void CreateAxis(const std::string & aName, MouseButton aPositiveButton, MouseButton aNegativeButton, float aSpeed, bool aResetOnRelease);
		
		/**
		* This method creates an input axis that is managed by the game's input system. This method specifically creates an axis that uses mouse movement as input.
		* @param aName The name of the axis to create.
		*/
		static void CreateAxis(const std::string & aName, bool aMouseX);

		/**
		* This method creates a button using the specified keycode.
		* @param aName The name of button to create.
		* @param aKey The keycode to use.
		*/
		static void CreateButton(const std::string & aName, KeyCode aKey);
		/**
		* This method creates a button using a specified mousebutton.		
		* @param aName The name of the button to create.
		* @param aMouseButton The mouse button to use.
		*/
		static void CreateButton(const std::string & aName, MouseButton aMouseButton);


	
		/** 
		* This method iterates through all the axis and finds the axis with the matching names. It adds all the values of 
		* each axis together and returns the result. (Min = -1, Max = 1)
		* @param aName The name of the axis to search for
		* @return Returns a value of -1 to 1 based on input from the negative and positive axis InputDevices assigned with CreateAxis.
		*/
		static Float32 GetAxis(const std::string & aName);

		/**
		* This method iterates through all buttons and checks if the button has the matching name. It then keeps count of all
		* buttons that are considered down (Down/Pressed) and up (Up/Released). The result is equal to down > up.
		* @param aName the nname of the button to search for
		* @return returns true if the number of down buttons is greater than up.
		*/
		static bool GetButton(const std::string & aName);
		/** 
		* This method iterates through all buttons until it finds one with the matching name and is being pressed.
		* @param aName The name of the button to search for.
		* @return Returns true if any of the buttons are being pressed.
		*/
		static bool GetButtonDown(const std::string & aName);
		/** 
		* This method iterates through all buttons until it finds one with the matching name and is being released.
		* @param aName The name of the button to search for.
		* @return Returns true if any of the buttons are being released.ss
		*/
		static bool GetButtonUp(const std::string & aName);

	private:
		static Input * s_Instance;
		Input();
		
		/** The position of the mouse.*/
		Vector2 m_MousePosition;
		/** The value of the mouse scroll axis.*/
		Float32 m_MouseScrollAxis;
		/** The input states for keys. */
		Array<InputState> m_KeyStates;
		/** The input states for mouse buttons.*/
		Array<InputState> m_MouseStates;
		/** The input axis' to manage */
		std::vector<InputAxis*> m_InputAxis;
		/** The input buttons' to manage*/
		std::vector<InputButton*> m_InputButtons;
		//Internal methods.

		void ProcessKeyDown(KeyCode aKey);
		void ProcessKeyUp(KeyCode aKey);
		void ProcessMouseDown(MouseButton aButton);
		void ProcessMouseUp(MouseButton aButton);
		void ProcessMouseMove(Float32 x, Float32 y);
		void ProcessMouseScroll(Float32 aAxis);
		void Update();


		friend class Application;
		friend class Reflection::MetaObject<Input>;
	};

	TYPE_DEFINE(Input)
}

#endif // GEM_INPUT_H