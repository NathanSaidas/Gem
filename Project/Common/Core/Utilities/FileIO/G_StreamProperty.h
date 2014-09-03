#ifndef G_STREAM_PROPERTY_H
#define G_STREAM_PROPERTY_H

#include "../../Primitives/G_PrimitiveObjects.h"
#include <string>
namespace Gem
{
    namespace FileIO
    {
        class StreamProperty : public Object
        {
            public:
                StreamProperty();
                ~StreamProperty();

                std::string name();
                std::string asString();
                int asInt();
                short asShort();
                float asFloat();
                double asDouble();

                void setName(std::string aName);
                void setValue(std::string aValue);
                void setValue(int aValue);
                void setValue(short aValue);
                void setValue(float aValue);
                void setValue(double aValue);

                virtual Pointer<Reflection::Type> getType() override;
            private:

                std::string m_Name;
                std::string m_Value;
        };

    }
}
GEM_CLASS(FileIO::StreamProperty, Object)
#endif