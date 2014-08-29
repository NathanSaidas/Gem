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
    //Pointer<Reflection::Type> Object::getType2()
    //{
    //    ///Pointer<Type> type(false);
    //    ///type.ref() = getType();
    //    return Pointer<Type>();
    //}
    Type * Object::getType()
    {
        return Type::create("Object",TypeID::OBJECT,sizeof(Object),Class::getType());
    }
    std::string Object::toString()
    {
        return nameOf(this);
    }
}