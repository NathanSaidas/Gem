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
        m_ResetOnRelease = false;
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

    void InputAxis::update()
    {
        bool positiveKeyDown = false;
        bool negativeKeyDown = false;
        Input * input = Input::instance();

        

        //Find out which keys are down
        for(int i = 0; i < MAX_AXIS_KEYS; i++)
        {
            if(m_PositiveKeys[i] < AxisCode::KEY_COUNT)
            {
                if(input->getKey((KeyCode)m_PositiveKeys[i]))
                {
                    positiveKeyDown = true;
                }
            }

            if(m_NegativeKeys[i] < AxisCode::KEY_COUNT)
            {
                if(input->getKey((KeyCode)m_NegativeKeys[i]))
                {
                    negativeKeyDown = true;
                }
            }
        }

        //If the two axis are equal and true to one another then do nothing
        if(positiveKeyDown == true && negativeKeyDown == true)
        {
            return;
        }

        if(positiveKeyDown == true)
        {
            m_CurrentValue += AXIS_INCREMENT_SPEED * Time::deltaTime();
            if(m_CurrentValue > 1.0f)
            {
                m_CurrentValue = 1.0f;
            }
        }
        else if(negativeKeyDown == true)
        {
            m_CurrentValue -= AXIS_INCREMENT_SPEED * Time::deltaTime();
            if(m_CurrentValue < -1.0f)
            {
                m_CurrentValue = -1.0f;
            }
        }
        else//They are both negative
        {
            if(m_ResetOnRelease == true)
            {
                m_CurrentValue = 0.0f;
            }
            else
            {
                //if on the left move to the right
                if(m_CurrentValue < 0.0f)
                {
                    m_CurrentValue += AXIS_INCREMENT_SPEED * Time::deltaTime();
                    if(m_CurrentValue > 0.0f)
                    {
                        m_CurrentValue = 0.0f;
                    }
                }
                //if on the right move to the left
                else if(m_CurrentValue > 0.0f)
                {
                    m_CurrentValue -= AXIS_INCREMENT_SPEED * Time::deltaTime();
                    if(m_CurrentValue < 0.0f)
                    {
                        m_CurrentValue = 0.0f;
                    }
                }
            }
        }
        
    }

    void InputAxis::setName(std::string aName)
    {
        m_Name = aName;
    }
    
    void InputAxis::setResetOnRelease(bool aResetFlag)
    {
        m_ResetOnRelease = aResetFlag;
    }
    void InputAxis::setPositiveKey(AxisCode aCode, int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            m_PositiveKeys[aKey] = aCode;
        }
    }
    void InputAxis::setNegativeKey(AxisCode aCode, int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            m_NegativeKeys[aKey] = aCode;
        }
    }

    std::string InputAxis::name()
    {
        return m_Name;
    }
    bool InputAxis::resetOnRelease()
    {
        return m_ResetOnRelease;
    }
    AxisCode InputAxis::positiveKey(int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            return m_PositiveKeys[aKey];
        }
    }
    AxisCode InputAxis::negativeKey(int aKey)
    {
        if(aKey >= 0 && aKey < MAX_AXIS_KEYS)
        {
            return m_NegativeKeys[aKey];
        }
    }
    float InputAxis::axisValue()
    {
        return m_CurrentValue;
    }

    Type InputAxis::getType()
    {
        return TypeFactory::create("InputAxis",TypeID::WINDOW_HOOK,sizeof(InputAxis));
    }
    Type InputAxis::baseType()
    {
        return Object::getType();
    }
    Type * InputAxis::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
    }
}