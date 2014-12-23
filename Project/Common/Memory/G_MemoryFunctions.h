#ifndef G_MEMORY_FUNCTIONS_H
#define G_MEMORY_FUNCTIONS_H

#include "G_MemoryManager.h"
#ifdef GEM_REFLECTION
#include "../Reflection/G_Reflection.h"
#endif
namespace Gem
{
	namespace Memory
	{
#ifdef GEM_REFLECTION
		inline void * Instantiate(char * aTypename)
		{
			return Hidden::MemoryManager::Instance()->Allocate(aTypename);
		}
		inline void * Instantiate(const char * aTypename)
		{
			return Hidden::MemoryManager::Instance()->Allocate(aTypename);
		}
		inline void * Instantiate(std::string aTypename)
		{
			return Hidden::MemoryManager::Instance()->Allocate(aTypename);
		}
		inline void * Instantiate(Type & aType)
		{
			return Hidden::MemoryManager::Instance()->Allocate(aType);
		}

		inline void * Destroy(void * aMemory, char * aTypename)
		{
			return Hidden::MemoryManager::Instance()->Deallocate(aMemory, aTypename);
		}
		inline void * Destroy(void * aMemory, const char * aTypename)
		{
			return Hidden::MemoryManager::Instance()->Deallocate(aMemory, aTypename);
		}
		inline void * Destroy(void * aMemory, std::string aTypename)
		{
			return Hidden::MemoryManager::Instance()->Deallocate(aMemory, aTypename);
		}
		inline void * Destroy(void * aMemory, Type & aType)
		{
			return Hidden::MemoryManager::Instance()->Deallocate(aMemory, aType);
		}
#endif

		///Allocates memory for a given object of type T
		template<class T>
		inline T * Instantiate()
		{
			return Hidden::MemoryManager::Instance()->Allocate_T<T>();
		}
		///Frees the memory for a given object of type T
		template<class T>
		inline T * Destroy(T * aMemory)
		{
			return Hidden::MemoryManager::Instance()->Deallocate_T<T>(aMemory);
		}
		///Instantiates an array of objects of type T, the memory allocated is done so through new[] operator.
		///The memory manager only observes the memory.
		template<class T>
		inline T * Instantiate(unsigned int aLength)
		{
			T * val = new T[aLength];
			Hidden::MemoryManager::Instance()->ArrayAlloc(val, aLength);
			return val;
		}
		///Destroys an array of objects of type T, the memory deallocated is done so through the delete[] operator.
		///The memory manager only observes the memory.
		template<class T>
		inline T * Destroy(T * aMemory, unsigned int aLength)
		{
			if (aMemory != nullptr)
			{
				Hidden::MemoryManager::Instance()->ArrayDealloc(aMemory, aLength);
				delete[] aMemory;
			}
			return nullptr;
		}

		///Memory report functions.
		///Generates a full report.
		inline MemoryReport GetMemoryReport()
		{
			return Hidden::MemoryManager::Instance()->GetMemoryReport();
		}
		///Generates a report on a specific collection of allocators, initial or dynamic
		inline MemoryReport GetMemoryReport(Memory::MemoryReportType aType)
		{
			return Hidden::MemoryManager::Instance()->GetMemoryReport(aType);
		}
		///Generates a report on a specific allocator block
		inline MemoryReport GetMemoryReport(Memory::MemoryBlock aBlock)
		{
			return Hidden::MemoryManager::Instance()->GetMemoryReport(aBlock);
		}
		///Generates a report on a specific allocator block and collection of allocators.
		inline MemoryReport GetMemoryReport(Memory::MemoryBlock aBlock, Memory::MemoryReportType aType)
		{
			return Hidden::MemoryManager::Instance()->GetMemoryReport(aBlock, aType);
		}
#ifdef GEM_REFLECTION
		inline void FlagGC(void * aAddress, Type & aType)
		{
			Hidden::MemoryManager::Instance()->FlagGC(aAddress, aType);
		}
		inline bool IsFlaggedGC(void * aAddress)
		{
			return Hidden::MemoryManager::Instance()->IsFlaggedForGC(aAddress);
		}
		inline void GCCollect()
		{
			Hidden::MemoryManager::Instance()->GCCollect();
		}
		inline void GCCollectAll()
		{
			Hidden::MemoryManager::Instance()->GCCollectAll();
		}
#endif
	}
}

#endif