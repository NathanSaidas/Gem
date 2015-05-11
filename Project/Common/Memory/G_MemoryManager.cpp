#include "G_MemoryManager.h"
#include "G_MemoryUtils.h"
#include "../Utilities/G_Time.h"
#ifdef GEM_REFLECTION
#include "../Reflection/G_Reflection.h"
#endif

namespace Gem
{
	namespace Memory
	{

		namespace Hidden
		{
			MemoryManager * MemoryManager::s_Instance = nullptr;
			MemoryManager * MemoryManager::Instance()
			{
				if (s_Instance == nullptr)
				{
					s_Instance = new MemoryManager();
				}
				return s_Instance;
			}
			void MemoryManager::Destroy()
			{
				if (s_Instance != nullptr)
				{
					delete s_Instance;
					s_Instance = nullptr;
				}
			}
			MemoryManager::MemoryManager()
			{
				m_DynamicID = 1;
				for (int i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
				{
					m_Allocators[i] = new FixedAllocator();
					m_Allocators[i]->Initalize(Memory::OBJECT_SIZES[i], Memory::MEMORY_ALIGNMENT, Memory::BLOCK_ALLOC_SIZES[i]);
					m_Allocators[i]->ID(0);
				}
			}
			MemoryManager::~MemoryManager()
			{
				printf("Memory Manager Cleaning Up\n");
				printf("=======Memory Leaks=======\n");

				for (int i = m_DynamicAllocators.size() - 1; i >= 0; i--)
				{
					PrintLeaks(m_DynamicAllocators[i], i);
					m_DynamicAllocators[i]->Dispose();
					delete m_DynamicAllocators[i];
				}

				for (int i = Memory::MEMORY_ALLOCATOR_COUNT - 1; i >= 0; i--)
				{
					PrintLeaks(m_Allocators[i], i);
					m_Allocators[i]->Dispose();
					delete m_Allocators[i];
				}
				system("pause");
				m_DynamicAllocators.clear();
			}

			void * MemoryManager::Allocate(unsigned int aSize)
			{
				//Get an allocator and allocate the object.
				FixedAllocator * allocator = GetAllocator(aSize + Memory::MEMORY_HEADER_SIZE);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(allocator != nullptr && "Failed to retrieve an allocator to allocate with.");
				assert(allocator->Initialized() && "Failed to retrieve an allocator to allocate with.");
#endif
				if (allocator == nullptr || !allocator->Initialized())
				{
					return nullptr;
				}
				void * allocatedObj = allocator->Allocate();

				///Write to memory header
				void * memoryHeader = (void*)((uintptr_t)allocatedObj - Memory::MEMORY_HEADER_SIZE);
				int headerMask = WriteHeaderMask(Memory::FLAG_VALID, allocator->ID());
				memcpy(memoryHeader, &headerMask, sizeof(int));

				return allocatedObj;
			}

			void * MemoryManager::Deallocate(void * aMemory, unsigned int aSize)
			{
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(aMemory != nullptr, "Attempting to deallocate invalid memory");
#endif
				if (aMemory == nullptr)
				{
					return nullptr;
				}
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
				FixedAllocator  * allocator = GetAllocatorByID(aSize + Memory::MEMORY_HEADER_SIZE, allocatorID);

#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(allocator != nullptr && "Failed to retrieve an allocator to deallocate with.");
				assert(allocator->Initialized() && "Failed to retrieve an allocator to deallocate with.");
#endif
				if (allocator == nullptr)
				{
					return aMemory;
				}
				///Do not need to check if the allocator is valid, it checks internally
				void * result = allocator->Deallocate((void*)((uintptr_t)aMemory));

				///If the allocator should expire.
				if (allocator->ID() != 0 && allocator->Allocations() == 0)
				{
					ExpireAllocator(allocator);
				}

				return result;
			}

#ifdef GEM_REFLECTION
			void * MemoryManager::Allocate(char * aTypename)
			{
				Type type = Reflection::GetType(aTypename);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(type) || type.Size() < 4);
#endif
				if (!Reflection::IsBadType(type) && type.Size() >= 4)
				{
					void * obj = Allocate(type.Size());
					return obj == nullptr ? nullptr : type.InvokeConstructor(obj);
				}
				return nullptr;
			}
			void * MemoryManager::Allocate(const char * aTypename)
			{
				Type type = Reflection::GetType(aTypename);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(type) || type.Size() < 4);
#endif
				if (!Reflection::IsBadType(type) && type.Size() >= 4)
				{
					void * obj = Allocate(type.Size());
					return obj == nullptr ? nullptr : type.InvokeConstructor(obj);
				}
				return nullptr;
			}
			void * MemoryManager::Allocate(std::string aTypename)
			{
				Type type = Reflection::GetType(aTypename);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(type) || type.Size() < 4);
#endif
				if (!Reflection::IsBadType(type) && type.Size() >= 4)
				{
					void * obj = Allocate(type.Size());
					return obj == nullptr ? nullptr : type.InvokeConstructor(obj);
				}
				return nullptr;
			}
			void * MemoryManager::Allocate(Type & aType)
			{
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(aType) || aType.Size() < 4);
#endif
				if (!Reflection::IsBadType(aType) && aType.Size() >= 4)
				{
					void * obj = Allocate(aType.Size());
					return obj == nullptr ? nullptr : aType.InvokeConstructor(obj);
				}
				return nullptr;
			}

			void * MemoryManager::Deallocate(void * aMemory, char * aTypename)
			{
				Type type = Reflection::GetType(aTypename);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(type) || type.Size() < 4);
#endif
				if (!Reflection::IsBadType(type) && type.Size() >= 4)
				{
					if (aMemory != nullptr)
					{
						type.InvokeDestructor(aMemory);
					}
					return Deallocate(aMemory, type.Size());
				}
				return aMemory;
			}
			void * MemoryManager::Deallocate(void * aMemory, const char * aTypename)
			{
				Type type = Reflection::GetType(aTypename);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(type) || type.Size() < 4);
#endif
				if (!Reflection::IsBadType(type) && type.Size() >= 4)
				{
					if (aMemory != nullptr)
					{
						type.InvokeDestructor(aMemory);
					}
					return Deallocate(aMemory, type.Size());
				}
				return aMemory;
			}
			void * MemoryManager::Deallocate(void * aMemory, std::string aTypename)
			{
				Type type = Reflection::GetType(aTypename);
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(type) || type.Size() < 4);
#endif
				if (!Reflection::IsBadType(type) && type.Size() >= 4)
				{
					if (aMemory != nullptr)
					{
						type.InvokeDestructor(aMemory);
					}
					return Deallocate(aMemory, type.Size());
				}
				return aMemory;
			}
			void * MemoryManager::Deallocate(void * aMemory, Type & aType)
			{
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(!Reflection::IsBadType(aType) || aType.Size() < 4);
#endif
				if (!Reflection::IsBadType(aType) && aType.Size() >= 4)
				{
					if (aMemory != nullptr)
					{
						aType.InvokeDestructor(aMemory);
					}
					return Deallocate(aMemory, aType.Size());
				}
				return aMemory;
			}
#endif



			MemoryReport MemoryManager::GetMemoryReport()
			{
				Memory::MemoryReport report;
				memset(&report, 0, sizeof(Memory::MemoryReport));

				for (int i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
				{
					Memory::AllocatorMemInfo info = m_Allocators[i]->GetMemoryInfo();
					report.info[i].allocations += info.allocations;
					report.info[i].maxAllocations += info.maxAllocations;
					report.memoryUsed += info.allocations * info.objectSize;
					report.memoryAvailable += info.maxAllocations * info.objectSize;
				}

				for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
				{
					Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
					int index = Memory::SizeToBlockIndex(info.objectSize);
					report.info[index].allocations += info.allocations;
					report.info[index].maxAllocations += info.maxAllocations;
					report.memoryUsed += info.allocations * info.objectSize;
					report.memoryAvailable += info.maxAllocations * info.objectSize;
				}

				return report;
			}
			MemoryReport MemoryManager::GetMemoryReport(Memory::MemoryReportType aType)
			{
				Memory::MemoryReport report;
				memset(&report, 0, sizeof(Memory::MemoryReport));

				switch (aType)
				{
				case Memory::MemoryReportType::All:
				{
													  for (int i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
													  {
														  Memory::AllocatorMemInfo info = m_Allocators[i]->GetMemoryInfo();
														  report.info[i].allocations += info.allocations;
														  report.info[i].maxAllocations += info.maxAllocations;
														  report.memoryUsed += info.allocations * info.objectSize;
														  report.memoryAvailable += info.maxAllocations * info.objectSize;
													  }

													  for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
													  {
														  Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
														  int index = Memory::SizeToBlockIndex(info.objectSize);
														  report.info[index].allocations += info.allocations;
														  report.info[index].maxAllocations += info.maxAllocations;
														  report.memoryUsed += info.allocations * info.objectSize;
														  report.memoryAvailable += info.maxAllocations * info.objectSize;
													  }
				}
					break;
				case MemoryReportType::Initial:
				{
														  for (int i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
														  {
															  Memory::AllocatorMemInfo info = m_Allocators[i]->GetMemoryInfo();
															  report.info[i].allocations += info.allocations;
															  report.info[i].maxAllocations += info.maxAllocations;
															  report.memoryUsed += info.allocations * info.objectSize;
															  report.memoryAvailable += info.maxAllocations * info.objectSize;
														  }
				}
					break;
				case MemoryReportType::Dynamic:
				{
														  for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
														  {
															  Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
															  int index = Memory::SizeToBlockIndex(info.objectSize);
															  report.info[index].allocations += info.allocations;
															  report.info[index].maxAllocations += info.maxAllocations;
															  report.memoryUsed += info.allocations * info.objectSize;
															  report.memoryAvailable += info.maxAllocations * info.objectSize;
														  }
				}
					break;
				default:
				{
						   for (int i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
						   {
							   Memory::AllocatorMemInfo info = m_Allocators[i]->GetMemoryInfo();
							   report.info[i].allocations += info.allocations;
							   report.info[i].maxAllocations += info.maxAllocations;
							   report.memoryUsed += info.allocations * info.objectSize;
							   report.memoryAvailable += info.maxAllocations * info.objectSize;
						   }
						   for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
						   {
							   Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
							   int index = Memory::SizeToBlockIndex(info.objectSize);
							   report.info[index].allocations += info.allocations;
							   report.info[index].maxAllocations += info.maxAllocations;
							   report.memoryUsed += info.allocations * info.objectSize;
							   report.memoryAvailable += info.maxAllocations * info.objectSize;
						   }
				}
					break;
				}
				return report;
			}
			MemoryReport MemoryManager::GetMemoryReport(Memory::MemoryBlock aBlock)
			{
				Memory::MemoryReport report;
				memset(&report, 0, sizeof(Memory::MemoryReport));

				{
					Memory::AllocatorMemInfo info = m_Allocators[(int)aBlock]->GetMemoryInfo();
					report.info[(int)aBlock].allocations += info.allocations;
					report.info[(int)aBlock].maxAllocations += info.maxAllocations;
					report.memoryUsed += info.allocations * info.objectSize;
					report.memoryAvailable += info.maxAllocations * info.objectSize;
				}

				for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
				{
					Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
					if (aBlock == Memory::SizeToBlock(info.objectSize))
					{
						int index = Memory::SizeToBlockIndex(info.objectSize);
						report.info[index].allocations += info.allocations;
						report.info[index].maxAllocations += info.maxAllocations;
						report.memoryUsed += info.allocations * info.objectSize;
						report.memoryAvailable += info.maxAllocations * info.objectSize;
					}

				}
				return report;
			}
			MemoryReport MemoryManager::GetMemoryReport(Memory::MemoryBlock aBlock, Memory::MemoryReportType aType)
			{
				Memory::MemoryReport report;
				memset(&report, 0, sizeof(Memory::MemoryReport));


				switch (aType)
				{
				case Memory::MemoryReportType::All:
				{
					{
						Memory::AllocatorMemInfo info = m_Allocators[(int)aBlock]->GetMemoryInfo();
						report.info[(int)aBlock].allocations += info.allocations;
						report.info[(int)aBlock].maxAllocations += info.maxAllocations;
						report.memoryUsed += info.allocations * info.objectSize;
						report.memoryAvailable += info.maxAllocations * info.objectSize;
					}

													  for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
													  {
														  Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
														  if (aBlock == Memory::SizeToBlock(info.objectSize))
														  {
															  int index = Memory::SizeToBlockIndex(info.objectSize);
															  report.info[index].allocations += info.allocations;
															  report.info[index].maxAllocations += info.maxAllocations;
															  report.memoryUsed += info.allocations * info.objectSize;
															  report.memoryAvailable += info.maxAllocations * info.objectSize;
														  }
													  }
				}
					break;
				case MemoryReportType::Initial:
				{
					{
						Memory::AllocatorMemInfo info = m_Allocators[(int)aBlock]->GetMemoryInfo();
						report.info[(int)aBlock].allocations += info.allocations;
						report.info[(int)aBlock].maxAllocations += info.maxAllocations;
						report.memoryUsed += info.allocations * info.objectSize;
						report.memoryAvailable += info.maxAllocations * info.objectSize;
					}
				}
					break;
				case MemoryReportType::Dynamic:
				{
														  for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
														  {
															  Memory::AllocatorMemInfo info = (*it)->GetMemoryInfo();
															  if (aBlock == Memory::SizeToBlock(info.objectSize))
															  {
																  int index = Memory::SizeToBlockIndex(info.objectSize);
																  report.info[index].allocations += info.allocations;
																  report.info[index].maxAllocations += info.maxAllocations;
																  report.memoryUsed += info.allocations * info.objectSize;
																  report.memoryAvailable += info.maxAllocations * info.objectSize;
															  }
														  }
				}
					break;
				default:
				{
					{
						AllocatorMemInfo info = m_Allocators[(int)aBlock]->GetMemoryInfo();
						report.info[(int)aBlock].allocations += info.allocations;
						report.info[(int)aBlock].maxAllocations += info.maxAllocations;
						report.memoryUsed += info.allocations * info.objectSize;
						report.memoryAvailable += info.maxAllocations * info.objectSize;
					}

						   for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
						   {
							   AllocatorMemInfo info = (*it)->GetMemoryInfo();
							   if (aBlock == Memory::SizeToBlock(info.objectSize))
							   {
								   int index = Memory::SizeToBlockIndex(info.objectSize);
								   report.info[index].allocations += info.allocations;
								   report.info[index].maxAllocations += info.maxAllocations;
								   report.memoryUsed += info.allocations * info.objectSize;
								   report.memoryAvailable += info.maxAllocations * info.objectSize;
							   }
						   }
				}
					break;
				}


				return report;
			}

			void MemoryManager::ArrayAlloc(void * aAddress, unsigned int aLength)
			{
				uintptr_t address = (uintptr_t)aAddress;
				Memory::ArrayAlloc data;
				data.address = aAddress;
				data.size = aLength;
				m_ArrayAllocations.insert(std::pair<uintptr_t, Memory::ArrayAlloc>(address, data));
			}
			void MemoryManager::ArrayDealloc(void * aAddress, unsigned int aLength)
			{
				uintptr_t address = (uintptr_t)aAddress;
				std::map<uintptr_t, Memory::ArrayAlloc>::iterator it = m_ArrayAllocations.find(address);
				m_ArrayAllocations.erase(it);
			}

			void MemoryManager::FlagGC(void * aAddress, Type & aType)
			{
#ifdef GEM_REFLECTION
				unsigned int flags;
				int allocatorID = 0;
				ReadHeaderMask(aAddress, flags, allocatorID);
				if (flags == FLAG_GC)
				{
					return;
				}
				flags = FLAG_GC;
				WriteHeaderMask(aAddress, flags, allocatorID);
				GCItem item(aAddress, aType);
				m_GarbageList.push_back(item);
#endif
			}

			bool MemoryManager::IsFlaggedForGC(void * aMemory)
			{ 
				unsigned int flags;
				int allocatorID = 0;
				ReadHeaderMask(aMemory, flags, allocatorID);
				return flags == FLAG_GC;
			}


			void MemoryManager::GCCollect()
			{
				//Garbage Collection does not exists without reflection
#ifdef GEM_REFLECTION
				float start = Time::GetTime();
				float time = start;
				float deltaTime = time - start;
				///While there is garbage
				while (m_GarbageList.size() > 0 && deltaTime < GC_DEFAULT_COLLECTION_TIME)
				{
					///Run Collection Cycle
					for (int i = 0; i < GC_DEFAULT_CHUNK_SIZE; i++)
					{
						if (m_GarbageList.size() <= 0)
						{
							break;
						}
						//Get item from the front && Deallocate it
						GCItem & item = m_GarbageList.front();
						Deallocate(item.address, item.type);
					}
					time = Time::GetTime();
					deltaTime = time - start;
				}
#endif
			}

			void MemoryManager::GCCollect(unsigned int aChunkSize, float aTime)
			{
				//Garbage Collection does not exists without reflection
#ifdef GEM_REFLECTION
				float start = Time::GetTime();
				float time = start;
				float deltaTime = time - start;
				///While there is garbage
				while (m_GarbageList.size() > 0 && deltaTime < aTime)
				{
					int itemsRemoved = 0;
					///Run Collection Cycle
					for (int i = 0; i < aChunkSize; i++)
					{
						if (m_GarbageList.size() == 0)
						{
							break;
						}
						//Get item from the front && Deallocate it
						GCItem & item = m_GarbageList.front();
						Deallocate(item.address, item.type);

						itemsRemoved++;
					}
					///Remove all items that were collected.
					if (itemsRemoved > 1)
					{
						m_GarbageList.erase(m_GarbageList.begin(), m_GarbageList.begin() + itemsRemoved);
					}
					else if (itemsRemoved == 1)
					{
						m_GarbageList.erase(m_GarbageList.begin());
					}
				}
#endif
			}

			void MemoryManager::GCCollectAll()
			{
#ifdef GEM_REFLECTION
				//Deallocate all the elements and clear the list.
				for (std::vector<GCItem>::iterator it = m_GarbageList.begin(); it != m_GarbageList.end(); ++it)
				{
					Deallocate((*it).address, (*it).type);
				}
				m_GarbageList.clear();
#endif
			}

			void MemoryManager::Update()
			{
				///Destroy Dynamic Allocators that aren't needed anymore.
				if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_TIME)
				{
					float currentTime = Time::GetTime();
					for (int i = m_DynamicAllocators.size() - 1; i >= 0; i--)
					{
						if (m_DynamicAllocators[i]->IsExpired())
						{
							float deltaTime = currentTime - m_DynamicAllocators[i]->ExpiredTime();
							if (deltaTime > DYNAMIC_ALLOCATOR_LIFETIME)
							{
								delete m_DynamicAllocators[i];
								m_DynamicAllocators.erase(m_DynamicAllocators.begin() + i);
							}
						}
					}
				}
				else if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_IMMEDIATE)
				{
					for (int i = m_DynamicAllocators.size() - 1; i >= 0; i--)
					{
						if (m_DynamicAllocators[i]->IsExpired())
						{
							delete m_DynamicAllocators[i];
							m_DynamicAllocators.erase(m_DynamicAllocators.begin() + i);
						}
					}
				}
			}

			FixedAllocator * MemoryManager::GetAllocator(unsigned int aSize)
			{
				///Search Initial Allocators
				for (unsigned char i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
				{
					if (m_Allocators[i]->ObjectSize() == aSize && !m_Allocators[i]->OutOfMemory())
					{
						return m_Allocators[i];
					}
				}
				///Search Dynamic Allocators
				for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
				{
					///If the size is correct and the allocator is not out of memory
					if ((*it)->ObjectSize() == aSize && !(*it)->OutOfMemory())
					{
						///Check and see if it can be used or not.
						if ((*it)->IsExpired())
						{
							///Allocators are not renewed when they are destroyed and DYNAMIC_DESTROY_IMMEDIATE is true
							if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_IMMEDIATE)
							{
								continue;
							}
							RenewAllocaator((*it));
						}
						return (*it);
					}
				}
				CreateAllocator(aSize);
				FixedAllocator * allocator = m_DynamicAllocators.back();
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(allocator->ObjectSize() == aSize && "Failed to retrieve an allocator the object heap size requested was too large.");
#endif
				return allocator;
			}
			FixedAllocator * MemoryManager::GetAllocator(unsigned int aSize, unsigned int aLength)
			{
				///Search Initial Allocators
				for (unsigned char i = 0; i < MEMORY_ALLOCATOR_COUNT; i++)
				{
					if (m_Allocators[i]->ObjectSize() == aSize && m_Allocators[i]->HasMemory(aLength))
					{
						return m_Allocators[i];
					}
				}
				///Search Dynamic Allocators
				for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
				{
					if ((*it)->ObjectSize() == aSize && (*it)->HasMemory(aLength))
					{
						///Check and see if it can be used or not.
						if ((*it)->IsExpired())
						{
							///Allocators are not renewed when they are destroyed and DYNAMIC_DESTROY_IMMEDIATE is true
							if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_IMMEDIATE)
							{
								continue;
							}
							RenewAllocaator((*it));
						}
						return (*it);
					}
				}
				CreateAllocator(aSize);
				FixedAllocator * allocator = m_DynamicAllocators.back();
#ifdef MEMORY_ERROR_LEVEL_ASSERT
				assert(allocator->ObjectSize() == aSize && !allocator->HasMemory(aLength) && "Failed to retrieve an allocator the object heap size requested was too large.");
#endif
				return allocator;
			}
			FixedAllocator * MemoryManager::GetAllocatorByID(unsigned int aSize, unsigned char aID)
			{
				if (aID == 0)
				{
					for (int i = 0; i < Memory::MEMORY_ALLOCATOR_COUNT; i++)
					{
						if (m_Allocators[i]->ObjectSize() == aSize)
						{
							return m_Allocators[i];
						}
					}
				}
				else
				{
					for (std::vector<FixedAllocator*>::iterator it = m_DynamicAllocators.begin(); it != m_DynamicAllocators.end(); it++)
					{
						if ((*it)->ID() == aID)
						{
							return (*it);
						}
					}
				}
				return nullptr;
			}

			void MemoryManager::CreateAllocator(unsigned int aSize)
			{
				int index = Memory::SizeToBlockIndex(aSize);
				if (index == -1)
				{
					return;
				}
				FixedAllocator * allocator = new FixedAllocator();
				allocator->Initalize(aSize, MEMORY_ALIGNMENT, DYNAMIC_BLOCK_SIZES[index]);
				allocator->ID(m_DynamicID);
				m_DynamicID++;
				m_DynamicAllocators.push_back(allocator);
			}

			unsigned int MemoryManager::WriteHeaderMask(unsigned int aObjectFlag, unsigned int aAllocator)
			{
				unsigned int mask = 0;
				mask |= aObjectFlag << 8;
				mask |= aAllocator;
				return mask;
			}
			void MemoryManager::WriteHeaderMask(void * aMemory, unsigned int aObjectFlag, int aAllocator)
			{
				unsigned int mask = 0;
				mask |= aObjectFlag << 8;
				mask |= aAllocator;
				void * memoryHeader = (void*)((uintptr_t)aMemory - Memory::MEMORY_HEADER_SIZE);
				memcpy(memoryHeader, &mask, sizeof(int));
			}
			void MemoryManager::ReadHeaderMask(void * aMemory, unsigned int & aObjectFlag, int & aAllocator)
			{
				aObjectFlag = Memory::FLAG_INVALID;
				aAllocator = -1;
				if (aMemory != nullptr)
				{
					void * memoryHeader = (void*)((uintptr_t)aMemory - MEMORY_HEADER_SIZE);
					unsigned int headerMask = 0;
					memcpy(&headerMask, memoryHeader, sizeof(int));
					aObjectFlag = headerMask >> 8;
					aAllocator = headerMask << 24;
					aAllocator = aAllocator >> 24;
				}
			}

			void MemoryManager::PrintLeaks(FixedAllocator * aAllocator, int aIndex)
			{
				AllocatorMemInfo info = aAllocator->GetMemoryInfo();

				printf("Allocator ==%d/%d==\nLeaked Allocations: %d\nLeaked Memory: %d\nObject Size: %d\nMax Allocations: %d\nMax Memory: %d\n",
					aAllocator->ID(), aIndex,
					info.allocations,
					info.allocations * info.objectSize,
					info.objectSize,
					info.maxAllocations,
					info.maxAllocations * info.objectSize);
				//printf("Leaked Allocations: %d");
				//printf("Leaked Memory: %d");
				//printf("Object Size: %d");
				//printf("Max Allocations: %d");
				//printf("Max Memory: %d");
			}

			void MemoryManager::ExpireAllocator(FixedAllocator * aAllocator)
			{
				if (aAllocator != nullptr)
				{
					//Flag it, this allocator will be destroyed upon an update call.
					if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_IMMEDIATE)
					{
						aAllocator->IsExpired(true);
						aAllocator->ExpiredTime(Time::GetTime());
					}
					//Flag it, if the allocator was not already flagged and specify the time it was flagged.
					else if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_TIME)
					{
						if (!aAllocator->IsExpired())
						{
							aAllocator->IsExpired(true);
							aAllocator->ExpiredTime(Time::GetTime());
						}
					}
					//Else do nothing, The allocator remains in memory forever.
				}
			}
			void MemoryManager::RenewAllocaator(FixedAllocator * aAllocator)
			{
				if (aAllocator != nullptr)
				{
					if (DYNAMIC_DESTROY_METHOD == DYNAMIC_DESTROY_TIME)
					{
						aAllocator->IsExpired(false);
						aAllocator->ExpiredTime(-1.0f);
					}
				}
			}

			void MemoryManager::Collect(void * aAddress)
			{
#ifdef GEM_REFLECTION
				for (std::vector<GCItem>::iterator it = m_GarbageList.begin(); it != m_GarbageList.end(); ++it)
				{
					if ((*it).address == aAddress)
					{
						m_GarbageList.erase(it);
						break;
					}
				}
#endif
			}

#pragma region OBSOLETE
			//void * MemoryManager::Allocate(unsigned int aSize, unsigned int aLength)
			//{
			//	///Get an allocator
			//	FixedAllocator * allocator = GetAllocator(aSize + Memory::MEMORY_HEADER_SIZE, aLength);
			//
			//#ifdef MEMORY_ERROR_LEVEL_ASSERT
			//	assert(allocator != nullptr && "Failed to retrieve an allocator to allocate with.");
			//	assert(allocator->Initialized() && "Failed to retrieve an allocator to allocate with.");
			//#endif
			//	if (allocator == nullptr || !allocator->Initialized())
			//	{
			//		return nullptr;
			//	}
			//	void * allocatedObj = allocator->Allocate(aLength);
			//	///Write to memory header
			//	void * memoryHeader = (void*)((uintptr_t)allocatedObj - Memory::MEMORY_HEADER_SIZE);
			//	int headerMask = WriteHeaderMask(Memory::FLAG_VALID, allocator->ID());
			//	memcpy(memoryHeader, &headerMask, sizeof(int));
			//
			//	return allocatedObj;
			//}
			//
			//
			//void * MemoryManager::Deallocate(void * aMemory, unsigned int aSize, unsigned int aLength)
			//{
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
			//	FixedAllocator * allocator = GetAllocatorByID(aSize + Memory::MEMORY_HEADER_SIZE, allocatorID);
			//
			//#ifdef MEMORY_ERROR_LEVEL_ASSERT
			//	assert(allocator != nullptr && "Failed to retrieve an allocator to allocate with.");
			//	assert(allocator->Initialized() && "Failed to retrieve an allocator to deallocate with.");
			//#endif
			//	if (allocator == nullptr)
			//	{
			//		return aMemory;
			//	}
			//	///Do not need to check if the allocator is valid, it checks internally
			//	return allocator->Deallocate((void*)((uintptr_t)aMemory - sizeof(void*)-Memory::MEMORY_HEADER_SIZE), aLength);
			//}
#pragma endregion
		}
	}
}