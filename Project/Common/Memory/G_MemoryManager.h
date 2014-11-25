#ifndef G_MEMORY_MANAGER_H
#define G_MEMORY_MANAGER_H

#pragma region CHANGE LOG
/* November,18,2014 - Nathan, MemoryManager now allocates handles for template Allocation Methods
*  November,18,2014 - Nathan, MemoryManager now has a garbage collector.See GCFlag and GCCollect
*  November,18,2014 - Nathan, MemoryManager now support dynamic memory allocation when fixed memory is empty.
*/
#pragma endregion

#include "G_Allocator.h"
#include "G_DynamicAllocator.h"
#include "G_MemoryHandle.h"
//#include "../Utilities/Collections/G_Iterators.h"

//Byte Defintiions
#define KILO_BYTE 1024             //1024
#define MEGA_BYTE 1024 * KILO_BYTE //1048576
#define GIGA_BYTE 1024 * MEGA_BYTE //1073741824


namespace Gem
{
	///Class Declarations
	namespace Collections
	{
		template<class T>
		class List;
		template<class T>
		class Iterator;
	}
	class UniqueNumberGenerator;

    namespace Memory
    {
        //Fixed Memory Allocatoin Sizes for Application Memory.
		const int BLOCK_8_ALLOC_SIZE = 1 * MEGA_BYTE;
        const int BLOCK_16_ALLOC_SIZE = 1 * MEGA_BYTE;
        const int BLOCK_32_ALLOC_SIZE = 1 * MEGA_BYTE;
        const int BLOCK_64_ALLOC_SIZE = 10 * MEGA_BYTE;
        const int BLOCK_128_ALLOC_SIZE = 2 * MEGA_BYTE;
        const int BLOCK_256_ALLOC_SIZE = 2 * MEGA_BYTE;
        const int BLOCK_512_ALLOC_SIZE = 2 * MEGA_BYTE;
        const int BLOCK_1024_ALLOC_SIZE = 1 * MEGA_BYTE;

        //Big Block Memory allocation sizes are used as a last resort if the object you're attempting to allocate does not fit in any of the other blocks.
        //Each Allocation is 8192bytes  Big in this case
        //There are 7040 allocations
        //Thus 55 MB in BIG_BLOCK allocations
        const int BIG_BLOCK_SIZE = 8192;
        const int BIG_BLOCK_ALLOCS = 2;
        const int BIG_BLOCK_ALLOC_SIZE = BIG_BLOCK_SIZE * BIG_BLOCK_ALLOCS;

		///Dynamic Memory Allocation Sizes for everything excluding Reserved.
		const int DYN_BLOCK_8_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_16_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_32_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_64_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_128_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_256_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_512_ALLOC_SIZE = 2 * MEGA_BYTE;
		const int DYN_BLOCK_1024_ALLOC_SIZE = 2 * MEGA_BYTE;

		///Fixed Memory Allocation Sizes for Collections::Iterators
		const int COLLECTIONS_BLOCK_8_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_16_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_32_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_64_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_128_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_256_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_512_ALLOC_SIZE = 1 * MEGA_BYTE;
		const int COLLECTIONS_BLOCK_1024_ALLOC_SIZE = 1 * MEGA_BYTE;


		//Memory for custom stuff such as allocating dynamic allocators
		const int RESERVED_BLOCK_SIZE = 2 * KILO_BYTE;

		///Garbage Collection Constants
		///How much time in ms should the garbage collector be allowed to run
		const float GC_STANDARD_COLLECTION_TIME = 0.016f;
		///How many items should the garbage collector collect before checking the time.
		const int GC_STANDARD_CHUNK_SIZE = 32;

		///Dynamic / Fixed Memory
		const int HANDLE_BLOCK_FIXED = 0;
		///An allocator could not be found.
		const int HANDLE_INVALID_MEMORY = -1;
		const int HANDLE_DEALLOCATED = -2;

        //There is currently 8 block sizes in powers of two
        //There is a ninth block size which is to be user defined.
        enum class BlockSize
        {
            BLOCK_8,
            BLOCK_16,
            BLOCK_32,
            BLOCK_64,
            BLOCK_128,
            BLOCK_256,
            BLOCK_512,
            BLOCK_1024,
            BLOCK_BIG,
            BLOCK_COUNT,
        };
        enum class ByteSize
        {
            BYTE,
            KILOBYTE,
            MEGABYTE,
            GIGABYTE
        };
    }

	
	//Memory Report struct contains information about Allocators.
	struct MemoryReport
	{
		AllocatorReport block8;
		AllocatorReport block16;
		AllocatorReport block32;
		AllocatorReport block64;
		AllocatorReport block128;
		AllocatorReport block256;
		AllocatorReport block512;
		AllocatorReport block1024;
		AllocatorReport bigBlock;
	};


    class MemoryManager
    {
    public:
        static MemoryManager * Instance();
        static void Destroy();

        Reflection::Primitive * Allocate(string & aTypeName);
        Reflection::Primitive * Allocate(integer & aTypeID);
        Reflection::Primitive * Allocate(Reflection::Type * aType);
        Reflection::Primitive * Deallocate(Reflection::Primitive * aPtr, string & aTypeName);
        Reflection::Primitive * Deallocate(Reflection::Primitive * aPtr, integer & aTypeID);
        Reflection::Primitive * Deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType);

		///Allocates an object of T type. The handle is returned however it can be 
		///casted to the T type. The handle contains information about where the memory was allocated.
        ///Allocations will favor the fixed allocators however if they are full a dynamic one will be chosen.
		template<class T>
        MemoryHandle<T> Allocate()
        {
            int size = sizeof(T);
            Allocator * allocator = GetAllocator(size);
            if (allocator == nullptr)
            {
                return MemoryHandle<T>(Memory::HANDLE_INVALID_MEMORY,nullptr);
            }
			if (allocator->OutOfMemory())
			{
				DynamicAllocator * dynAllocator = GetDynamicAllocator(size);
				if (dynAllocator != nullptr)
				{
					return MemoryHandle<T>(dynAllocator->GetID(), dynAllocator->Allocate<T>());
				}
				else
				{
					dynAllocator = CreateDynamicAllocator(size);
					if (dynAllocator == nullptr)
					{
						return MemoryHandle<T>(Memory::HANDLE_INVALID_MEMORY, nullptr);
					}
					else

					{
						return MemoryHandle<T>(dynAllocator->GetID(), dynAllocator->Allocate<T>());
					}
				}
			}
			return MemoryHandle<T>(Memory::HANDLE_BLOCK_FIXED, allocator->Allocate<T>());
        }

		//template<class T>
		//MemoryHandle<Collections::Iterator<T>> SomeFunc()
		//{
		//	return MemoryHandle<Collections::Iterator<T>>(0, nullptr);
		//}

		template<class T>
		MemoryHandle<Collections::Iterator<T>> AllocateIterator()
		{
			int size = sizeof(Collections::Iterator<T>);
			Allocator * allocator = GetCollectionsAllocator(size);
			if (allocator == nullptr)
			{
				return MemoryHandle<Collections::Iterator<T>>(Memory::HANDLE_INVALID_MEMORY, nullptr);
			}
			if (allocator->OutOfMemory())
			{
				//TODO: Get a dynamic allocator and allocate memory
				DynamicAllocator * dynAllocator = GetCollectionsDynamicAllocator(size);
				if (dynAllocator != nullptr)
				{
					Collections::Iterator<T> * iter = dynAllocator->Allocate<Collections::Iterator<T>>();
					//iter->m_MemoryHandle = dynAllocator->GetID();
					SetIteratorHandle(iter, dynAllocator->GetID());
					return MemoryHandle<Collections::Iterator<T>>(dynAllocator->GetID(), iter);
				}
				else
				{
					dynAllocator = CreateDynamicAllocator(size);
					if (dynAllocator != nullptr)
					{
						Collections::Iterator<T> * iter = dynAllocator->Allocate<Collections::Iterator<T>>();
						//iter->m_MemoryHandle = dynAllocator->GetID();
						SetIteratorHandle(iter, dynAllocator->GetID());
						return MemoryHandle<Collections::Iterator<T>>(dynAllocator->GetID(), iter);
					}
					else
					{
						return MemoryHandle<Collections::Iterator<T>>(Memory::HANDLE_INVALID_MEMORY, nullptr);
					}
				}
			}
			Collections::Iterator<T> * iter = MemoryHandle<Collections::Iterator<T>>(Memory::HANDLE_BLOCK_FIXED, allocator->Allocate<Collections::Iterator<T>>());
			//iter->m_MemoryHandle = Memory::HANDLE_BLOCK_FIXED;
			SetIteratorHandle(iter, Memory::HANDLE_BLOCK_FIXED);
			return MemoryHandle<Collections::Iterator<T>>(Memory::HANDLE_BLOCK_FIXED,iter);
		}
		///Assumes the allocator is in fixed memory. 
		///Don't use this unless you are certain it is there.
        template<class T>
        T * Deallocate(T * aAddress)
        {
            int size = sizeof(T);
            Allocator * allocator = GetAllocator(size);
            if (allocator == nullptr)
            {
                return aAddress;
            }
            return allocator->Deallocate<T>(aAddress);
        }
		///Checks the handle block ID and retrieves the dynamic allocator whos block ID matches.
		template<class T>
		T * Deallocate(MemoryHandle<T> & aHandle)
		{
			int size = sizeof(T);
			if (aHandle.BlockID() == Memory::HANDLE_BLOCK_FIXED)
			{
				Allocator * allocator = GetAllocator(size);
				if (allocator != nullptr)
				{
					return allocator->Deallocate<T>(aHandle);
				}
			}
			else
			{
				DynamicAllocator * allocator = GetDynamicAllocatorByID(aHandle.BlockID());
				if (allocator == nullptr)
				{
					aHandle.m_BlockID = Memory::HANDLE_INVALID_MEMORY;
					return aHandle;
				}
				else
				{
					return allocator->Deallocate<T>(aHandle);
				}
			}
			return aHandle;
		}
		template<class T>
		MemoryHandle<T> DeallocateHandle(MemoryHandle<T> & aHandle)
		{
			int size = sizeof(T);
			if (aHandle.BlockID() == Memory::HANDLE_BLOCK_FIXED)
			{
				Allocator * allocator = GetAllocator(size);
				if (allocator != nullptr)
				{
					return MemoryHandle<T>(Memory::HANDLE_DEALLOCATED, allocator->Deallocate<T>(aHandle));
				}
			}
			else
			{
				DynamicAllocator * allocator = GetDynamicAllocatorByID(aHandle.BlockID());
				if (allocator == nullptr)
				{
					return aHandle;
				}
				else
				{
					return MemoryHandle<T>(Memory::HANDLE_DEALLOCATED, allocator->Deallocate<T>(aHandle));
				}
			}
			return aHandle;
		}
		template<class T>
		Collections::Iterator<T> * DeallocateIterator(Collections::Iterator<T> * aIterator)
		{
			int handle = GetIteratorHandle(aIterator);
			if (handle == Memory::HANDLE_BLOCK_FIXED)
			{
				Allocator * allocator = GetCollectionsAllocator(sizeof(Collections::Iterator<T>));
				if (allocator != nullptr)
				{
					return allocator->Deallocate<Collections::Iterator<T>>(aIterator);
				}
			}
			else
			{
				DynamicAllocator * allocator = GetCollectionsDynamicAllocatorByID(handle);
				if (allocator != nullptr)
				{
					return allocator->Deallocate<Collections::Iterator<T>>(aIterator);
				}
			}
			return aIterator;
		}


        //template<class T>
        //T * AllocateArray(unsigned int aLength)
        //{
        //    int size = sizeof(T) * aLength;
        //    Allocator * allocator = GetAllocator(size);
        //    if (allocator == nullptr)
        //    {
        //        return nullptr;
        //    }
        //    return allocator->AllocateArray<T>(aLength);
        //}

		template<class T>
		MemoryHandle<T> AllocateArray(unsigned int aLength)
		{
			int size = sizeof(T);
			Allocator * allocator = GetAllocator(size);
			if (allocator == nullptr)
			{
				return MemoryHandle<T>(Memory::HANDLE_INVALID_MEMORY, nullptr);
			}
			if (allocator->OutOfMemory())
			{
				DynamicAllocator * dynAllocator = GetDynamicAllocator(size);
				if (dynAllocator != nullptr)
				{
					return MemoryHandle<T>(dynAllocator->GetID(), dynAllocator->AllocateArray<T>(aLength));
				}
				else
				{
					dynAllocator = CreateDynamicAllocator(size);
					if (dynAllocator == nullptr)
					{
						return MemoryHandle<T>(Memory::HANDLE_INVALID_MEMORY, nullptr);
					}
					else

					{
						return MemoryHandle<T>(dynAllocator->GetID(), dynAllocator->AllocateArray<T>(aLength));
					}
				}
			}
			return MemoryHandle<T>(Memory::HANDLE_BLOCK_FIXED, allocator->AllocateArray<T>(aLength));
		}

        template<class T>
        T * DeallocateArray(T * aAddress, unsigned int aLength)
        {
            int size = sizeof(T);
            Allocator * allocator = GetAllocator(size);
            if (allocator == nullptr)
            {
                return aAddress;
            }
            return allocator->DeallocateArray<T>(aAddress, aLength);
        } 

		template<class T>
		MemoryHandle<T> DeallocateArrayHandle(MemoryHandle<T> & aHandle, unsigned int aLength)
		{
			int size = sizeof(T);
			if (aHandle.BlockID() == Memory::HANDLE_BLOCK_FIXED)
			{
				Allocator * allocator = GetAllocator(size);
				if (allocator != nullptr)
				{
					return MemoryHandle<T>(Memory::HANDLE_DEALLOCATED, allocator->DeallocateArray<T>(aHandle,aLength));
				}
			}
			else
			{
				DynamicAllocator * allocator = GetDynamicAllocatorByID(aHandle.BlockID());
				if (allocator == nullptr)
				{
					return aHandle;
				}
				else
				{
					return MemoryHandle<T>(Memory::HANDLE_DEALLOCATED, allocator->DeallocateArray<T>(aHandle, aLength));
				}
			}
			return aHandle;
		}

		///Gets the allocator for the given object size.
        Allocator * GetAllocator(int aSize);
		Allocator * GetCollectionsAllocator(int aSize);
		///Gets a dynamic allocator by object size
		DynamicAllocator * GetDynamicAllocator(int aSize);
		///Gets a dynamic allocator by ID
		DynamicAllocator * GetDynamicAllocatorByID(int aID);

		///Flags a Primitive object for Garbage Collection
		void GCFlag(MemoryHandle<Reflection::Primitive> & aHandle);
		template<class T>
		void GCFlag(Collections::Iterator<T> * aGarbage)
		{
			if (aGarbage != nullptr && (GetIteratorFlags(aGarbage) & Reflection::FLAG_GC_COLLECT) != Reflection::FLAG_GC_COLLECT)
			{
				SetIteratorGCFlag(aGarbage);
				m_CollectionsGarbageList->Add(aGarbage);
			}
		}
		///Performs a collection cycle using standard parameters
		void GCCollect();
		///Performs a collection cycle using standard chunk size.
		///@aTime - The amount of time spent collecting
		void GCCollect(float aTime);
		///Performs a collection cycle using the given parameters.
		///@atime - The amount of time spent collecting
		///@aChunkSize - The number of objects to collect before each time check.
		void GCCollect(float aTime, int aChunkSize);

		///Prints Memory report to console. (Don't use this).
		void PrintMemory();
		///Retrieves a report of the current memory usage on all fixed allocators.
		MemoryReport GetMemoryReport();

		int SizeToBlockSize(int aSize);
    private:
#pragma region SINGLETON
        static MemoryManager * s_Instance;
        MemoryManager();
        ~MemoryManager();
#pragma endregion
        //Memory Blocks for each allocator
#pragma region MEMORY BLOCKS
		///Application Memory
        void * m_ApplicationBlock8;         //0
        void * m_ApplicationBlock16;        //1
        void * m_ApplicationBlock32;        //2
        void * m_ApplicationBlock64;        //3
        void * m_ApplicationBlock128;       //4
        void * m_ApplicationBlock256;       //5
        void * m_ApplicationBlock512;       //6
        void * m_ApplicationBlock1024;      //7
        void * m_ApplicationBigBlock;       //8
		///Collections Memory
		void * m_CollectionsBlock8;			//0
		void * m_CollectionsBlock16;		//1
		void * m_CollectionsBlock32;		//2
		void * m_CollectionsBlock64;		//3
		void * m_CollectionsBlock128;		//4
		void * m_CollectionsBlock256;		//5
		void * m_CollectionsBlock512;		//6
		void * m_CollectionsBlock1024;		//7
		//Reserved Memory Block
		void * m_ReservedBlock;  //Reserved memory for the memory manager dynamic allocators
#pragma endregion
		///An allocator for reserved memory.
		Allocator * m_ReservedAllocator;
#pragma region APPLICATION
		//Array of Allocators
        Allocator * m_ApplicationAllocator[(unsigned int)Memory::BlockSize::BLOCK_COUNT];
		///Dynamic allocators for the Application allocations.
		Collections::List<DynamicAllocator*> * m_DynamicApplicationAllocators;
		///A list of garbage handles that need to be freed from the Application
		Collections::List<MemoryHandle<Reflection::Primitive>> * m_ApplicationGarbageList;
		///A helper method to create a dynamic allocator for Application memory heap.
		DynamicAllocator * CreateDynamicAllocator(int aSize);
#pragma endregion

#pragma region COLLECTIONS
		///A array of fixed allocators for Collections
		Allocator * m_CollectionsAllocator[(unsigned int)Memory::BlockSize::BLOCK_COUNT - 1];
		///A list of dynamic allocators for Collections
		Collections::List<DynamicAllocator*> * m_DynamicCollectionsAllocators;
		///List of all of Collections garbage to be cleaned up. Allocated on the fixed memory heap.
		Collections::List<Reflection::Primitive*> * m_CollectionsGarbageList;
		///A helper method to create a dynamic allocator for Collections
		DynamicAllocator * CreateCollectionsAllocator(int aSize);
		///Gets a dynamic allocator for collections by object size
		DynamicAllocator * GetCollectionsDynamicAllocator(int aSize);
		///Gets a dynamic allocator for collections by ID
		DynamicAllocator * GetCollectionsDynamicAllocatorByID(int aID);

		void SetIteratorHandle(Reflection::Primitive * aPrimitive, int aHandle);
		int GetIteratorHandle(Reflection::Primitive * aPrimitive);
		void SetIteratorGCFlag(Reflection::Primitive * aPrimitive);
		int GetIteratorFlags(Reflection::Primitive * aPrimitive);

#pragma endregion
		//Garbage Collection
		///Whether or not to automatically collect with standard times
		bool m_AutoGarbageCollection;
		///ID Generator for Dynamic Allocators. This generates IDs for both application and collections allocators
		UniqueNumberGenerator * m_DynamicAllocatorIDGenerator;
		//Performs updates on garbage collectors
		void Update();
		friend class Application;
    };

    namespace Memory
    {
        //Helpers
        Reflection::Primitive * Instantiate(const char * aTypeName);
        Reflection::Primitive * Instantiate(string & aTypeName);
        Reflection::Primitive * Instantiate(integer & aTypeID);
        Reflection::Primitive * Instantiate(Reflection::Type * aType);
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, const char * aTypeName);
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, string & aTypeName);
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, integer & aTypeID);
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, Reflection::Type * aType);
        template<class T>
        MemoryHandle<T> Instantiate()
        {
            return MemoryManager::Instance()->Allocate<T>();
        }
        template<class T>
		T * Destroy(T * aAddress)
        {
            return MemoryManager::Instance()->Deallocate<T>(aAddress);
        }
		template<class T>
		MemoryHandle<T> DestroyHandle(MemoryHandle<T> & aHandle)
		{
			return MemoryManager::Instance()->DeallocateHandle<T>(aHandle);
		}
        template<class T>
        MemoryHandle<T> InstantiateArray(unsigned int aLength)
        {
            return MemoryManager::Instance()->AllocateArray<T>(aLength);
        }
        template<class T>
        T * DestroyArray(T * aAddress, unsigned int aLength)
        {
            return MemoryManager::Instance()->DeallocateArray<T>(aAddress, aLength);
        }
		template<class T>
		MemoryHandle<T> DestroyArrayHandle(MemoryHandle<T> & aHandle, unsigned int aLength)
		{
			return MemoryManager::Instance()->DeallocateArrayHandle(aHandle, aLength);
		}
		inline void GCFlag(MemoryHandle<Reflection::Primitive> aHandle)
		{
			return MemoryManager::Instance()->GCFlag(aHandle);
		}
    }


}

#endif