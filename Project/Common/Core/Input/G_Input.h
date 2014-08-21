#ifndef OL_INPUT_H
#define OL_INPUT_H

#include "../Base Objects/G_Object.h"
#include "G_KeyDef.h"
#include "G_Key.h"
#include "../Math/G_Math.h"
#include <vector>
#include <string>

namespace Gem
{
    class InputAxis;
    class Input : public Object
    {
    public:

        static Input * instance();
        static void destroy();

        //int getKeyState(KeyCode aKeyCode);

        //returns true if key is down (press)
        inline
        bool getKeyDown(KeyCode aKeyCode)
        {
            if(m_Keys[(int)aKeyCode] != nullptr)
            {
                return m_Keys[(int)aKeyCode]->currentState() == ButtonState::PRESS;
            }
            return false;
        }
        //returns true if the key is up (released)
        inline
        bool getKeyUp(KeyCode aKeyCode)
        {
            if(m_Keys[(int)aKeyCode] != nullptr)
            {
                return m_Keys[(int)aKeyCode]->currentState() == ButtonState::RELEASE;
            }
            return false;
        }

        //returns true if the key is down or pressed
        //returns false if the key is up or released
        inline
        bool getKey(KeyCode aKeyCode)
        {
            if(m_Keys[(int)aKeyCode] != nullptr)
            {
                return m_Keys[(int)aKeyCode]->isDown() == true;
            }
            return false;
        }
        inline bool getMouseButtonDown(MouseButton aButton)
        {
            if(m_MouseButtons[(int)aButton] != nullptr)
            {
                return m_MouseButtons[(int)aButton]->currentState() == ButtonState::PRESS;
            }
            return false;
        }
        inline bool getMouseButtonUp(MouseButton aButton)
        {
            if(m_MouseButtons[(int)aButton] != nullptr)
            {
                return m_MouseButtons[(int)aButton]->currentState() == ButtonState::RELEASE;
            }
            return false;
        }
        inline bool getMouseButton(MouseButton aButton)
        {
            if(m_MouseButtons[(int)aButton] != nullptr)
            {
                return m_MouseButtons[(int)aButton]->isDown() == true;
            }
            return false;
        }
        inline Vector2 scrollDirection()
        {
            return m_MouseScrollWheel;
        }
        inline Vector2 mousePoisition()
        {
            return m_MousePosition;
        }
        inline Vector2 lastMousePosition()
        {
            return m_LastMousePosition;
        }
        inline Vector2 deltaMousePosition()
        {
            return Math::abs(m_MousePosition - m_LastMousePosition);
        }

        ///Input Axis Functions
        void destroyAxis(std::string aName);
        void createAxis(std::string aName);
        void createAxis(std::string aName, AxisCode aPositiveKey, AxisCode aNegativeKey);
        void setAxisPositiveKey(std::string aName, AxisCode aKeyCode, int aKey);
        void setAxisNegativeKey(std::string aName, AxisCode aKeyCode, int aKey);
        void setAxisResetOnRelease(std::string, bool aResetflag);

        float getAxis(std::string aName);
        AxisCode getAxisPositiveKey(std::string aName , int aKey);
        AxisCode getAxisNegativeKey(std::string aName , int aKey);
        bool getAxisResetOnRelease(std::string aName );

        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);
    private:
        Key * m_Keys[(int)KeyCode::COUNT];
        Button * m_MouseButtons[(int)MouseButton::COUNT];

        Vector2 m_MouseScrollWheel;

        Vector2 m_MousePosition;
        Vector2 m_LastMousePosition;
        int m_CurrentWindowID;


        std::vector<InputAxis*> m_Axis;

        static Input * s_Instance;
        Input();
        ~Input();


        void update();

        void processKeyEvent(int aKey, int aAction, int aMods);
        void processMouseEvent(int aButton, int aAction, int aMods);
        void processMouseMove(float aX, float aY);
        void processMouseScroll(float aX, float aY);

        friend class PoolAllocator;
        friend class WindowManager;
        friend class Application;
    };

}

#endif