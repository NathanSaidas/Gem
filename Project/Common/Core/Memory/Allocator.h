#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdint>
#include <assert.h>
#include <malloc.h>
#include <new.h>
#include "../Base Objects/G_Object.h"

namespace Gem
{
    //From http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010
    
    #define ASSERT assert

    typedef unsigned int uint;

    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef uintptr_t uptr; 


    //Returns a pointer to the next alligned memory address
    inline void * nextAlignedAddress(void* aAddress, u8 aAlignment)
    {
        return (void*)( ( (uptr)aAddress + (aAlignment-1) ) & ~(aAlignment -1) );
    }

    inline u8 alignAdjustment(void * aAddress, u8 aAlignment)
    {
        u8 adjustment = aAlignment - ( (uptr)aAddress & (aAlignment -1) );
        
        if(adjustment == aAlignment)
        {
            return 0;
        }
        return adjustment;
    }

    inline u8 alignAdjustmentWithHeader(void* aAddress, u8 aAlignment, u8 aHeaderSize)
    {
        u8 adjustment = aAlignment - ( (uptr)aAddress & ( aAlignment-1) );

        if(adjustment == aAlignment)
        {
            adjustment = 0;
        }

        u8 neededSpace = aHeaderSize;

        if(adjustment < neededSpace)
        {
            neededSpace -= adjustment;

            adjustment += aAlignment * (neededSpace / aAlignment);

            if(neededSpace % aAlignment > 0)
            {
                adjustment += aAlignment;
            }
        }

        return adjustment;
    }


    /*
    *   Class: Allocator
    *   Base Class: Object
    *   Interfaces: N/A 
    *   Description: Base Allocator for all coming Allocators
    *   Date Modified: 27/06/2014 by Nathan Hanlan
    */
    class Allocator : public Object
    {
    public:
        Allocator()
        {
            m_UsedMemory = 0;
            m_NumberOfAllocations = 0;
        }

        virtual ~Allocator()
        {
            ASSERT(m_NumberOfAllocations == 0 && m_UsedMemory == 0);
        }

        virtual void * allocate(u32 aSize, u8 aAlignment) = 0;
        virtual void deallocate(void * p) = 0;

        template<class T> T * allocateNew()
        {
            return new(allocate(sizeof(T),__alignof(T)))T;
        }
        template<class T> T * allocateNew(const T & t)
        {
            return new (allocate(sizeof(T),__alignof(T)))T(t);
        }
        template<class T> void deallocateDelete(T * aObject)
        {
            if(aObject != nullptr)
            {
                aObject->~T();
                deallocate(aObject);
            }
        }
        template<class T> T* allocateArray(u32 aLength)
        {
            if(aLength == 0)
            {
                return nullptr;
            }

            //Calculate the size of the header
            u32 headerSize = sizeof(u32)/sizeof(T);
            if(sizeof(u32)%sizeof(T) > 0)
            {
                headerSize += 1;
            }

            //Allocate the array
            T * objectArray = ( ( T*) allocate(sizeof(T) *(aLength + headerSize),__alignof(T)) ) + headerSize;

            //Assign ptr
            *(((u32*)objectArray)-1) = aLength;

            //Allocate each object in the array
            for(u32 i = 0; i < aLength; i++)
            {
                new(&objectArray[i])T;
            }

            return p;
        }
        template<class T> void deallocateArray(T * aArray)
        {
            if(aArray == nullptr)
            {
                return;
            }

            u32 length = *(((u32*)pArray) - 1);

            for(u32 i = 0; i < length; i++)
            {
                aArray[i].~T();
            }

            u32 headerSize = sizeof(u32)/sizeof(T);

            if(sizeof(u32)%sizeof(T) > 0)
            {
                headerSize += 1;
            }

            deallocate(aArray - headerSize);
        }
        

        u32 usedMemory()
        {
            return m_UsedMemory;
        }

        u32 numberOfAllocations()
        {
            return m_NumberOfAllocations;
        }

        /*
        *   Function: getType
        *   Return Type: Type
        *   Description: Returns the type for Allocator
        *   Parameters: none
        *   Date Modified: 27/06/2014 by Nathan Hanlan
        */
        virtual Reflection::Type getType();
        /*
        *   Function: baseType()
        *   Return Type: Type
        *   Description: Returns the base type for Allocator
        *   Parameters: none
        *   Date Modified: 27/06/2014 by Nathan Hanlan
        */
        virtual Reflection::Type baseType();
        /*
        *   Function: instanceOf
        *   Return Type: Type
        *   Description: Returns the parent types of this class
        *   Parameters: none
        *   Date Modified: 27/06/2014 by Nathan Hanlan
        */
        virtual Reflection::Type * instanceOf(int & aCount);

    protected:
        u32 m_UsedMemory;
        u32 m_NumberOfAllocations;
    private:
    };

}

#endif