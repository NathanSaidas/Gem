#ifndef G_STREAM_FILE_H
#define G_STREAM_FILE_H

#include "../../Base Objects/G_Object.h"
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
        class StreamObject;
        class StreamDoc;
        class StreamFile : public Object
        {
        public:

            StreamFile();
            ~StreamFile();

            bool add(StreamObject * aObject);
            bool remove(StreamObject * aObject);
            bool remove(std::string aName);
            
            bool rename(std::string aName, std::string aNewName);
            bool rename(StreamObject * aObject, std::string aNewName);

            StreamObject * get(std::string aName);
            

            void serialize(pugi::xml_node & aNode);
            void deserialize(pugi::xml_node & aNode);

            StreamDoc * streamDoc();

            void clear();

            virtual Reflection::Type * getType();
        private:
            std::vector<StreamObject*> m_StreamObjects;
            StreamDoc * m_Document;

            friend class StreamDoc;

        };



    }
}

#endif