#include "G_Object.h"
#include "../Reflection/G_Reflection.h"
#include "../Reflection/G_ReflectionOperators.h"
#include "G_string.h"


namespace Gem
{

    using namespace Reflection;

    Object::Object()
    {

    }
    Object::~Object()
    {

    }
    //Pointer<Reflection::Type> Object::GetType2()
    //{
    //    ///Pointer<Type> type(false);
    //    ///type.ref() = GetType();
    //    return Pointer<Type>();
    //}
    Pointer<Reflection::Type> Object::GetType()
    {
        return typeOf("Object");
    }
    string Object::toString()
    {
        return "";
    }
    void Object::onInstantiate()
    {

    }
    void Object::onDestroy()
    {

    }
}