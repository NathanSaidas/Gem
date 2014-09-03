#ifndef G_INPUT_AXIS_H
#define G_INPUT_AXIS_H

#include "../Primitives/G_PrimitiveObjects.h"
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

        virtual Pointer<Reflection::Type> getType() override;
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
GEM_CLASS(InputAxis,Object)

#endif