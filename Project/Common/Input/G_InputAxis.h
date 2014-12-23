#ifndef G_INPUT_AXIS_H
#define G_INPUT_AXIS_H

#include "G_Key.h"
#include "../G_Object.h"

namespace Gem
{


    class InputAxis : public object
    {
		G_CLASS_DEF(InputAxis,object)
    public:
        InputAxis();
        ~InputAxis();

        void Update();

        void SetName(std::string & aName);
        void SetResetOnRelease(bool aReSetFlag);
        void SetPositiveKey(AxisCode aCode, int aKey);
        void SetNegativeKey(AxisCode aCode, int aKey);

        std::string Name();
        bool ReSetOnRelease();
        AxisCode PositiveKey(int aKey);
        AxisCode NegativeKey(int aKey);
        float AxisValue();
    private:
        static const int MAX_AXIS_KEYS = 2;
        static const float AXIS_INCREMENT_SPEED;
        
        std::string m_Name;
        bool m_ReSetOnRelease;
        float m_CurrentValue;
        
        
        AxisCode m_PositiveKeys[MAX_AXIS_KEYS];
        AxisCode m_NegativeKeys[MAX_AXIS_KEYS];
        

    };
}

#endif