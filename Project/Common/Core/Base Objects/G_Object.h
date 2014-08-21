#ifndef G_OBJECT_H
#define G_OBJECT_H

//#include "../../Reflection/OL_Reflection.h"
#include "../Reflection/G_Class.h"
#include "../Reflection/G_Type.h"

namespace Gem
{

    /*
    *   All objects have a type they must name
    */

    class MemoryManager;
    class PoolAllocator;

    //Remember
    //Virtual Method takes (Pure or not) takes 4 bytes for a pointer address
    
    class Object : public Reflection::Class
    {
    public:
        Object();
        virtual ~Object();


        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);

        //friend MemoryManager;
        //friend PoolAllocator;
    };


    /*
    COPY PASTE for implementing new types template

    Type WindowHook::getType()
    {
        return TypeFactory::create("ClassName",TypeID::ClassID,sizeof(Class));
    }
    Type WindowHook::baseType()
    {
        return Object::getType();
    }
    Type * WindowHook::instanceOf(int & aCount)
    {
        int prevCount = 0;
        Type * prevTypes = Object::instanceOf(prevCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,prevCount +1,prevTypes,prevCount);
        return types;
    }
    */
}

#endif