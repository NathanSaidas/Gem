#ifndef G_INPUT_AXIS_H
#define G_INPUT_AXIS_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "G_Key.h"

namespace Gem
{


    class InputAxis : public Object
    {
    public:
        InputAxis();
        ~InputAxis();

        void Update();

        void SetName(string aName);
        void SetResetOnRelease(bool aReSetFlag);
        void SetPositiveKey(AxisCode aCode, int aKey);
        void SetNegativeKey(AxisCode aCode, int aKey);

        string Name();
        bool ReSetOnRelease();
        AxisCode PositiveKey(int aKey);
        AxisCode NegativeKey(int aKey);
        float AxisValue();

        virtual Pointer<Reflection::Type> GetType() override;
    private:
        static const int MAX_AXIS_KEYS = 2;
        static const float AXIS_INCREMENT_SPEED;
        
        string m_Name;
        bool m_ReSetOnRelease;
        float m_CurrentValue;
        
        
        AxisCode m_PositiveKeys[MAX_AXIS_KEYS];
        AxisCode m_NegativeKeys[MAX_AXIS_KEYS];
        

    };
}
GEM_CLASS(InputAxis,Object)

#endif