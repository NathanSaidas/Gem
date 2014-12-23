#ifndef G_ALLOCATOR_H
#define G_ALLOCATOR_H

#pragma region CHANGE LOG
/*	December, 17, 2014 - Nathan Hanlan - Added class Allocator (Memory Management 2.0)
*	December, 17, 2014 - Nathan Hanlan - Implemented base class features as well as error handling using MEMORY_ERROR_LEVEL_ASSERT
*	December, 18, 2014 - Nathan Hanlan - Array allocations and deallocations are no longer supported.
*/
#pragma endregion

#include "G_MemoryDefinitions.h"
#include "G_MemoryUtils.h"
#include <assert.h>
#include <iostream>

namespace Gem
{
	///The base allocator class. Implement the allocation methods below.
	class Allocator
	{
	public:
		Allocator();
		Allocator(const Allocator & aRef);
		virtual ~Allocator();

		///Virtual Allocation/Deallocation Methods
		virtual void * Allocate() = 0;
		virtual void * Deallocate(void * aMemory) = 0;


		///Remove any allocated memory
		virtual void Dispose() = 0;

		///Type Allocation/Deallocation Methods
#pragma region TYPE ALLOCATIONS
		template<class T>
		T * Allocate_T()
		{
			void * ptr = Allocate();
#ifdef MEMORY_ERROR_LEVEL_ASSERT
			assert(ptr != nullptr && "Allocator failed to allocate memory.");
#endif
			if (ptr != nullptr)
			{
				T * obj = new(ptr)T();
				return obj;
			}
			return nullptr;
		}
		template<class T>
		T * Deallocate_T(T * aMemory)
		{
#ifdef MEMORY_ERROR_LEVEL_ASSERT
			assert(aMemory != nullptr && "Allocator failed to deallocate memory. Memory is null.");
#endif
			
			if (aMemory != nullptr)
			{
				aMemory->~T();
			}
			return (T*)Deallocate(aMemory);
		}
		
#pragma endregion


	public:
		///Accessors to the properties
		unsigned char ID();
		unsigned char ID(unsigned char aID);
		unsigned int Allocations();
		unsigned int UsedMemory();
		unsigned int TotalSize();
		unsigned int ObjectSize();
		unsigned int NumberOfObjects();
		unsigned char ObjectAlignment();
		float ExpiredTime();
		float ExpiredTime(float aValue);
		bool IsExpired();
		bool IsExpired(bool aValue);
		
		///If the allocator has enough memory for the allocation. The aAmount param is in allocations not memory
		bool HasMemory(unsigned int aAmount);
		///Returns true if the allocator is out of memory.
		bool OutOfMemory();

		Memory::AllocatorMemInfo GetMemoryInfo();

	protected:
		

		unsigned char m_ID;
		unsigned int m_Allocations;
		unsigned int m_UsedMemory;
		unsigned int m_TotalSize;
		unsigned int m_ObjectSize;
		unsigned int m_NumberOfObjects;
		unsigned char m_ObjectAlignment;
		bool m_SelfAllocated;
		void * m_Memory;
		///Time the Allocator was declared Expired
		float m_ExpiredTime;
		bool m_IsExpired;

	private:
#pragma region OBSOLETE
			///Array Allocations are no longer supported as of December, 18, 2014
			//virtual void * Allocate(int aLength) = 0;
			//virtual void * Deallocate(void * aMemory, unsigned int aLength) = 0;
			//template<class T>
			//T * Allocate_T(unsigned int aLength)
			//{
			//	void * ptr = Allocate(aLength);
			//#ifdef MEMORY_ERROR_LEVEL_ASSERT
			//	assert(ptr != nullptr && "Allocator failed to allocate array of memory.");
			//#endif
			//	if (ptr != nullptr)
			//	{
			//		T * obj = new(ptr)T[aLength];
			//		uintptr_t address = (uintptr_t)obj;
			//		printf("Array offset: %d\n", address);
			//		return obj;
			//	}
			//	return nullptr;
			//}
			//template<class T>
			//T * Deallocate_T(T * aMemory, unsigned int aLength)
			//{
			//#ifdef MEMORY_ERROR_LEVEL_ASSERT
			//	assert(aMemory != nullptr && "Allocator failed to deallocate array of memory. Memory is null.");
			//#endif
			//	return (T*)Deallocate(aMemory, aLength);
			//}
#pragma endregion

	};

}

#endif