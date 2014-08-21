#ifndef G_INPUT_AXIS_H
#define G_INPUT_AXIS_H

#include "../Base Objects/G_Object.h"
#include "G_Key.h"
#include <string>

namespace Gem
{


    class InputAxis : public Object
    {
    public:
        InputAxis();
        ~InputAxis();

        void update();

        void setName(std::string aName);
        void setResetOnRelease(bool aResetFlag);
        void setPositiveKey(AxisCode aCode, int aKey);
        void setNegativeKey(AxisCode aCode, int aKey);

        std::string name();
        bool resetOnRelease();
        AxisCode positiveKey(int aKey);
        AxisCode negativeKey(int aKey);
        float axisValue();

        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);
    private:
        static const int MAX_AXIS_KEYS = 2;
        static const float AXIS_INCREMENT_SPEED;
        
        std::string m_Name;
        bool m_ResetOnRelease;
        float m_CurrentValue;
        
        
        AxisCode m_PositiveKeys[MAX_AXIS_KEYS];
        AxisCode m_NegativeKeys[MAX_AXIS_KEYS];
        

    };
}

#endif