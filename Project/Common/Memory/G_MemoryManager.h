#ifndef G_MEMORY_MANAGER_H
#define G_MEMORY_MANAGER_H

#pragma region CHANGE LOG
/*	December, 17, 2014 - Nathan Hanlan - Added class Memory Manager.
*	December, 22, 2014 - Nathan Hanlan - Put MemoryManager inside Memory namespace
*	December, 22, 2014 - Nathan Hanlan - Adding in Garbage Collection Features
*	December, 22, 2014 - Nathan Hanlan - GetAllocator now returns Dynamic Allocators which have not expired.
*	December, 22, 2014 - Nathan Hanlan - Allocate and Deallocate methods now properlly Expire / Renew Dynamic Allocators when they are no longer needed based on the defined Memory Manager State
*	December, 22, 2014 - Nathan Hanlan - Added and implemented Garbage Collection based on Cycles and Total Time. This is handled on the main thread. Garbage Collection is only available with GEM_REFLECTION
*/
#pragma endregion

#include "G_MemoryDefinitions.h"
#include "G_FixedAllocator.h"
#include "../G_Config.h"

#include <string>
#include <vector>
#include <map>

namespace Gem
{
	class Type;
	namespace Memory
	{

		namespace Hidden
		{
			//The memory manager class is responsible for holding all the allocators and the giant (or small) heaps of memory.
			//as well as the root functions for allocating and deallocating memory objects
			class MemoryManager
			{
			public:
				///Singleton Methods
				///Creates the instance
				static MemoryManager * Instance();
				///Destroys the instance
				static void Destroy();

				///Void Ptr Allocation/Deallocations methods. These do not invoke constructors / destructors thus they do not 
				///initialize the virtual function table.
				void * Allocate(unsigned int aSize);
				void * Deallocate(void * aMemory, unsigned int aSize);

#ifdef GEM_REFLECTION
				///Allocation/Deallocation Methods available if using GEM_REFLECTION
				void * Allocate(char * aTypename);
				void * Allocate(const char * aTypename);
				void * Allocate(std::string aTypename);
				void * Allocate(Type & aType);

				void * Deallocate(void * aMemory, char * aTypename);
				void * Deallocate(void * aMemory, const char * aTypename);
				void * Deallocate(void * aMemory, std::string aTypename);
				void * Deallocate(void * aMemory, Type & aType);
#endif


				///Type Ptr Allocation/Deallocation methods. These do invoke constructors / destructors
				template<class T>
				T * Allocate_T()
				{
					///Get an allocator and allocate the object
					unsigned int size = sizeof(T);
					FixedAllocator * allocator = GetAllocator(size + Memory::MEMORY_HEADER_SIZE);
					///Error Handling
#ifdef MEMORY_ERROR_LEVEL_ASSERT
					assert(allocator != nullptr && "Failed to retrieve an allocator to allocate with.");
					assert(allocator->Initialized() && "Failed to retrieve an allocator to allocate with.");
#endif
					if (allocator == nullptr || !allocator->Initialized())
					{
						return nullptr;
					}
					///Allocate memory and initialize the object
					///The memory address that gets returned from allocation functions is already offset.
					T * obj = allocator->Allocate_T<T>();
					//Write to memory header.
					void * memoryHeader = (void*)((uintptr_t)obj - Memory::MEMORY_HEADER_SIZE);
					int headerMask = WriteHeaderMask(Memory::FLAG_VALID, allocator->ID());
					memcpy(memoryHeader, &headerMask, sizeof(int));
					return obj;
				}

				template<class T>
				T * Deallocate_T(T * aMemory)
				{
					unsigned int size = sizeof(T);
					///Read in the memory mask
					unsigned int objectFlag = 0;
					int allocatorID = 0;
					ReadHeaderMask(aMemory, objectFlag, allocatorID);
					if (objectFlag == FLAG_GC)
					{
						Collect(aMemory);
					}
					///Error Check the Mask
#ifdef MEMORY_ERROR_LEVEL_ASSERT
					assert(!(objectFlag == Memory::FLAG_INVALID || allocatorID == -1) && "Failed to read memory header mask");
#endif
					if (objectFlag == Memory::FLAG_INVALID || allocatorID == -1)
					{
						return aMemory;
					}
					///Retrieve an allocator and deallocate the object.
					FixedAllocator * allocator = GetAllocatorByID(size + Memory::MEMORY_HEADER_SIZE, allocatorID);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
					assert(allocator != nullptr && "Failed to retrieve an allocator to deallocate with.");
					assert(allocator->Initialized() && "Failed to retrieve an allocator to deallocate with.");
#endif
					if (allocator == nullptr)
					{
						return aMemory;
					}
					T * result = allocator->Deallocate_T<T>((T*)((uintptr_t)aMemory));
					///If the allocator should expire.
					if (allocator->ID() != 0 && allocator->Allocations() == 0)
					{
						ExpireAllocator(allocator);
					}
					return result;
				}



				///These methods generate reports on how much memory is being used currently.
				Memory::MemoryReport GetMemoryReport();
				Memory::MemoryReport GetMemoryReport(Memory::MemoryReportType aType);
				Memory::MemoryReport GetMemoryReport(Memory::MemoryBlock aBlock);
				Memory::MemoryReport GetMemoryReport(Memory::MemoryBlock aBlock, Memory::MemoryReportType aType);
				
				///Method to watch arrays as they get allocated and deallocated
				void ArrayAlloc(void * aAddress, unsigned int aLength);
				void ArrayDealloc(void * aAddress, unsigned int aLength);

				///Flags an item for garbage collection. When using reflection the type is required to allow the object's destructor to get called.
				bool IsFlaggedForGC(void * aMemory);
				///FlagGC has no implementation without GEM_REFLECTION
				void FlagGC(void * aAddress, Type & aType);
				///GCCollect tells the garbage collector to collect items. These methods have no implementation without GEM_REFLECTION
				///This GCCollect uses the default collection constants for chunksize and time.
				void GCCollect();
				///@aChunkSize - The amount of items removed before time is evaluated again.
				///@aTime - How much total time should the collector spend collecting.
				void GCCollect(unsigned int aChunkSize, float aTime);
				///The garbage collection removes all flagged items.
				void GCCollectAll();
				///Updates the allocators. If allocators are expired they are deleted.
				void Update();
			protected:
				///Retrieving allocators based on allocation size. Checks for available memory
				FixedAllocator * GetAllocator(unsigned int aSize);
				FixedAllocator * GetAllocator(unsigned int aSize, unsigned int aLength);
				///Retrieving allocators based on ID. Does not check for available memory.
				FixedAllocator * GetAllocatorByID(unsigned int aSize, unsigned char aID);
				///Creates a dynamic allocator with the given object size.
				void CreateAllocator(unsigned int aSize);
				///Read and write functions for Memory Headers
				unsigned int WriteHeaderMask(unsigned int aObjectFlag, unsigned int aAllocator);
				void WriteHeaderMask(void * aMemory, unsigned int aObjectFlag, int aAllocator);
				void ReadHeaderMask(void * aMemory, unsigned int & aObjectFlag, int & aAllocator);
				///Prints out all memory leaks.
				void PrintLeaks(FixedAllocator * aAllocator, int aIndex);
				///Removes an allocator based on the what mode has been defined in the MemoryDefinitions
				void ExpireAllocator(FixedAllocator * aAllocator);
				void RenewAllocaator(FixedAllocator * aAllocator);
				///Removes an item from the garbage collection list.
				void Collect(void * aAddress);
				///Initial allocator and Dynamic allocators.
				FixedAllocator * m_Allocators[12];
				std::vector<FixedAllocator*> m_DynamicAllocators;
				std::map<uintptr_t, Memory::ArrayAlloc> m_ArrayAllocations;
#ifdef GEM_REFLECTION
				std::vector<GCItem> m_GarbageList;
#endif
				unsigned char m_DynamicID;
			private:
				MemoryManager();
				~MemoryManager();
				static MemoryManager * s_Instance;
#pragma region OBSOLETE
				///Array Allocations are no longer supported as of December, 18, 2014
				//void * Allocate(unsigned int aSize, unsigned int aLength);
				//void * Deallocate(void * aMemory, unsigned int aSize, unsigned int aLength);
				//template<class T>
				//T * Allocate_T(unsigned int aLength)
				//{
				//	unsigned int size = sizeof(T);
				//	///Get an allocator
				//	FixedAllocator * allocator = GetAllocator(size + Memory::MEMORY_HEADER_SIZE, aLength);
				//
				//#ifdef MEMORY_ERROR_LEVEL_ASSERT
				//	assert(allocator != nullptr && "Failed to retrieve an allocator to allocate with.");
				//	assert(allocator->Initialized() && "Failed to retrieve an allocator to allocate with.");
				//#endif
				//	if (!allocator->Initialized())
				//	{
				//		return nullptr;
				//	}
				//	T * allocatedObj = allocator->Allocate_T<T>(aLength);
				//	uintptr_t address = (uintptr_t)allocatedObj;
				//	///Write to memory header
				//	void * memoryHeader = (void*)((uintptr_t)allocatedObj - Memory::MEMORY_HEADER_SIZE);
				//	int headerMask = WriteHeaderMask(Memory::FLAG_VALID, allocator->ID());
				//	memcpy(memoryHeader, &headerMask, sizeof(int));
				//
				//	return allocatedObj;
				//}
				//
				//template<class T>
				//T * Deallocate_T(T * aMemory, unsigned int aLength)
				//{
				//	unsigned int size = sizeof(T);
				//	///Read in the memory mask
				//	unsigned int objectFlag = 0;
				//	int allocatorID = 0;
				//	ReadHeaderMask(aMemory, objectFlag, allocatorID);
				//	///Error Check the Mask
				//#ifdef MEMORY_ERROR_LEVEL_ASSERT
				//	assert(!(objectFlag == Memory::FLAG_INVALID || allocatorID == -1) && "Failed to read memory header mask");
				//#endif
				//	if (objectFlag == Memory::FLAG_INVALID || allocatorID == -1)
				//	{
				//		return aMemory;
				//	}
				//	///Retrieve an allocator and deallocate the object.
				//	FixedAllocator * allocator = GetAllocatorByID(size + Memory::MEMORY_HEADER_SIZE, allocatorID);
				//
				//#ifdef MEMORY_ERROR_LEVEL_ASSERT
				//	assert(allocator != nullptr && "Failed to retrieve an allocator to deallocate with.");
				//	assert(allocator->Initialized() && "Failed to retrieve an allocator to deallocate with.");
				//#endif
				//	return allocator->Deallocate_T<T>((T*)((uintptr_t)aMemory - sizeof(void*)-Memory::MEMORY_HEADER_SIZE), aLength);
				//}
#pragma endregion
			};
		}
	}
}

#endif