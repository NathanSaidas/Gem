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

namespace Gem
{
	FORCE_EXPORT_META(Input);
	FORCE_EXPORT(Array<InputState>);

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