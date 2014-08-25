#ifndef G_I_XML_SERIALIZABLE_H
#define G_I_XML_SERIALIZABLE_H

namespace pugi
{
    class xml_node;
    class xml_node_iterator;
}

namespace Gem
{
    class IXmlSerializable
    {
    public:

        virtual pugi::xml_node serialize(pugi::xml_node & aNode, bool aIncludeTypeInfo = false) = 0;
        virtual bool deserialize(pugi::xml_node & aNode,bool aIncludeTypeInfo = false) = 0;
    private:

    };
}

#endif