#include "G_StreamObject.h"
#include "G_StreamProperty.h"
#include "../../Reflection/G_Reflection.h"
#include "../../Memory/G_Memory.h"
#include "../../Primitives/G_PrimitiveObjects.h"

#include <pugixml.hpp>

namespace Gem
{
    using namespace Reflection;

    namespace FileIO
    {
        StreamObject::StreamObject()
        {

        }
        StreamObject::StreamObject(std::vector<Object> & aArgs) : Object()
        {
            if(aArgs.size() > 0)
            {
                //if(typeOf<
            }
        }
        StreamObject::~StreamObject()
        {
            clearProperties();
        }

        bool StreamObject::addProperty(StreamProperty * aProperty)
        {
            if(aProperty == nullptr)
            {
                return false;
            }
            bool exists = false;

            for(int i = 0; i < m_Properties.size(); i++)
            {
                if(m_Properties[i] != nullptr && m_Properties[i]->name() == aProperty->name())
                {
                    exists = true;
                }
            }

            if(exists == false)
            {
                m_Properties.push_back(aProperty);
            }

            return !exists;
        }
        bool StreamObject::removeProperty(StreamProperty * aProperty)
        {
            for(int i = 0; i < m_Properties.size(); i++)
            {
                if(m_Properties[i] == aProperty)
                {
                    m_Properties[i] = Memory::destroy<StreamProperty>(m_Properties[i]);
                    m_Properties.erase(m_Properties.begin() + i);
                    return true;
                }
            }
        }
        bool StreamObject::removeProperty(std::string aName)
        {
            for(int i = 0; i < m_Properties.size(); i++)
            {
                if(m_Properties[i] != nullptr && m_Properties[i]->name() == aName)
                {
                    m_Properties[i] = Memory::destroy<StreamProperty>(m_Properties[i]);
                    m_Properties.erase(m_Properties.begin() + i);
                    return true;
                }
            }
            return false;
        }

        StreamProperty * StreamObject::getProperty(std::string aName)
        {
            for(int i = 0; i < m_Properties.size(); i++)
            {
                if(m_Properties[i] != nullptr && m_Properties[i]->name() == aName)
                {
                    return m_Properties[i];
                }
            }
            return nullptr;
        }

        void StreamObject::clearProperties()
        {
            for(int i = 0; i < m_Properties.size(); i++)
            {
                m_Properties[i] = Memory::destroy<StreamProperty>(m_Properties[i]);
            }
            m_Properties.clear();
        }

        std::string StreamObject::name()
        {
            return m_Name;
        }
        void StreamObject::serialize(pugi::xml_node & aNode)
        {
            for(int i = 0; i < m_Properties.size(); i++)
            {
                StreamProperty * property = m_Properties[i];
                if(property == nullptr)
                {
                    continue;
                }

                aNode.append_attribute(property->name().c_str()) = property->asString().c_str();
            }
        }
        void StreamObject::deserialize(pugi::xml_node & aNode)
        {
            std::string localname;
            std::string localvalue;

            //parse attributes from node
            for(pugi::xml_attribute_iterator iter = aNode.attributes_begin(); iter != aNode.attributes_end(); ++iter)
            {
                localname = iter->name();
                localvalue = iter->as_string();

                if(localname == "_Name_")
                {
                    continue;
                }

                if(localname != "" && localvalue != "")
                {
                    //Create a property node
                    StreamProperty * property = Memory::instantiate<StreamProperty>();
                    property->setName(localname);
                    property->setValue(localvalue);
                    addProperty(property);
                }
            }
        }

        Pointer<Reflection::Type> StreamObject::getType()
        {
            return typeOf("StreamObject");
        }
    }
}