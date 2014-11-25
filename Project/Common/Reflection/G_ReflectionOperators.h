#ifndef G_REFLECTION_OPERATORS_H
#define G_REFLECTION_OPERATORS_H

#include "../Memory/G_Pointer.h"

namespace Gem
{
    class string;
    class integer;
    class Allocator;

    namespace Reflection
    {
        class Type;
        class Primitive;
    }

    Pointer<Reflection::Type> typeOf(const char * aTypename);
    Pointer<Reflection::Type> typeOf(string & aTypename);
    Pointer<Reflection::Type> typeOf(integer & aID);
    template<class T>
    Pointer<Reflection::Type> typeOf()
    {
        T t;
        return t.GetType();
    }

    //Determines if the 'aBaseType' is derived from 'aDerivedType'
    bool InstanceOf(Pointer<Reflection::Type> & aBaseType, Pointer<Reflection::Type> & aDerived);
    bool InstanceOf(Reflection::Primitive * aBaseType, Reflection::Primitive * aDerivedType);

    namespace Reflection
    {
        

        //Type * typeOf(string & aTypename);
        //Type * typeOf(integer &aId);
        

        class Hidden
        {
        public:
            
        private:
            //Simply invokes the constructors and destructors of the class by name
            static Primitive * createPrimitive(Primitive * aAddress, string & aTypename);
            static Primitive * createPrimitive(Primitive * aAddress, integer & aID);
            static Primitive * DestroyPrimitive(Primitive * aAddress, string & aTypename);
            static Primitive * DestroyPrimitive(Primitive * aAddress, integer & aID);

            friend Gem::Allocator;
        };
    }
}

#endif