#include "G_Object.h"
#include "../Reflection/G_Reflection.h"
#include <string>
namespace Gem
{

    using namespace Reflection;

    Object::Object()
    {

    }
    Object::~Object()
    {

    }
    Type * Object::getType()
    {
        return Type::create("Object",TypeID::OBJECT,sizeof(Object),Class::getType());
    }
    std::string Object::toString()
    {
        return nameOf(this);
    }
}