#include "G_SerializedField.h"
#include <pugixml.hpp>
namespace Gem
{
    namespace Reflection
    {
        pugi::xml_node SerializedField::serializeField(pugi::xml_node & aNode)
        {
            return aNode;
        }
    }
}