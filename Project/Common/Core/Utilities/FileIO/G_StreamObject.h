#ifndef G_STREAM_OBJECT_H
#define G_STREAM_OBJECT_H

#include "../../Primitives/G_PrimitiveObjects.h"

#include <string>
#include <vector>


namespace pugi
{
    class xml_node;
}

namespace Gem
{
    namespace FileIO
    {
        class StreamProperty;

        class StreamObject : public Object
        {
        public:
            StreamObject();
            StreamObject(std::vector<Object> & aArgs);
            ~StreamObject();

            bool addProperty(StreamProperty * aProperty);
            bool removeProperty(StreamProperty * aProperty);
            bool removeProperty(std::string aName);

            StreamProperty * getProperty(std::string aName);

            void clearProperties();

            std::string name();

            void serialize(pugi::xml_node & aNode);
            void deserialize(pugi::xml_node & aNode);

            virtual Pointer<Reflection::Type> getType() override;
        private:
            std::string m_Name;
            std::vector<StreamProperty*> m_Properties;

            friend class StreamFile;
        };
    }
}
GEM_CLASS(FileIO::StreamObject,Object)
#endif