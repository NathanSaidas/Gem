#ifndef G_TYPE_H
#define G_TYPE_H

#include "G_Primitive.h"
#include "../Primitives/G_string.h"
#include "../Primitives/G_integer.h"

//Base class of every data type. Enum / Class
namespace Gem
{
    class string;
    class Allocator;

    namespace Reflection
    {
        class TypeInfo;
        typedef Primitive * (*CreatePrimitiveFunc)(Primitive*);
        typedef Primitive * (*DestroyPrimitiveFunc)(Primitive*);

        class Type sealed: public Primitive
        {
        public:
            Type();
            ~Type();

            inline bool operator ==(Type & aRhs)
            {
                return aRhs.m_TypeID == m_TypeID;
            }
            //Create methods
            //static Type * create(char * aName, int aTypeID, int aSize, Type * aBaseType);
            //static void freeType(Type * aType);

            //Operators
            //static char * nameOf(Type * aType);
            //static int typeID(Type * aType);
            //static int sizeOf(Type * aType);
            //static bool instanceOf(Type * aBase, Type * aDerived);



            //Type * getType();

            //string & name() const;
            //char * name() const;
            string  name() const;
            integer typeID() const;
            integer size() const;
            integer alignment() const;
            //string  baseClass() const;
            //char * baseClass() const;
            Type * baseClass() const;
            

            Pointer<Type> getType() override;
        protected:
            static Type * copy(Type * aType);

            //char * m_Name;
            string m_Name;
            integer m_TypeID;
            integer m_Size;
            integer m_Alignment;
            //string m_BaseClass;
            //char * m_BaseClass;
            Type * m_BaseType;

            CreatePrimitiveFunc m_CreateFunc;
            DestroyPrimitiveFunc m_DestroyFunc;

            Primitive * invokeCreate(Primitive* aAddress);
            Primitive * invokeDestroy(Primitive* aAddress);
           
            friend Gem::Allocator;
            friend class Hidden;
            friend class Runtime;
        };
    }
}
GEM_CLASS(Type,Primitive)

#endif