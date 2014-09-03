#ifndef G_OBJECT_H
#define G_OBJECT_H

//#include "../../Reflection/OL_Reflection.h"
//#include "../Memory/G_Pointer.h"
#include "../Reflection/G_Class.h"
#include "G_string.h"




namespace Gem
{

    class MemoryManager;

    namespace Reflection
    {
        class Type;
    }

    //Remember
    //Virtual Method takes (Pure or not) takes 4 bytes for a pointer address
    
    class Object : public Reflection::Class
    {
    public:
        Object();
        virtual ~Object();


        //virtual Reflection::Type * getType();
        //virtual Pointer<
        virtual Pointer<Reflection::Type> getType() override;
        virtual string toString();
        //friend MemoryManager;
        //friend PoolAllocator;
    protected:
        virtual void onInstantiate();
        virtual void onDestroy();
    private:
        //int m_ReferenceCount;
    };
}
GEM_CLASS(Object, Class)

#endif