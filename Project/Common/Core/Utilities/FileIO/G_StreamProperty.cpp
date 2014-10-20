#include "G_StreamProperty.h"
#include "../G_Utilities.h"
#include "../../Reflection/G_Reflection.h"

namespace Gem
{
    using namespace Reflection;

    namespace FileIO
    {
        StreamProperty::StreamProperty() : Object()
        {

        }
        StreamProperty::~StreamProperty()
        {
        }

        std::string StreamProperty::name()
        {
            return m_Name;
        }
        std::string StreamProperty::asString()
        {
            return m_Value;
        }
        int StreamProperty::asInt()
        {
            return S2I(m_Value);
        }
        short StreamProperty::asShort()
        {
            return convertStringToShort(m_Value);
        }
        float StreamProperty::asFloat()
        {
            return S2F(m_Value);
        }
        double StreamProperty::asDouble()
        {
            return convertStringToDouble(m_Value);
        }

        void StreamProperty::setName(std::string aName)
        {
            m_Name = aName;
        }
        void StreamProperty::setValue(std::string aValue)
        {
            m_Value = aValue;
        }
        void StreamProperty::setValue(int aValue)        
        {
			m_Value = I2S(aValue).c_str();
        }
        void StreamProperty::setValue(short aValue)      
        {
            m_Value = convertShortToString(aValue);
        }
        void StreamProperty::setValue(float aValue)      
        {
            m_Value = F2S(aValue).c_str();
        }
        void StreamProperty::setValue(double aValue)     
        {
            m_Value = convertDoubleToString(aValue);
        }

        Pointer<Reflection::Type> StreamProperty::getType()
        {
            return typeOf("StreamProperty");
        }
    }
}