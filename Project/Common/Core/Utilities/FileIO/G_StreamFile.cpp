#include "G_StreamFile.h"
#include "G_StreamObject.h"
#include "G_StreamDoc.h"
#include "../../Reflection/G_Reflection.h"
#include "../../Memory/G_Memory.h"
#include "../G_Utilities.h"
#include <pugixml.hpp>

using namespace pugi;

namespace Gem
{
    using namespace Reflection;
    namespace FileIO
    {
        StreamFile::StreamFile()
        {
            m_Document = nullptr;
        }
        StreamFile::~StreamFile()
        {
            clear();
        }

        bool StreamFile::add(StreamObject * aObject)
        {
            if(aObject == nullptr)
            {
                return false;
            }
            bool exists = false;

            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] != nullptr && m_StreamObjects[i]->name() == aObject->name())
                {
                    exists = true;
                }
            }
            if(exists == false)
            {
                m_StreamObjects.push_back(aObject);
            }
            return !exists;
        }
        bool StreamFile::remove(StreamObject * aObject)
        {
            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] == aObject)
                {
                    m_StreamObjects[i] = Memory::destroy<StreamObject>(m_StreamObjects[i]);
                    m_StreamObjects.erase(m_StreamObjects.begin() + i);
                    return true;
                }
            }
            return false;
        }
        bool StreamFile::remove(std::string aName)
        {
            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] != nullptr && m_StreamObjects[i]->name() == aName)
                {
                    m_StreamObjects[i] = Memory::destroy<StreamObject>(m_StreamObjects[i]);
                    m_StreamObjects.erase(m_StreamObjects.begin() + i);
                    return true;
                }
            }
            return false;
        }

        bool StreamFile::rename(std::string aName, std::string aNewName)
        {
            StreamObject * aObject = get(aName);
            if(aObject == nullptr)
            {
                return false;
            }

            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] != nullptr && m_StreamObjects[i]->name() == aNewName)
                {
                    return false;
                }
            }

            aObject->m_Name = aNewName;
            return true;
        }

        bool StreamFile::rename(StreamObject * aObject, std::string aNewName)
        {
            if(aObject == nullptr)
            {
                return false;
            }
            StreamObject * object = nullptr;
            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] == aObject)
                {
                    object = m_StreamObjects[i];
                    break;
                }
            }

            if(object == nullptr)
            {
                return false;
            }

            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] != nullptr && m_StreamObjects[i]->name() == aNewName)
                {
                    return false;
                }
            }

            object->m_Name = aNewName;
            return true;

        }
           
        StreamObject * StreamFile::get(std::string aName)
        {
            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                if(m_StreamObjects[i] != nullptr && m_StreamObjects[i]->name() == aName)
                {
                    return m_StreamObjects[i];
                }
            }

            return nullptr;
        }

        void StreamFile::serialize(pugi::xml_node & aNode)
        {
            int size = m_StreamObjects.size();

            std::string nodeName = "";

            for(int i = 0; i < size; i ++)
            {
                StreamObject * object = m_StreamObjects[i];
                if(object == nullptr)
                {
                    continue;
                }
                nodeName = "StreamObject_";
                nodeName.append(I2S(i));

                xml_node node = aNode.append_child(nodeName.c_str());
                aNode.append_attribute("_Name_") = object->name().c_str();
                object->serialize(node);
            }
        }
        void StreamFile::deserialize(pugi::xml_node & aNode)
        {
            string nodeName = "";
     
            //parse nodes from root
            for(xml_node_iterator iter = aNode.begin(); iter != aNode.end(); ++iter)
            {
                xml_node node(iter->internal_object());
                xml_attribute objname = node.attribute("_Name_");

                if(string(objname.as_string()) == string::Empty)
                {
                    continue;
                }

                //Create SteamObject
                StreamObject * object = Memory::instantiate<StreamObject>();
                //give it the node to deserailize
                if(object != nullptr)
                {
                    object->m_Name = objname.as_string();
                    object->deserialize(node);
                }
                add(object);
            }
        }

        StreamDoc * StreamFile::streamDoc()
        {
            return m_Document;
        }
        void StreamFile::clear()
        {
            for(int i = 0; i < m_StreamObjects.size(); i++)
            {
                m_StreamObjects[i] = Memory::destroy<StreamObject>(m_StreamObjects[i]);
            }
            m_StreamObjects.clear();
        }
        Pointer<Reflection::Type> StreamFile::getType()
        {
            return typeOf("StreamFile");
        }
    }
}