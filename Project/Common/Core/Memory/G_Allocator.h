#ifndef G_ALLOCATOR_H
#define G_ALLOCATOR_H


#include <assert.h>
#include <exception>
#include <iostream>
#include <vector>

#define ASSERT assert

namespace Gem
{
    //Forward declarations
    namespace Reflection
    {
        class Primitive;
        class Type;
    }
    class string;
    class integer;

    class Allocator
    {
    public:
        Allocator(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void* aMem);
        ~Allocator();


        Reflection::Primitive * allocate(string & aTypename);
        Reflection::Primitive * allocate(integer & aTypeID);
        Reflection::Primitive * allocate(Reflection::Type * aType);
        Reflection::Primitive * deallocate(Reflection::Primitive * aPtr, string & aTypename);
        Reflection::Primitive * deallocate(Reflection::Primitive * aPtr, integer & aTypeID);
        Reflection::Primitive * deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType);

        template<class T>
        T * allocate()
        {
            ASSERT(m_FreeList != nullptr);
            if (m_FreeList == nullptr)
            {
                return nullptr;
            }

            void * ptr = m_FreeList;

            m_FreeList = (void**)(*m_FreeList);

            m_UsedMemory += m_ObjectSize;
            m_Allocations++;

            T * obj = new(ptr)T();

            return obj;
        }
        template<class T>
        T * deallocate(T * aPtr)
        {
            ASSERT(aPtr != nullptr);
            if (aPtr == nullptr)
            {
                return nullptr;
            }

            aPtr->T::~T();

            *((void**)aPtr) = m_FreeList;
            m_FreeList = (void**)aPtr;

            m_UsedMemory -= m_ObjectSize;
            m_Allocations--;
            return nullptr;
        }

        template<class T>
        T * allocateArray(unsigned int aLength)
        {
            ASSERT(m_FreeList != nullptr);
            if (m_FreeList == nullptr)
            {
                return nullptr;
            }
            void * ptr = m_FreeList;
            //Adjust the free list ptr by 2
            m_FreeList = (void**)(*m_FreeList);
            m_FreeList = (void**)(*m_FreeList);

            m_UsedMemory += m_ObjectSize;
            m_Allocations++;

            T * objArray = new(ptr)T[aLength];
            objArray = (T*)(*objArray);

            return objArray;
        }

        template<class T>
        T * deallocateArray(T * aAddress, unsigned int aLength)
        {
            ASSERT(aAddress != nullptr);
            if (aAddress == nullptr)
            {
                return nullptr;
            }
            T * obj = (T*)(&aAddress);
            obj->T::~T[aLength]();

            *((void**)aAddress) = m_FreeList;
            m_FreeList = (void**)aAddress;

            m_UsedMemory -= m_ObjectSize;
            m_Allocations--;

            return nullptr;
        }
    private:
        //Number of Allocations made
        unsigned int m_Allocations;
        //How much memory has been used by the allocator
        unsigned int m_UsedMemory;


        //Total size of the object allocation
        unsigned int m_TotalSize;
        //Size of each allocation
        unsigned int m_ObjectSize;
        //Number of Objects?
        unsigned int m_NumberOfObjects;
        //What is the object alignment for this memory
        unsigned char m_ObjectAlignment;
        //The list of free pointers.
        void ** m_FreeList;
    };
}

#endif