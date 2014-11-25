#include "G_InputAxis.h"
#include "../Reflection/G_Reflection.h"
#include "../Memory/G_Memory.h"
#include "G_Input.h"
#include "../Utilities/G_Time.h"

namespace Gem
{
    using namespace Reflection;

    const float InputAxis::AXIS_INCREMENT_SPEED = 5.0f;

    InputAxis::InputAxis() : Object()
    {
        m_CurrentValue = 0.0f;
        m_ReSetOnRelease = false;
        m_Name = "";
        for(int i = 0; i < MAX_AXIS_KEYS; i++)
        {
            m_PositiveKeys[i] = AxisCode::NONE;
            m_NegativeKeys[i] = AxisCode::NONE;
        }
    }
    InputAxis::~InputAxis()
    {

    }

    void InputAxis::Update()
    {
        bool PositiveKeyDown = false;
        bool NegativeKeyDown = false;
        Input * input = Input::Instance();

        

        //Find out which keys are down
        for(int i = 0; i < MAX_AXIS_KEYS; i++)
        {
            if(m_PositiveKeys[i] < AxisCode::KEY_COUNT)
            {
                if(input->GetKey((KeyCode)m_PositiveKeys[i]))
                {
                    PositiveKeyDown = true;
                }
            }

            if(m_NegativeKeys[i] < AxisCode::KEY_COUNT)
            {
                if(input->GetKey((KeyCode)m_NegativeKeys[i]))
                {
                    NegativeKeyDown = true;
                }
            }
        }

        //If the two axis are equal and true to one another then do nothing
        if(PositiveKeyDown == true && NegativeKeyDown == true)
        {
            return;
        }

        if(PositiveKeyDown == true)
        {
            m_CurrentValue += AXIS_INCREMENT_SPEED * Time::DeltaTime();
            if(m_CurrentValue > 1.0f)
            {
                m_CurrentValue = 1.0f;
            }
        }
        else if(NegativeKeyDown == true)
        {
            m_CurrentValue -= AXIS_INCREMENT_SPEED * Time::DeltaTime();
            if(m_CurrentValue < -1.0f)
            {
                m_CurrentValue = -1.0f;
            }
        }
        else//They are both negative
        {
            if(m_ReSetOnRelease == true)
            {
                m_CurrentValue = 0.0f;
            }
            else
            {
                //if on the left move to the right
                if(m_CurrentValue < 0.0f)
                {
                    m_CurrentValue += AXIS_INCREMENT_SPEED * Time::DeltaTime();
                    if(m_CurrentValue > 0.0f)
                    {
                        m_CurrentValue = 0.0f;
                    }
                }
                //if on the right move to the left
                else if(m_CurrentValue > 0.0f)
                {
                    m_CurrentValue -= AXIS_INCREMENT_SPEED * Time::DeltaTime();
                    if(m_CurrentValue < 0.0f)
                    {
                        m_CurrentValue = 0.0f;
                    }
                }
            }
        }
        
    }

    void InputAxis::SetName(string aName)
    {
        m_Name = aName;
    }
    
    void InputAxis::SetResetOnRelease(bool aReSetFlag)
    {
        m_ReSetOnRelease = aReSetFlag;
    }
    void InputAxis::SetPositiveKey(AxisCode aCode, int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            m_PositiveKeys[aKey] = aCode;
        }
    }
    void InputAxis::SetNegativeKey(AxisCode aCode, int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            m_NegativeKeys[aKey] = aCode;
        }
    }

    string InputAxis::Name()
    {
        return m_Name;
    }
    bool InputAxis::ReSetOnRelease()
    {
        return m_ReSetOnRelease;
    }
    AxisCode InputAxis::PositiveKey(int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            return m_PositiveKeys[aKey];
        }
    }
    AxisCode InputAxis::NegativeKey(int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            return m_NegativeKeys[aKey];
        }
    }
    float InputAxis::AxisValue()
    {
        return m_CurrentValue;
    }

    Pointer<Reflection::Type> InputAxis::GetType()
    {
        return typeOf("InputAxis");
    }

}