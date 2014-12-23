#include "G_FixedAllocator.h"
#include "G_MemoryUtils.h"
#include <stdio.h>
namespace Gem
{
	FixedAllocator::FixedAllocator() : Allocator()
	{
		m_FreeList = nullptr;
	}
	FixedAllocator::~FixedAllocator()
	{

	}
	///Virtual Allocation/Deallocation Methods
	void * FixedAllocator::Allocate()
	{
		///Error checking
#ifdef MEMORY_ERROR_LEVEL_ASSERT
		assert(m_FreeList != nullptr && "Allocator was not initialized.");
#endif
		if (m_FreeList == nullptr)
		{
			return nullptr;
		}
		///
		uintptr_t address = (uintptr_t)m_FreeList;
		//printf("Allocation Start: %d\n", address);

		void * ptr = m_FreeList;
		m_FreeList = (void**)(*m_FreeList);
		m_UsedMemory += m_ObjectSize;
		m_Allocations++;
		address = (uintptr_t)m_FreeList;
		//printf("Allocation Next: %d\n", address);
		return (void*)((uintptr_t)ptr + Memory::MEMORY_HEADER_SIZE);
	}
	void * FixedAllocator::Deallocate(void * aMemory)
	{
//#ifdef MEMORY_ERROR_LEVEL_ASSERT
//		assert(m_FreeList != nullptr && "Allocator was not initialized.");
//#endif
//		if (m_FreeList == nullptr)
//		{
//			return aMemory;
//		}
		aMemory = (void*)((uintptr_t)aMemory - Memory::MEMORY_HEADER_SIZE);
		if (aMemory == nullptr)
		{
			return nullptr;
		}
		*((void**)aMemory) = m_FreeList;
		m_FreeList = (void**)aMemory;
		m_UsedMemory -= m_ObjectSize;
		m_Allocations--;
		return nullptr;
	}
	
	//Initializes the fixed allocator
	bool FixedAllocator::Initalize(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void * aMemory)
	{
#ifdef MEMORY_ERROR_LEVEL_ASSERT
		assert(aObjectSize >= sizeof(void*) && "Object must be of size greater than a void ptr.");
		assert(aMemory != nullptr && "Cannot initialize a Fixed Allocator with invalid memory.");
#endif
		if (m_Memory != nullptr || aObjectSize < sizeof(void*) || aMemory == nullptr)
		{
			return false;
		}
		m_ObjectAlignment = aObjectAlignment;
		m_TotalSize = aSize;
		m_ObjectSize = aObjectSize;
		m_Memory = aMemory;
		m_SelfAllocated = false;
		unsigned char adjustment = Memory::AlignAdjustment(m_Memory, m_ObjectAlignment);
		m_FreeList = (void**)((uintptr_t)m_Memory + adjustment);
		m_NumberOfObjects = (aSize - adjustment) / m_ObjectSize;

		void ** iter = m_FreeList;
		for (unsigned int i = 0; i < m_NumberOfObjects - 1; i++)
		{
			*iter = (void*)((uintptr_t)iter + m_ObjectSize);
			iter = (void**)*iter;
		}
		*iter = nullptr;
	}
	bool FixedAllocator::Initalize(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize)
	{
#ifdef MEMORY_ERROR_LEVEL_ASSERT
		assert(aObjectSize >= sizeof(void*) && "Object must be of size greater than a void ptr.");
#endif
		if (m_Memory != nullptr || aObjectSize < sizeof(void*))
		{
			return false;
		}
		m_ObjectAlignment = aObjectAlignment;
		m_TotalSize = aSize;
		m_ObjectSize = aObjectSize;
		m_Memory = malloc(aSize);
		m_SelfAllocated = true;
		unsigned char adjustment = Memory::AlignAdjustment(m_Memory, m_ObjectAlignment);
		m_FreeList = (void**)((uintptr_t)m_Memory + adjustment);
		m_NumberOfObjects = (aSize - adjustment) / m_ObjectSize;

		void ** iter = m_FreeList;
		for (unsigned int i = 0; i < m_NumberOfObjects - 1; i++)
		{
			*iter = (void*)((uintptr_t)iter + m_ObjectSize);
			iter = (void**)*iter;
		}
		*iter = nullptr;
	}
	///Remove any allocated memory
	void FixedAllocator::Dispose()
	{
		if (m_SelfAllocated == true)
		{
			free(m_Memory);
		}
		m_Allocations = 0;
		m_UsedMemory = 0;
		m_TotalSize = 0;
		m_ObjectSize = 0;
		m_NumberOfObjects = 0;
		m_ObjectAlignment = 0;
		m_SelfAllocated = false;
		m_Memory = nullptr;
	}

	bool FixedAllocator::Initialized()
	{
		return m_Memory != nullptr;
	}



#pragma region OBSOLETE
	//void * FixedAllocator::Allocate(int aLength)
	//{
	//#ifdef MEMORY_ERROR_LEVEL_ASSERT
	//	assert(m_FreeList != nullptr && "Allocator was not initialized.");
	//#endif
	//	if (m_FreeList == nullptr)
	//	{
	//		return nullptr;
	//	}
	//	uintptr_t address = (uintptr_t)m_FreeList;
	//	printf("Allocation Start: %d\n", address);
	//	void * ptr = m_FreeList;
	//	//m_FreeList = (void**)((uintptr_t)m_FreeList + aLength * m_ObjectSize + sizeof(void*) + Memory::MEMORY_HEADER_SIZE);
	//	m_FreeList = (void**)(*m_FreeList);
	//	address = (uintptr_t)m_FreeList;
	//	printf("Allocation Move_A: %d\n", address);
	//	m_FreeList = (void**)(*m_FreeList);
	//	address = (uintptr_t)m_FreeList;
	//	printf("Allocation Move_B: %d\n", address);
	//
	//	m_UsedMemory += m_ObjectSize * aLength;
	//	m_Allocations++;
	//	address = (uintptr_t)ptr + Memory::MEMORY_HEADER_SIZE;
	//	printf("Header Offset %d\n", address);
	//	return (void*)((uintptr_t)ptr + Memory::MEMORY_HEADER_SIZE);
	//}
	//void * FixedAllocator::Deallocate(void * aMemory, unsigned int aLength)
	//{
	//#ifdef MEMORY_ERROR_LEVEL_ASSERT
	//	assert(m_FreeList != nullptr && "Allocator was not initialized.");
	//#endif
	//	if (m_FreeList == nullptr)
	//	{
	//		return aMemory;
	//	}
	//	if (aMemory == nullptr)
	//	{
	//		return nullptr;
	//	}
	//
	//	uintptr_t address = (uintptr_t)aMemory;
	//	printf("Memory Address %d\n", address);
	//	address = (uintptr_t)(*(void**)aMemory);
	//	printf("Dereferenced Address %d\n", address);
	//	address = (uintptr_t)m_FreeList;
	//	printf("Free List Address %d\n", address);
	//
	//	*((void**)aMemory) = m_FreeList;
	//	m_FreeList = (void**)aMemory;
	//	m_UsedMemory -= m_ObjectSize * aLength;
	//	m_Allocations--;
	//	return nullptr;
	//}
#pragma endregion
}