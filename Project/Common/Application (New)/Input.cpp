#include "Input.h"
#include "../Memory/Memory.h"


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
		m_MouseScrollAxis(0.0f),
        m_IsListeningForCharacter(false),
        m_CurrentCharacter(0)
	{
		m_KeyStates.Allocate(512);
		m_MouseStates.Allocate((SInt32)MouseButton::Middle + 1);

		for (UInt32 i = 0; i < m_KeyStates.GetCount(); i++)
		{
			m_KeyStates[i] = InputState::Up;
		}

		for (UInt32 i = 0; i < m_MouseStates.GetCount(); i++)
		{
			m_MouseStates[i] = InputState::Up;
		}
	}

	Input::~Input()
	{

		//Clear up memory for any allocated input axis / buttons.
		for (UInt32 i = 0; i < m_InputAxis.size(); i++)
		{
			MEM_POOL_DEALLOC_T(m_InputAxis[i], InputAxis);
		}

		for (UInt32 i = 0; i < m_InputButtons.size(); i++)
		{
			MEM_POOL_DEALLOC_T(m_InputButtons[i], InputButton);
		}

		m_InputAxis.clear();
		m_InputButtons.clear();
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

	void Input::CreateAxis(const std::string & aName, KeyCode aPositiveKey, KeyCode aNegativeKey, float aSpeed, bool aResetOnRelease)
	{
		if (s_Instance == nullptr)
		{
			return;
		}

		InputAxis * inputAxis = MEM_POOL_ALLOC_T(InputAxis, aName, aPositiveKey, aNegativeKey, aSpeed, aResetOnRelease);
		s_Instance->m_InputAxis.push_back(inputAxis);
	}
	void Input::CreateAxis(const std::string & aName, MouseButton aPositiveButton, MouseButton aNegativeButton, float aSpeed, bool aResetOnRelease)
	{
		if (s_Instance == nullptr)
		{
			return;
		}

		InputAxis * inputAxis = MEM_POOL_ALLOC_T(InputAxis, aName, aPositiveButton, aNegativeButton, aSpeed, aResetOnRelease);
		s_Instance->m_InputAxis.push_back(inputAxis);
	}
	void Input::CreateAxis(const std::string & aName, bool aMouseX)
	{
		if (s_Instance == nullptr)
		{
			return;
		}

		InputAxis * inputAxis = MEM_POOL_ALLOC_T(InputAxis, aName, aMouseX);
		s_Instance->m_InputAxis.push_back(inputAxis);
	}

	void Input::CreateButton(const std::string & aName, KeyCode aKey)
	{
		if (s_Instance == nullptr)
		{
			return;
		}
		InputButton * inputButton = MEM_POOL_ALLOC_T(InputButton, aName, aKey);
		s_Instance->m_InputButtons.push_back(inputButton);
	}

	void Input::CreateButton(const std::string & aName, MouseButton aMouseButton)
	{
		if (s_Instance == nullptr)
		{
			return;
		}
		InputButton * inputButton = MEM_POOL_ALLOC_T(InputButton, aName, aMouseButton);
		s_Instance->m_InputButtons.push_back(inputButton);
	}

	Float32 Input::GetAxis(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return 0.0f;
		}

		float totalValue = 0.0f;

		for (std::vector<InputAxis*>::iterator it = s_Instance->m_InputAxis.begin();
			it != s_Instance->m_InputAxis.end();
			it++)
		{
			InputAxis * axis = *it;
			if (axis->GetName() == aName)
			{
				if (axis->GetInputDeviceType() == InputDeviceType::MousePosition)
				{
					return axis->GetValue();
				}
				else
				{
					totalValue += axis->GetValue();
				}
			}
		}
		
		return Math::Clamp(totalValue, -1.0f, 1.0f);
	}

	bool Input::GetButton(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return false;
		}

		int buttonsDown = 0;
		int buttonsUp = 0;

		for (std::vector<InputButton*>::iterator it = s_Instance->m_InputButtons.begin();
			it != s_Instance->m_InputButtons.end();
			it++)
		{
			InputButton * button = *it;
			if (button->GetName() == aName)
			{

				InputState state = button->GetState();

				if (state == InputState::Down || state == InputState::Pressed)
				{
					buttonsDown++;
				}
				else
				{
					buttonsUp++;
				}
			}
		}
		return buttonsDown > buttonsUp;
	}

	bool Input::GetButtonDown(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return 0.0f;
		}

		for (std::vector<InputButton*>::iterator it = s_Instance->m_InputButtons.begin();
			it != s_Instance->m_InputButtons.end();
			it++)
		{
			InputButton * button = *it;
			if (button->GetName() == aName)
			{

				InputState state = button->GetState();
				if (state == InputState::Pressed)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool Input::GetButtonUp(const std::string & aName)
	{
		if (s_Instance == nullptr)
		{
			return 0.0f;
		}

		for (std::vector<InputButton*>::iterator it = s_Instance->m_InputButtons.begin();
			it != s_Instance->m_InputButtons.end();
			it++)
		{
			InputButton * button = *it;
			if (button->GetName() == aName)
			{
				InputState state = button->GetState();
				if (state == InputState::Released)
				{
					return true;
				}
			}
		}
		return false;
	}

    RDEFINE_PRIVATE_FUNCTION(Input, ProcessKeyDown, void,KeyCode)
	void Input::ProcessKeyDown(KeyCode aKey)
	{
        

		InputState currentState = m_KeyStates[(SInt32)aKey];
		if (currentState == InputState::Up || currentState == InputState::Released)
		{
			m_KeyStates[(SInt32)aKey] = InputState::Pressed;
		}
	}

    RDEFINE_PRIVATE_FUNCTION(Input, ProcessKeyUp, void, KeyCode)
	void Input::ProcessKeyUp(KeyCode aKey)
	{
		InputState currentState = m_KeyStates[(SInt32)aKey];
		if (currentState == InputState::Down || currentState == InputState::Pressed)
		{
			m_KeyStates[(SInt32)aKey] = InputState::Released;
		}
	}

    RDEFINE_PRIVATE_FUNCTION(Input, ProcessMouseDown, void, MouseButton)
	void Input::ProcessMouseDown(MouseButton aButton)
	{
		InputState currentState = m_MouseStates[(SInt32)aButton];
		if (currentState == InputState::Up || currentState == InputState::Released)
		{
			m_MouseStates[(SInt32)aButton] = InputState::Pressed;
		}
	}

    RDEFINE_PRIVATE_FUNCTION(Input, ProcessMouseUp, void, MouseButton)
	void Input::ProcessMouseUp(MouseButton aButton)
	{
		InputState currentState = m_MouseStates[(SInt32)aButton];
		if (currentState == InputState::Down || currentState == InputState::Pressed)
		{
			m_MouseStates[(SInt32)aButton] = InputState::Released;
		}
	}

    RDEFINE_PRIVATE_FUNCTION(Input, ProcessMouseMove, void, Float32, Float32)
	void Input::ProcessMouseMove(Float32 x, Float32 y)
	{
		m_MousePosition = Vector2(x, y);
	}

    RDEFINE_PRIVATE_FUNCTION(Input, ProcessMouseScroll, void, Float32)
	void Input::ProcessMouseScroll(Float32 aAxis)
	{
		m_MouseScrollAxis = aAxis;
	}
    RDEFINE_PRIVATE_FUNCTION(Input, ProcessChar, void, UInt32)
    void Input::ProcessChar(UInt32 aCharacter)
    {
        if (m_IsListeningForCharacter)
        {
            m_CurrentCharacter = aCharacter;
            m_IsListeningForCharacter = false;
        }
    }

    void Input::ListenForFirstKey()
    {
        m_CurrentCharacter = 0;
        m_IsListeningForCharacter = true;
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

		for (std::vector<InputAxis*>::iterator it = m_InputAxis.begin();
			it != m_InputAxis.end();
			it++)
		{
			(*it)->Update();
		}
	}


}