#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H



#include "Allocator.h"

namespace Gem
{

    //The Pool Allocator by Tiago Costa
    //From http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010
    //This allocator only allows allocations of a fixed size and alignment to be made, this results in both fast allocations and deallocations to be made. 
    //Like the FreeList allocator, a linked-list of free blocks is maintaied but since all blocks are the same size each free block only needs to store a pointer to the next one.
    //Another advantage of Pool allactors is no need to align each allocation, since all allocations have the same size/alignment only the first block has to be aligned, 
    //this results in a almost non-existant memory overhead.

    /*
    *   Class: PoolAllocator
    *   Base Class: Allocator
    *   Interfaces: none
    *   Description: Assigns objects a memory space of a fixed size.
    *   Date Modified: 27/06/2014 by Nathan Hanlan
    */
    class PoolAllocator : public Allocator
    {
    public:
        PoolAllocator(u32 aObjectSize, u8 aObjectAlignment, u32 aSize, void* aMem);
        ~PoolAllocator();

        //Previous allocate and deallocate methods
        void * allocate(u32 aSize, u8 aAlignment);
        void deallocate(void * p);

        //Rewrote the Allocate and Deallocate method to be used as a template for various different classes.
        /*
        *   Function: allocate
        *   Return Type: a pointer to the given T
        *   Description: Assigns a pointer to a memory address and invokes the constructor
        *   Parameters: none
        *   Date Modified: 24/07/2014 by Nathan Hanlan
        */
        template<class T>
        inline T * allocate()
        {
            //Make sure the free list exists
            ASSERT(m_FreeList != nullptr);
            if(m_FreeList == nullptr)
            {
                return nullptr;
            }
            void * p = m_FreeList;
            //Adjust freelist pointer.
            m_FreeList = (void**)(*m_FreeList);
            //Modify memory stats
            m_UsedMemory += m_ObjectSize;
            m_NumberOfAllocations ++;
            //Invoke the constructor of the object but using the given address of 'p'
            T * tObj = new(p)T();
            return tObj;
        }
        /*
        *   Function: allocate
        *   Return Type: a pointer to the given T
        *   Description: Assigns a pointer to a memory address and invokes the constructor
        *   Parameters: @int aLength - The size of the array
        *   Date Modified: 24/07/2014 by Nathan Hanlan
        */
        template<class T>
        inline T * allocate(int aLength)
        {
            ASSERT(m_FreeList != nullptr);
            if(m_FreeList == nullptr)
            {
                return nullptr;
            }

            void * p = m_FreeList;
            m_FreeList = (void**)(*m_FreeList);

            m_UsedMemory += m_ObjectSize;
            m_NumberOfAllocations ++;

            T * tObjArray = new(p)T[aLength];
            return tObjArray;
        }
        /*
        *   Function: deallocate
        *   Return Type: void
        *   Description: Frees up the given memory address
        *   Parameters: @void *p - The address pointer
        *   Date Modified: 24/07/2014 by Nathan Hanlan
        */
        template<class T>
        inline void deallocate(void * p)
        {
            //Make sure the address is valid
            ASSERT(p != nullptr);
            if(p == nullptr)
            {
                return;
            }
            //Cast the object to a type
            T * object = (T*)p;
            object->T::~T();
            //Adjust the ferelist pointer
            *((void**)p) = m_FreeList;
            m_FreeList = (void**)p;
            //Modify memory stats
            m_UsedMemory -= m_ObjectSize;
            m_NumberOfAllocations--;
        }

        inline void deallocateSize(Object * p)
        {
            //Make sure the address is valid
            ASSERT(p != nullptr);
            if(p == nullptr)
            {
                return;
            }

            p->Object::~Object();

            //Adjust the ferelist pointer
            *((void**)p) = m_FreeList;
            m_FreeList = (void**)p;
            //Modify memory stats
            m_UsedMemory -= m_ObjectSize;
            m_NumberOfAllocations--;
        }

        /*
        *   Function: deallocate
        *   Return Type: void
        *   Description: Frees up the given memory address
        *   Parameters: @void *p - The address pointer
        *   @int aLength - The length of the array
        *   Date Modified: 24/07/2014 by Nathan Hanlan
        */
        template<class T>
        inline void deallocate(void * p, int aLength)
        {
            ASSERT(p != nullptr);
            if(p == nullptr)
            {
                return;
            }
            T * object = (T*)p;
            object->T::~T[aLength]();

            *((void**)p) = m_FreeList;
            m_FreeList = (void**)p;

            m_UsedMemory -= m_ObjectSize;
            m_NumberOfAllocations--;
        }

        

        virtual Reflection::Type * getType();

        inline int objectSize()
        {
            return m_ObjectSize;
        }

    private:
        PoolAllocator(const PoolAllocator&){}
        PoolAllocator& operator=(const PoolAllocator&){}
        u32 m_Size;
        u32 m_ObjectSize;
        u32 m_NumberOfObjects;
        u8 m_ObjectAlignment;
        void ** m_FreeList;
    };

}


#endif