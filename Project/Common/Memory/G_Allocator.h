#ifndef G_ALLOCATOR_H
#define G_ALLOCATOR_H


#include <assert.h>
#include <exception>
#include <iostream>
#include <vector>
#include "G_MemoryHandle.h"

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

	struct AllocatorReport
	{
		unsigned int allocations;
		unsigned int usedMemory;
		unsigned int totalSize;
		unsigned int numberOfObjects;
	};

    class Allocator
    {
    public:
		Allocator();
        Allocator(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void* aMem);
        ~Allocator();

		virtual void Init(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void* aMem);


        Reflection::Primitive * Allocate(string & aTypeName);
        Reflection::Primitive * Allocate(integer & aTypeID);
        Reflection::Primitive * Allocate(Reflection::Type * aType);
        Reflection::Primitive * Deallocate(Reflection::Primitive * aPtr, string & aTypeName);
        Reflection::Primitive * Deallocate(Reflection::Primitive * aPtr, integer & aTypeID);
        Reflection::Primitive * Deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType);

        template<class T>
        T * Allocate()
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
		T * Allocate(const T & aInitializer)
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

			T * obj = new(ptr)T(aInitializer);

			return obj;
		}

        template<class T>
        T * Deallocate(T * aPtr)
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
        T * AllocateArray(unsigned int aLength)
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
            objArray = (T*)(objArray);

            return objArray;
        }

        template<class T>
        T * DeallocateArray(T * aAddress, unsigned int aLength)
        {
            ASSERT(aAddress != nullptr);
            if (aAddress == nullptr)
            {
                return nullptr;
            }
			T * obj = (aAddress);
            //obj->~T[aLength]();
			obj->~T();

            *((void**)aAddress) = m_FreeList;
            m_FreeList = (void**)aAddress;

            m_UsedMemory -= m_ObjectSize;
            m_Allocations--;

            return nullptr;
        }
		int Allocations()
		{
			return m_Allocations;
		}
		int UsedMemory()
		{
			return m_UsedMemory;
		}
		int ObjectSize()
		{
			return m_ObjectSize;
		}
		int NumberOfObjects()
		{
			return m_NumberOfObjects;
		}
		void GetInfo(AllocatorReport & aReport)
		{
			aReport.allocations = m_Allocations;
			aReport.numberOfObjects = m_NumberOfObjects;
			aReport.totalSize = m_TotalSize;
			aReport.usedMemory = m_UsedMemory;
		}
		bool OutOfMemory()
		{
			return m_Allocations >= m_NumberOfObjects;
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