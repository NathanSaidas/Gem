#include "G_MemoryManager.h"
#include "../Primitives/G_string.h"
#include "../Primitives/G_integer.h"
#include "../Primitives/G_PrimitiveObjects.h"
#include "../Reflection/G_ReflectionOperators.h"
#include "../Reflection/G_Type.h"
#include "../Utilities/Collections/G_Iterators.h"
#include "../Utilities/Collections/G_List.h"
#include "../Utilities/G_Time.h"
#include "../Utilities/G_UniqueNumberGenerator.h"
#include "../Math/G_Math.h"
#include <malloc.h>

namespace Gem
{
    //determine if the value is between the lower and higher bounds. Inclusive
    inline bool InBounds(const float & aValue, const float & aLower, const float & aHigher)
    {
        if (aValue >= aLower && aValue <= aHigher)
        {
            return true;
        }
        return false;
    }

    inline bool InBounds(const int & aValue, const int & aLower, const int & aHigher)
    {
        if (aValue >= aLower && aValue <= aHigher)
        {
            return true;
        }
        return false;
    }


    MemoryManager * MemoryManager::s_Instance;
    MemoryManager * MemoryManager::Instance()
    {
        if (s_Instance == 0)
        {
            s_Instance = new MemoryManager();
        }
        return s_Instance;
    }
    void MemoryManager::Destroy()
    {
        if (s_Instance != 0)
        {
            delete s_Instance;
            s_Instance = 0;
        }
    }

    MemoryManager::MemoryManager()
    {
        //Allocate Memory
        m_ApplicationBlock8 = malloc(Memory::BLOCK_8_ALLOC_SIZE);
        m_ApplicationBlock16 = malloc(Memory::BLOCK_16_ALLOC_SIZE);
        m_ApplicationBlock32 = malloc(Memory::BLOCK_32_ALLOC_SIZE);
        m_ApplicationBlock64 = malloc(Memory::BLOCK_64_ALLOC_SIZE);
        m_ApplicationBlock128 = malloc(Memory::BLOCK_128_ALLOC_SIZE);
        m_ApplicationBlock256 = malloc(Memory::BLOCK_256_ALLOC_SIZE);
        m_ApplicationBlock512 = malloc(Memory::BLOCK_512_ALLOC_SIZE);
        m_ApplicationBlock1024 = malloc(Memory::BLOCK_1024_ALLOC_SIZE);
        m_ApplicationBigBlock = malloc(Memory::BIG_BLOCK_ALLOC_SIZE);

		m_CollectionsBlock8 = malloc(Memory::COLLECTIONS_BLOCK_8_ALLOC_SIZE);
		m_CollectionsBlock16 = malloc(Memory::COLLECTIONS_BLOCK_16_ALLOC_SIZE);
		m_CollectionsBlock32 = malloc(Memory::COLLECTIONS_BLOCK_32_ALLOC_SIZE);
		m_CollectionsBlock64 = malloc(Memory::COLLECTIONS_BLOCK_64_ALLOC_SIZE);
		m_CollectionsBlock128 = malloc(Memory::COLLECTIONS_BLOCK_128_ALLOC_SIZE);
		m_CollectionsBlock256 = malloc(Memory::COLLECTIONS_BLOCK_256_ALLOC_SIZE);
		m_CollectionsBlock512 = malloc(Memory::COLLECTIONS_BLOCK_512_ALLOC_SIZE);
		m_CollectionsBlock1024 = malloc(Memory::COLLECTIONS_BLOCK_1024_ALLOC_SIZE);

		m_ReservedBlock = malloc(Memory::RESERVED_BLOCK_SIZE);

        m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_8] = new Allocator(8, 8, Memory::BLOCK_8_ALLOC_SIZE, m_ApplicationBlock8);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_16] = new Allocator(16, 8, Memory::BLOCK_16_ALLOC_SIZE, m_ApplicationBlock16);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_32] = new Allocator(32, 8, Memory::BLOCK_32_ALLOC_SIZE, m_ApplicationBlock32);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_64] = new Allocator(64, 8, Memory::BLOCK_64_ALLOC_SIZE, m_ApplicationBlock64);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_128] = new Allocator(128, 8, Memory::BLOCK_128_ALLOC_SIZE, m_ApplicationBlock128);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_256] = new Allocator(256, 8, Memory::BLOCK_256_ALLOC_SIZE, m_ApplicationBlock256);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_512] = new Allocator(512, 8, Memory::BLOCK_512_ALLOC_SIZE, m_ApplicationBlock512);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_1024] = new Allocator(1024, 8, Memory::BLOCK_1024_ALLOC_SIZE, m_ApplicationBlock1024);
		m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_BIG] = new Allocator(Memory::BIG_BLOCK_SIZE, 8, Memory::BIG_BLOCK_ALLOC_SIZE, m_ApplicationBigBlock);


		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_8] = new Allocator(8, 8, Memory::COLLECTIONS_BLOCK_8_ALLOC_SIZE, m_CollectionsBlock8);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_16] = new Allocator(16, 8, Memory::COLLECTIONS_BLOCK_16_ALLOC_SIZE, m_CollectionsBlock16);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_32] = new Allocator(32, 8, Memory::COLLECTIONS_BLOCK_32_ALLOC_SIZE, m_CollectionsBlock32);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_64] = new Allocator(64, 8, Memory::COLLECTIONS_BLOCK_64_ALLOC_SIZE, m_CollectionsBlock64);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_128] = new Allocator(128, 8, Memory::COLLECTIONS_BLOCK_128_ALLOC_SIZE, m_CollectionsBlock128);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_256] = new Allocator(256, 8, Memory::COLLECTIONS_BLOCK_256_ALLOC_SIZE, m_CollectionsBlock256);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_512] = new Allocator(512, 8, Memory::COLLECTIONS_BLOCK_512_ALLOC_SIZE, m_CollectionsBlock512);
		m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_1024] = new Allocator(1024, 8, Memory::COLLECTIONS_BLOCK_1024_ALLOC_SIZE, m_CollectionsBlock1024);

		m_ReservedAllocator = new Allocator(SizeToBlockSize(sizeof(DynamicAllocator)), 8, Memory::RESERVED_BLOCK_SIZE, m_ReservedBlock);
		
		m_AutoGarbageCollection = false;
		
		m_DynamicAllocatorIDGenerator = Allocate<UniqueNumberGenerator>();
		m_DynamicAllocatorIDGenerator->Reset(1);
		m_DynamicApplicationAllocators = Allocate<Collections::List<DynamicAllocator*>>();
		m_DynamicCollectionsAllocators = Allocate<Collections::List<DynamicAllocator*>>();
		m_ApplicationGarbageList = Allocate<Collections::List<MemoryHandle<Reflection::Primitive>>>();
		m_CollectionsGarbageList = Allocate<Collections::List<Reflection::Primitive*>>();

    }
    MemoryManager::~MemoryManager()
    {
		///Do a final collect for 100 seconds or until its finished.
		GCCollect(100.0f, 32);
		///Clean up dynamic allocators
		if (m_DynamicApplicationAllocators->Count() > 0)
		{
			Collections::Iterator<DynamicAllocator*> * enumerator = m_DynamicApplicationAllocators->GetFront();
			while (enumerator != nullptr)
			{
				DynamicAllocator * current = enumerator->Current();
				Collections::Iterator<DynamicAllocator*> * temp = enumerator;
				enumerator = enumerator->GetNext();
				if (current != nullptr)
				{
					if (current->Allocations() > 0)
					{
						std::cout << "Leaking Dynamic Application Objects...\n";
					}
					m_DynamicApplicationAllocators->Remove(current);
					m_ReservedAllocator->Deallocate<DynamicAllocator>(current);
				}
			}
		}

		if (m_DynamicCollectionsAllocators->Count() > 0)
		{
			Collections::Iterator<DynamicAllocator*> * enumerator = m_DynamicCollectionsAllocators->GetFront();
			while (enumerator != nullptr)
			{
				DynamicAllocator * current = enumerator->Current();
				Collections::Iterator<DynamicAllocator*> * temp = enumerator;
				enumerator = enumerator->GetNext();
				if (current != nullptr)
				{
					if (current->Allocations() > 0)
					{
						std::cout << "Leaking Dynamic Collection Objects...\n";
					}
					m_DynamicCollectionsAllocators->Remove(current);
					m_ReservedAllocator->Deallocate<DynamicAllocator>(current);
				}
			}
		}

		std::cout << "Items on application garbage list " << m_ApplicationGarbageList->Count() << std::endl;
		std::cout << "Items on the collections garbage list" << m_CollectionsGarbageList->Count() << std::endl;

		m_ApplicationGarbageList = Deallocate<Collections::List<MemoryHandle<Reflection::Primitive>>>(m_ApplicationGarbageList);
		m_CollectionsGarbageList = Deallocate<Collections::List<Reflection::Primitive*>>(m_CollectionsGarbageList);
		m_DynamicCollectionsAllocators = Deallocate<Collections::List<DynamicAllocator*>>(m_DynamicCollectionsAllocators);
		m_DynamicApplicationAllocators = Deallocate<Collections::List<DynamicAllocator*>>(m_DynamicApplicationAllocators);
		m_DynamicAllocatorIDGenerator = Deallocate<UniqueNumberGenerator>(m_DynamicAllocatorIDGenerator);

		for (int i = 0; i < (int)Memory::BlockSize::BLOCK_COUNT; i++)
        {
            if (m_ApplicationAllocator[i] != nullptr)
            {
				if (m_ApplicationAllocator[i]->Allocations() > 0)
				{
					std::cout << "Leaking Application Fixed Allocator Objects [" << i << "]\n";
				}
                delete m_ApplicationAllocator[i];
                m_ApplicationAllocator[i] = nullptr;
            }
        }
		for (int i = 0; i < (int)Memory::BlockSize::BLOCK_COUNT - 1; i++)
		{
			if (m_CollectionsAllocator[i] != nullptr)
			{
				if (m_CollectionsAllocator[i]->Allocations() > 0)
				{
					std::cout << "Leaking Collection Fixed Allocator Objects [" << i << "]\n";
					std::cout << "Objects Leaked: " << m_CollectionsAllocator[i]->Allocations() << "\n";
				}
				delete m_CollectionsAllocator[i];
				m_CollectionsAllocator[i] = nullptr;
			}
		}

		delete m_ReservedAllocator;
		m_ReservedAllocator = 0;

		free(m_ApplicationBlock8);
		free(m_ApplicationBlock16);
		free(m_ApplicationBlock32);
		free(m_ApplicationBlock64);
		free(m_ApplicationBlock128);
		free(m_ApplicationBlock256);
		free(m_ApplicationBlock512);
		free(m_ApplicationBlock1024);
		free(m_ApplicationBigBlock);

		free(m_CollectionsBlock8);
		free(m_CollectionsBlock16);
		free(m_CollectionsBlock32);
		free(m_CollectionsBlock64);
		free(m_CollectionsBlock128);
		free(m_CollectionsBlock256);
		free(m_CollectionsBlock512);
		free(m_CollectionsBlock1024);

		free(m_ReservedBlock);
    }
    Reflection::Primitive * MemoryManager::Allocate(string & aTypeName)
    {
        Pointer<Reflection::Type> type = typeOf(aTypeName);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return Allocate(type.ref());
    }
    Reflection::Primitive * MemoryManager::Allocate(integer & aTypeID)
    {
        Pointer<Reflection::Type> type = typeOf(aTypeID);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return Allocate(type.ref());
    }
    Reflection::Primitive * MemoryManager::Allocate(Reflection::Type * aType)
    {
        if (aType == nullptr)
        {
            return nullptr;
        }

        Allocator * allocator = GetAllocator(aType->size());
        if (allocator == nullptr)
        {
            return nullptr;
        }
        return allocator->Allocate(aType);
    }
    Reflection::Primitive * MemoryManager::Deallocate(Reflection::Primitive * aPtr, string & aTypeName)
    {
        Pointer<Reflection::Type> type = typeOf(aTypeName);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return Deallocate(aPtr,type.ref());
    }
    Reflection::Primitive * MemoryManager::Deallocate(Reflection::Primitive * aPtr, integer & aTypeID)
    {
        Pointer<Reflection::Type> type = typeOf(aTypeID);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return Deallocate(aPtr,type.ref());
    }
    Reflection::Primitive * MemoryManager::Deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType)
    {
        if (aType == nullptr)
        {
            return aPtr;
        }
        Allocator * allocator = GetAllocator(aType->size());
        if (allocator == nullptr)
        {
            return aPtr;
        }
        return allocator->Deallocate(aPtr);
    }

    Allocator * MemoryManager::GetAllocator(int aSize)
    {
        if (InBounds(aSize, 1, 8))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_8];
        }
		else if (InBounds(aSize, 9, 16))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_16];
        }
		else if (InBounds(aSize, 17, 32))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_32];
        }
		else if (InBounds(aSize, 33, 64))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_64];
        }
		else if (InBounds(aSize, 65, 128))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_128];
        }
		else if (InBounds(aSize, 129, 256))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_256];
        }
		else if (InBounds(aSize, 257, 512))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_512];
        }
		else if (InBounds(aSize, 513, 1024))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_1024];
        }
		else if (InBounds(aSize, 1025, Memory::BIG_BLOCK_SIZE))
        {
            return m_ApplicationAllocator[(int)Memory::BlockSize::BLOCK_BIG];
        }
        return 0;
	}
	Allocator * MemoryManager::GetCollectionsAllocator(int aSize)
	{
		if (InBounds(aSize, 1, 8))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_8];
		}
		else if (InBounds(aSize, 9, 16))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_16];
		}
		else if (InBounds(aSize, 17, 32))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_32];
		}
		else if (InBounds(aSize, 33, 64))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_64];
		}
		else if (InBounds(aSize, 65, 128))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_128];
		}
		else if (InBounds(aSize, 129, 256))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_256];
		}
		else if (InBounds(aSize, 257, 512))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_512];
		}
		else if (InBounds(aSize, 513, 1024))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_1024];
		}
		else if (InBounds(aSize, 1025, Memory::BIG_BLOCK_SIZE))
		{
			return m_CollectionsAllocator[(int)Memory::BlockSize::BLOCK_BIG];
		}
		return 0;
	}
	DynamicAllocator * MemoryManager::GetDynamicAllocator(int aSize)
	{
		aSize = SizeToBlockSize(aSize);
		if (m_DynamicApplicationAllocators == nullptr || m_DynamicApplicationAllocators->Count() == 0)
		{
			return nullptr;
		}
		Collections::Iterator<DynamicAllocator*> * iter = m_DynamicApplicationAllocators->GetFront();
		while (iter != nullptr)
		{
			DynamicAllocator * current = iter->Current();
			if (current == nullptr || current->ObjectSize() != aSize || current->OutOfMemory())
			{
				iter = iter->GetNext();
				continue;
			}
			return current;
		}
		return nullptr;
	}
	DynamicAllocator * MemoryManager::GetDynamicAllocatorByID(int aID)
	{
		if (aID <= 0)
		{
			return nullptr;
		}
		Collections::Iterator<DynamicAllocator*> * iter = m_DynamicApplicationAllocators->GetFront();
		while (iter != nullptr)
		{
			DynamicAllocator * current = iter->Current();
			if (current->GetID() == aID)
			{
				return current;
			}
			iter = iter->GetNext();
		}
		return nullptr;
	}
	void MemoryManager::GCFlag(MemoryHandle<Reflection::Primitive> & aHandle)
	{
		Reflection::Primitive * garbage = aHandle.Memory();
		if (garbage == nullptr)
		{
			return;
		}
		if (garbage->IsFlaggedForGC())
		{
			return;
		}
		garbage->m_Flags |= Reflection::FLAG_GC_COLLECT;
		m_ApplicationGarbageList->Add(aHandle);
	}
	void MemoryManager::GCCollect()
	{
		float startTime = Time::GetTime();
		float deltaTime = 0.0f;

		while (deltaTime < Memory::GC_STANDARD_COLLECTION_TIME && m_ApplicationGarbageList->Count() > 0)
		{
			int count = 0;
			while (count < Memory::GC_STANDARD_CHUNK_SIZE && m_ApplicationGarbageList->Count() > 0)
			{
				m_ApplicationGarbageList->RemoveAt(0);
				count++;
			}
			deltaTime = Time::GetTime() - startTime;
		}
		startTime = Time::GetTime();
		deltaTime = 0.0f;

		while (deltaTime < Memory::GC_STANDARD_COLLECTION_TIME && m_CollectionsGarbageList->Count() > 0)
		{
			int count = 0;
			while (count < Memory::GC_STANDARD_CHUNK_SIZE && m_CollectionsGarbageList->Count() > 0)
			{
				m_CollectionsGarbageList->RemoveAt(0);
				count++;
			}
			deltaTime = Time::GetTime() - startTime;
		}
	}
	void MemoryManager::GCCollect(float aTime)
	{
		float startTime = Time::GetTime();
		float deltaTime = 0.0f;

		while (deltaTime < aTime && m_ApplicationGarbageList->Count() > 0)
		{
			int count = 0;
			while (count < Memory::GC_STANDARD_CHUNK_SIZE && m_ApplicationGarbageList->Count() > 0)
			{
				m_ApplicationGarbageList->RemoveAt(0);
				count++;
			}
			deltaTime = Time::GetTime() - startTime;
		}

		startTime = Time::GetTime();
		deltaTime = 0.0f;

		while (deltaTime < aTime && m_CollectionsGarbageList->Count() > 0)
		{
			int count = 0;
			while (count < Memory::GC_STANDARD_CHUNK_SIZE && m_CollectionsGarbageList->Count() > 0)
			{
				m_CollectionsGarbageList->RemoveAt(0);
				count++;
			}
			deltaTime = Time::GetTime() - startTime;
		}

	}

	void MemoryManager::GCCollect(float aTime, int aChunkSize)
	{
		float startTime = Time::GetTime();
		float deltaTime = 0.0f;

		while (deltaTime < aTime && m_ApplicationGarbageList->Count() > 0)
		{
			int count = 0;
			while (count < aChunkSize && m_ApplicationGarbageList->Count() > 0)
			{
				//TODO: Figure out a way to delete iterators contents..
				m_ApplicationGarbageList->RemoveAt(0);
				count++;
			}
			deltaTime = Time::GetTime() - startTime;
		}

		startTime = Time::GetTime();
		deltaTime = 0.0f;

		while (deltaTime < aTime && m_CollectionsGarbageList->Count() > 0)
		{
			int count = 0;
			while (count < aChunkSize && m_CollectionsGarbageList->Count() > 0)
			{
				//TODO: Figure out a way to delete iterators contents.
				m_CollectionsGarbageList->RemoveAt(0);
				count++;
			}
			deltaTime = Time::GetTime() - startTime;
		}
	}

	void MemoryManager::PrintMemory()
	{
		for (int i = 0; i < 9; i++)
		{
			int usedMemory = m_ApplicationAllocator[i]->UsedMemory();
			int allocations = m_ApplicationAllocator[i]->Allocations();
			int objectSize = m_ApplicationAllocator[i]->ObjectSize();
			int totalObjects = m_ApplicationAllocator[i]->NumberOfObjects();

			std::cout << "Allocator: " << objectSize << "\n";
			std::cout << "Used Memory: " << usedMemory << "\n";
			std::cout << "Allocations: " << allocations << "\n";
			std::cout << "Total Objects: " << totalObjects << "\n";
		}


	}

	MemoryReport MemoryManager::GetMemoryReport()
	{
		MemoryReport report;
		m_ApplicationAllocator[0]->GetInfo(report.block8);
		m_ApplicationAllocator[1]->GetInfo(report.block16);
		m_ApplicationAllocator[2]->GetInfo(report.block32);
		m_ApplicationAllocator[3]->GetInfo(report.block64);
		m_ApplicationAllocator[4]->GetInfo(report.block128);
		m_ApplicationAllocator[5]->GetInfo(report.block256);
		m_ApplicationAllocator[6]->GetInfo(report.block512);
		m_ApplicationAllocator[7]->GetInfo(report.block1024);
		m_ApplicationAllocator[8]->GetInfo(report.bigBlock);
		return report;
	}
	int MemoryManager::SizeToBlockSize(int aSize)
	{
		if (InBounds(aSize, 1, 8))
		{
			return 8;
		}
		else if (InBounds(aSize, 9, 16))
		{
			return 16;
		}
		else if (InBounds(aSize, 17, 32))
		{
			return 32;
		}
		else if (InBounds(aSize, 33, 64))
		{
			return 64;
		}
		else if (InBounds(aSize, 65, 128))
		{
			return 128;
		}
		else if (InBounds(aSize, 129, 256))
		{
			return 256;
		}
		else if (InBounds(aSize, 257, 512))
		{
			return 512;
		}
		else if (InBounds(aSize, 513, 1024))
		{
			return 1024;
		}
		else if (InBounds(aSize, 1025, Memory::BIG_BLOCK_SIZE))
		{
			return Memory::BIG_BLOCK_SIZE;
		}
		return 0;
	}
	DynamicAllocator * MemoryManager::CreateDynamicAllocator(int aSize)
	{
		int blockSize = SizeToBlockSize(aSize);
		if (blockSize == 0)
		{
			return nullptr;
		}
		int allocSize = -1;
		switch (blockSize)
		{
		case 8:
			allocSize = Memory::DYN_BLOCK_8_ALLOC_SIZE;
			break;
		case 16:
			allocSize = Memory::DYN_BLOCK_16_ALLOC_SIZE;
			break;
		case 32:
			allocSize = Memory::DYN_BLOCK_32_ALLOC_SIZE;
			break;
		case 64:
			allocSize = Memory::DYN_BLOCK_64_ALLOC_SIZE;
			break;
		case 128:
			allocSize = Memory::DYN_BLOCK_128_ALLOC_SIZE;
			break;
		case 256:
			allocSize = Memory::DYN_BLOCK_256_ALLOC_SIZE;
			break;
		case 512:
			allocSize = Memory::DYN_BLOCK_512_ALLOC_SIZE;
			break;
		case 1024:
			allocSize = Memory::DYN_BLOCK_1024_ALLOC_SIZE;
			break;
		default:
			allocSize = -1;
			break;
		}
		if (allocSize == -1)
		{
			return nullptr;
		}
		DynamicAllocator * allocator = m_ReservedAllocator->Allocate<DynamicAllocator>();
		allocator->Init(m_DynamicAllocatorIDGenerator->Get(), blockSize, 8, allocSize);

		m_DynamicApplicationAllocators->Add(allocator);
		return allocator;
	}
	DynamicAllocator * MemoryManager::CreateCollectionsAllocator(int aSize)
	{
		int blockSize = SizeToBlockSize(aSize);
		if (blockSize == 0)
		{
			return nullptr;
		}
		int allocSize = -1;
		switch (blockSize)
		{
		case 8:
			allocSize = Memory::DYN_BLOCK_8_ALLOC_SIZE;
			break;
		case 16:
			allocSize = Memory::DYN_BLOCK_16_ALLOC_SIZE;
			break;
		case 32:
			allocSize = Memory::DYN_BLOCK_32_ALLOC_SIZE;
			break;
		case 64:
			allocSize = Memory::DYN_BLOCK_64_ALLOC_SIZE;
			break;
		case 128:
			allocSize = Memory::DYN_BLOCK_128_ALLOC_SIZE;
			break;
		case 256:
			allocSize = Memory::DYN_BLOCK_256_ALLOC_SIZE;
			break;
		case 512:
			allocSize = Memory::DYN_BLOCK_512_ALLOC_SIZE;
			break;
		case 1024:
			allocSize = Memory::DYN_BLOCK_1024_ALLOC_SIZE;
			break;
		default:
			allocSize = -1;
			break;
		}
		if (allocSize == -1)
		{
			return nullptr;
		}
		DynamicAllocator * allocator = m_ReservedAllocator->Allocate<DynamicAllocator>();
		allocator->Init(m_DynamicAllocatorIDGenerator->Get(), blockSize, 8, allocSize);

		m_DynamicCollectionsAllocators->Add(allocator);
		return allocator;
	}
	///Gets a dynamic allocator for collections by object size
	DynamicAllocator * MemoryManager::GetCollectionsDynamicAllocator(int aSize)
	{
		aSize = SizeToBlockSize(aSize);
		if (m_DynamicCollectionsAllocators == nullptr || m_DynamicCollectionsAllocators->Count() == 0)
		{
			return nullptr;
		}
		Collections::Iterator<DynamicAllocator*> * iter = m_DynamicCollectionsAllocators->GetFront();
		while (iter != nullptr)
		{
			DynamicAllocator * current = iter->Current();
			if (current == nullptr || current->ObjectSize() != aSize || current->OutOfMemory())
			{
				iter = iter->GetNext();
				continue;
			}
			return current;
		}
		return nullptr;
	}
	///Gets a dynamic allocator for collections by ID
	DynamicAllocator * MemoryManager::GetCollectionsDynamicAllocatorByID(int aID)
	{
		if (aID <= 0)
		{
			return nullptr;
		}
		Collections::Iterator<DynamicAllocator*> * iter = m_DynamicCollectionsAllocators->GetFront();
		while (iter != nullptr)
		{
			DynamicAllocator * current = iter->Current();
			if (current->GetID() == aID)
			{
				return current;
			}
		}
		return nullptr;
	}

	void MemoryManager::SetIteratorHandle(Reflection::Primitive * aPrimitive, int aHandle)
	{
		if (aPrimitive != nullptr)
		{
			aPrimitive->SetHandle(aHandle);
		}
	}
	int MemoryManager::GetIteratorHandle(Reflection::Primitive * aPrimitive)
	{
		if (aPrimitive != nullptr)
		{
			return aPrimitive->GetHandle();
		}
		return Memory::HANDLE_INVALID_MEMORY;
	}
	void MemoryManager::SetIteratorGCFlag(Reflection::Primitive * aPrimitive)
	{
		if (aPrimitive != nullptr)
		{
			aPrimitive->m_Flags |= Reflection::FLAG_GC_COLLECT;
		}
	}
	int MemoryManager::GetIteratorFlags(Reflection::Primitive * aPrimitive)
	{
		if (aPrimitive != nullptr)
		{
			return aPrimitive->m_Flags;
		}
		return Reflection::FLAG_INVALID;
	}
	void MemoryManager::Update()
	{
		if (m_AutoGarbageCollection)
		{
			GCCollect();
		}
		Collections::Iterator<DynamicAllocator*> * enumerator = m_DynamicApplicationAllocators->GetFront();
		
		while (enumerator != nullptr)
		{
			Collections::Iterator<DynamicAllocator*> * temp = enumerator;
			DynamicAllocator * current = temp->Current();
			enumerator = enumerator->GetNext();

			if (current != nullptr && current->Allocations() == 0)
			{
				m_DynamicAllocatorIDGenerator->Free(current->GetID());
				m_DynamicApplicationAllocators->Remove(current);
				m_ReservedAllocator->Deallocate<DynamicAllocator>(current);
			}
		}
		enumerator = m_DynamicCollectionsAllocators->GetFront();
		while (enumerator != nullptr)
		{
			Collections::Iterator<DynamicAllocator*> * temp = enumerator;
			DynamicAllocator * current = temp->Current();
			enumerator = enumerator->GetNext();

			if (current != nullptr && current->Allocations() == 0)
			{
				m_DynamicAllocatorIDGenerator->Free(current->GetID());
				m_DynamicCollectionsAllocators->Remove(current);
				m_ReservedAllocator->Deallocate<DynamicAllocator>(current);
			}
		}

	}

    namespace Memory
    {
        Reflection::Primitive * instantiate(const char * aTypeName)
        {
            return MemoryManager::Instance()->Allocate(string(aTypeName));
        }
        Reflection::Primitive * instantiate(string & aTypeName)
        {
			return MemoryManager::Instance()->Allocate(aTypeName);
        }
        Reflection::Primitive * instantiate(integer & aTypeID)
        {
			return MemoryManager::Instance()->Allocate(aTypeID);
        }
        Reflection::Primitive * instantiate(Reflection::Type * aType)
        {
			return MemoryManager::Instance()->Allocate(aType);
        }
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, const char * aTypeName)
        {
            return MemoryManager::Instance()->Deallocate(aAddress, string(aTypeName));
        }
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, string & aTypeName)
        {
			return MemoryManager::Instance()->Deallocate(aAddress, aTypeName);
        }
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, integer & aTypeID)
        {
			return MemoryManager::Instance()->Deallocate(aAddress, aTypeID);
        }
        Reflection::Primitive * Destroy(Reflection::Primitive * aAddress, Reflection::Type * aType)
        {
			return MemoryManager::Instance()->Deallocate(aAddress, aType);
        }
    }
}