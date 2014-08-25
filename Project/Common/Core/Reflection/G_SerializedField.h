#ifndef G_SERIALIZED_FIELD_H
#define G_SERIALIZED_FIELD_H
#include "G_Field.h"
namespace pugi
{
    class xml_node;
}

namespace Gem
{
    namespace Reflection
    {
        class SerializedField : public Field
        {
        public:
            SerializedField(){}
            SerializedField(void * aAddress):Field(aAddress){}
            ~SerializedField(){}

            pugi::xml_node serializeField(pugi::xml_node & aNode);

        private:

        };
    }
}

#endif