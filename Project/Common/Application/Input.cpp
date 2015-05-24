#include "Input.h"

namespace Gem
{
	RDEFINE_CLASS(Input, object)

	Input * Input::s_Instance = nullptr;
	Input * Input::GetInstance()
	{
		return s_Instance;
	}
	void Input::Initialize()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Input();
		}
	}
	void Input::Terminate()
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	Input::Input() : object(),
		m_MousePosition(Vector2::Zero()),
		m_MouseScrollAxis(0.0f)
	{
		m_KeyStates.Allocate(512);
		m_MouseStates.Allocate((SInt32)MouseButton::Middle + 1);
	}

	Input::~Input()
	{

	}

	bool Input::GetKey(KeyCode aKeyCode)
	{
		return s_Instance != nullptr ?
			s_Instance->m_KeyStates[(SInt32)aKeyCode] == InputState::Down || s_Instance->m_KeyStates[(SInt32)aKeyCode] == InputState::Pressed :
			false;
	}
	bool Input::GetKeyDown(KeyCode aKeyCode)
	{
		return s_Instance != nullptr ? 
			s_Instance->m_KeyStates[(SInt32)aKeyCode] == InputState::Pressed :
			false;
	}
	bool Input::GetKeyUp(KeyCode aKeyCode)
	{
		return s_Instance != nullptr ?
			s_Instance->m_KeyStates[(SInt32)aKeyCode] == InputState::Released :
			false;
	}

	bool Input::GetMouseButton(MouseButton aButton)
	{
		return s_Instance != nullptr ?
			s_Instance->m_MouseStates[(SInt32)aButton] == InputState::Down || s_Instance->m_MouseStates[(SInt32)aButton] == InputState::Pressed :
			false;
	}
	bool Input::GetMouseButtonDown(MouseButton aButton)
	{
		return s_Instance != nullptr ?
			s_Instance->m_MouseStates[(SInt32)aButton] == InputState::Pressed :
			false;
	}
	bool Input::GetMouseButtonUp(MouseButton aButton)
	{
		return s_Instance != nullptr ?
			s_Instance->m_MouseStates[(SInt32)aButton] == InputState::Released :
			false;
	}


	void Input::ProcessKeyDown(KeyCode aKey)
	{
		InputState currentState = m_KeyStates[(SInt32)aKey];
		if (currentState == InputState::Up || currentState == InputState::Released)
		{
			m_KeyStates[(SInt32)aKey] = InputState::Pressed;
		}
	}
	void Input::ProcessKeyUp(KeyCode aKey)
	{
		InputState currentState = m_KeyStates[(SInt32)aKey];
		if (currentState == InputState::Down || currentState == InputState::Pressed)
		{
			m_KeyStates[(SInt32)aKey] = InputState::Released;
		}
	}
	void Input::ProcessMouseDown(MouseButton aButton)
	{
		InputState currentState = m_MouseStates[(SInt32)aButton];
		if (currentState == InputState::Up || currentState == InputState::Released)
		{
			m_MouseStates[(SInt32)aButton] = InputState::Pressed;
		}
	}
	void Input::ProcessMouseUp(MouseButton aButton)
	{
		InputState currentState = m_MouseStates[(SInt32)aButton];
		if (currentState == InputState::Down || currentState == InputState::Pressed)
		{
			m_MouseStates[(SInt32)aButton] = InputState::Released;
		}
	}

	void Input::ProcessMouseMove(Float32 x, Float32 y)
	{
		m_MousePosition = Vector2(x, y);
	}

	void Input::ProcessMouseScroll(Float32 aAxis)
	{
		m_MouseScrollAxis = aAxis;
	}

	void Input::Update()
	{
		for (int i = 0; i < m_KeyStates.GetCount(); i++)
		{
			if (m_KeyStates[i] == InputState::Pressed)
			{
				m_KeyStates[i] = InputState::Down;
			}
			else if (m_KeyStates[i] == InputState::Released)
			{
				m_KeyStates[i] = InputState::Up;
			}
		}

		for (int i = 0; i < m_MouseStates.GetCount(); i++)
		{
			if (m_MouseStates[i] == InputState::Pressed)
			{
				m_MouseStates[i] = InputState::Down;
			}
			else if (m_MouseStates[i] == InputState::Released)
			{
				m_MouseStates[i] = InputState::Up;
			}
		}
	}


}