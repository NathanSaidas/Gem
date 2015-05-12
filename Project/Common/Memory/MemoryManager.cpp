#include "MemoryManager.h"
//#include "../SystemAssert.h"
//#include "../SystemError.h"
#include "../Core/BasicTypes.h"
//#include "../Utilities/Utilities.h"

namespace Gem
{
    namespace Memory
    {
        MemoryManager * MemoryManager::s_Instance = nullptr;

        MemoryManager::MemoryManager()
        {
            m_LockedAllocator = nullptr;
            for (int i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
            {
                m_DynamicPool[i] = nullptr;
            }

			//Initialize the stack, frame and default pool allocators. 
            m_StaticStack = new StackAllocator(Config::Memory::DEFAULT_STACK_ALLOC_SIZE, 0);
            m_StaticFrame = new FrameAllocator(Config::Memory::DEFAULT_FRAME_ALLOC_SIZE, 0);

            for (int i = 0; i < Config::Memory::DEFAULT_POOL_BLOCK_COUNT; i++)
            {
                m_StaticPool[i] = new PoolAllocator(Config::Memory::DEFAULT_POOL_ALLOC_SIZES[i], 0, Config::Memory::DEFAULT_POOL_BLOCK_SIZES[i], Config::Memory::DEFAULT_OBJECT_ALIGNMENT);
            }
        }
        MemoryManager::~MemoryManager()
        {
			//Free up any dynamic pool allocators. (Allocators which were allocated when the default one's were full)
            for (int i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
            {
                if (m_DynamicPool[i] != nullptr)
                {
                    delete m_DynamicPool[i];
                    m_DynamicPool[i] = nullptr;
                }
            }
			//Free up the stack, frame and default pool allocators.
            delete m_StaticStack;
            m_StaticStack = nullptr;
            delete m_StaticFrame;
            m_StaticFrame = nullptr;

            for (int i = 0; i < Config::Memory::DEFAULT_POOL_BLOCK_COUNT; i++)
            {
                delete m_StaticPool[i];
                m_StaticPool[i] = nullptr;
            }

            m_LockedAllocator = nullptr;

			//Log any memory leaks found.
			for (std::map<IntPtr, AllocationInfo>::iterator it = m_RecordedAllocations.begin(); it != m_RecordedAllocations.end(); ++it)
            {
                AllocationInfo info = (*it).second;
                printf("::Leaked Memory::\nFile: %s\n Line: %d\nAddress: %d\n", info.GetFilename(), info.GetLine(), info.GetAddress());
            }

        }

        /// <summary>
        /// Initializes the memory manager by creating a singleton instance of MemoryManager
        /// </summary>
        void MemoryManager::Initialize()
        {
            if (s_Instance == nullptr)
            {
                s_Instance = new MemoryManager();
            }
        }
        
        /// <summary>
        /// Terminates the memory manager by destroying the singleton instance of the MemoryManager
        /// </summary>
        void MemoryManager::Terminate()
        {
            if (s_Instance != nullptr)
            {
                delete s_Instance;
                s_Instance = nullptr;
            }
        }
                
        /// <summary>
        /// Gets the singleton instance of the Memory Manager.
        /// </summary>
        /// <returns>Returns the singleton instance of the memory manager.</returns>
        MemoryManager * MemoryManager::GetInstance()
        {
            return s_Instance;
        }



		///<summary>
		/// Allocates an address for the specified size and alignment. The type of allocattion can be specified as well.
		///</summary>
		/// <param name="aSize">The size of the object to be allocated.</param>
		/// <param name="aAlignment">The alignment of the object to be allocated.</param>
		/// <param name="aFilename">The name of the file making the allocation.</param>
		/// <param name="aLine">The line where the allocation is made.</param>
#ifdef CONFIG_MEMORY_DEBUG
		void * MemoryManager::Allocate(UInt32 aSize, UInt8 aAlignment, AllocatorType aType, const char * aFilename, UInt32 aLine)
#else
		void * MemoryManager::Allocate(UInt32 aSize, UInt8 aAlignment, AllocatorType aType)
#endif
		{
			//If the size or alignment is invalid the allocation cannot occur. Return null.
			if (aSize == 0 || aAlignment == 0)
			{
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_VERBOSE_LOGGING
				//DEBUG_LOG(Error::INVALID_ALLOCATION_SIZE);
	#endif
#endif
				return nullptr;
			}

			void * address = nullptr;

			//If there is a allocator specified to use. Use it instead.
			if (m_LockedAllocator != nullptr)
			{
				//Pools need a size adjustment for the allocation.
				if (m_LockedAllocator->GetAllocatorType() == AllocatorType::Pool)
				{
					UInt32 objectSize = MemoryUtils::ConvertSizeToPoolSize(aSize);
					address = m_LockedAllocator->Allocate(objectSize, aAlignment);
				}
				//Other allocators use the default type.
				else
				{
					address = m_LockedAllocator->Allocate(aSize, aAlignment);
				}
			}
			else
			{
				switch (aType)
				{
				case AllocatorType::Frame:
					address = AllocateFrame(aSize, aAlignment);
					break;
				case AllocatorType::Stack:
					address = AllocateStack(aSize, aAlignment);
					break;
				case AllocatorType::Pool:
					address = AllocatePool(aSize, aAlignment);
					break;

				default:
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_VERBOSE_LOGGING
					//DEBUG_LOG(Error::INVALID_ALLOCATOR_TYPE);
	#endif
#endif
					break;
				}
			}
			

#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_TRACK
			//Record the information if the address is not null.
			if (address != nullptr && m_RecentAllocator != nullptr && aType != AllocatorType::Frame)
			{
				RecordAllocation(reinterpret_cast<IntPtr>(address), aFilename, aLine, m_RecentAllocator->GetID(), aType);
			}
	#endif
#endif
			return address;
		}

		///<summary>
		/// Deallocates the address. This assumes the memory was allocated from the memory manager to begin with and utilizes the MemoryHeader for getting additional information.
		///</summary>
		/// <param name="aAddress">The address to be deallocated.</param>
		/// <param name="aType">The type of allocator to deallocate from.</param>
		/// <param name="aObjectSize">The size of the object for pool deallocations for extra security and dynamic pool searching.</param>
		void * MemoryManager::Deallocate(void * aAddress, AllocatorType aType, UInt32 aObjectSize )
		{
			if (aAddress == nullptr)
			{
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_VERBOSE_LOGGING
				//DEBUG_LOG(Error::INVALID_DEALLOCATION_ADDRESS);
	#endif
#endif
				return nullptr;
			}

			void * address = nullptr;
			//If theres a specified allocator to use. Use it first.
			if (m_LockedAllocator != nullptr)
			{
				address = m_LockedAllocator->Deallocate(aAddress);
			}
			else
			{
				switch (aType)
				{
				case AllocatorType::Frame:
					address = DeallocateFrame(aAddress);
					break;
				case AllocatorType::Stack:
					address = DeallocateStack(aAddress);
					break;
				case AllocatorType::Pool:
					address = DeallocatePool(aAddress, aObjectSize);
					break;
				default:
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_VERBOSE_LOGGING
					//DEBUG_LOG(Error::INVALID_ALLOCATOR_TYPE);
	#endif
#endif
					break;
				}
			}

			


#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_TRACK
			if (address == nullptr)
			{
				EraseAllocation(reinterpret_cast<IntPtr>(aAddress));
			}
	#endif
#endif
			return address;
		}
		
		/// <summary>
		/// Rollbacks the stack to the specified address.
		/// </summary>
		/// <param name="aAddress">The address to rollback to</param>
		/// <returns>Returns a nullptr </returns>
		void * MemoryManager::RollbackStack(void * aAddress)
		{
			if (m_StaticStack != nullptr)
			{
#ifdef CONFIG_MEMORY_DEBUG
#ifdef CONFIG_MEMORY_TRACK
				
				while(true)
				{
					void * address = m_StaticStack->FastDeallocate(aAddress);
					if (address == nullptr)
					{
						//Finished Rolling back the stack
						if (aAddress != nullptr)
						{
							EraseAllocation(reinterpret_cast<IntPtr>(aAddress));
						}
						break;
					}
					else
					{
						//Continue rolling back the stack.
						EraseAllocation(reinterpret_cast<IntPtr>(address));
					}
				}
#else
				return m_StaticStack->Rollback(aAddress);
#endif
#else
				return m_StaticStack->Rollback(aAddress);
#endif
			}
			return nullptr;
		}
		
		/// <summary>
		/// Rollbacks the stack to a nullptr;
		/// </summary>
		/// <returns>Returns a nullptr;</returns>
		void * MemoryManager::RollbackStack()
		{
			return RollbackStack(nullptr);
		}
		
		/// <summary>
		/// Resets the frame allocator memory back to the start.
		/// </summary>
		void MemoryManager::ResetFrame()
		{
			if (m_StaticFrame != nullptr)
			{
				m_StaticFrame->Reset();
			}
		}

		///<summary>
		/// Searches for the allocator using the address pased in and then locks the allocator of the given address.
		///</summary>
		///<param name="aAddress">The address to search for the allocator for.</param>
		void MemoryManager::LockAllocator(void * aAddress)
		{
			if (m_LockedAllocator == nullptr)
			{
				m_LockedAllocator = GetAllocator(aAddress);
			}
		}

		///<summary>
		/// Locks the stack allocator if no allocator is not locked
		///</summary>
		void MemoryManager::LockStackAllocator()
		{
			if (m_LockedAllocator == nullptr)
			{
				m_LockedAllocator = m_StaticStack;
			}
		}		

		///<summary>
		/// Locks the frame allocator if no allocator is not locked
		///</summary>
		void MemoryManager::LockFrameAllocator()
		{
			if (m_LockedAllocator == nullptr)
			{
				m_LockedAllocator = m_StaticFrame;
			}
		}

		///<summary>
		/// Locks the pool allocator if no allocator is not locked. Searches through static ones before dynamic.
		///</summary>
		///<param name="aObjectSize">The size of the object to compare to for pool allocator searches.</param>
		///<param name="aAlignment">The alignment of the object to compare to for pool allocator searches.</param>
		void MemoryManager::LockPoolAllocator(UInt32 aObjectSize, UInt8 aAlignment)
		{
			LockStaticPoolAllocator(aObjectSize, aAlignment);
			LockDynamicPoolAllocator(aObjectSize, aAlignment);
		}

		///<summary>
		/// Locks the pool allocator if no allocator is not locked.
		///</summary>
		///<param name="aObjectSize">The size of the object to compare to for pool allocator searches.</param>
		///<param name="aAlignment">The alignment of the object to compare to for pool allocator searches.</param>
		void MemoryManager::LockStaticPoolAllocator(UInt32 aObjectSize, UInt8 aAlignment)
		{
			if (m_LockedAllocator == nullptr)
			{
				SInt8 allocatorIndex = MemoryUtils::ConvertSizeToIndex(aObjectSize);
				if (allocatorIndex != -1)
				{
					if (m_StaticPool[allocatorIndex]->GetObjectAlignment() == aAlignment)
					{
						m_LockedAllocator = m_StaticPool[allocatorIndex];
					}
				}
			}
			
		}

		///<summary>
		/// Locks the pool allocator if no allocator is not locked.
		///</summary>
		///<param name="aObjectSize">The size of the object to compare to for pool allocator searches.</param>
		///<param name="aAlignment">The alignment of the object to compare to for pool allocator searches.</param>
		void MemoryManager::LockDynamicPoolAllocator(UInt32 aObjectSize, UInt8 aAlignment)
		{
			if (m_LockedAllocator == nullptr)
			{
				UInt32 objectSize = MemoryUtils::ConvertSizeToPoolSize(aObjectSize);

				for (int i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
				{
					if (m_DynamicPool[i] != nullptr
						&& m_DynamicPool[i]->GetObjectSize() == objectSize
						&& m_DynamicPool[i]->GetObjectAlignment() == aAlignment)
					{
						m_LockedAllocator = m_DynamicPool[i];
						break;
					}
				}
			}
		}

		///<summary>
		/// Locks the pool allocator if no allocator is not locked.
		///</summary>
		///<param name="aIndex">The index of the dynamic pool allocator to lock.</param>
		void MemoryManager::LockDynamicPoolAllocator(UInt8 aIndex)
		{
			if (m_LockedAllocator == nullptr && m_DynamicPool[aIndex] != nullptr)
			{
				m_LockedAllocator = m_DynamicPool[aIndex];
			}
		}

		///<summary>
		/// Releases the lock on the allocator.
		///</summary>
		void MemoryManager::ReleaseLock()
		{
			m_LockedAllocator = nullptr;
		}

		///<summary>
		/// Gets the state of whether or not the allocator is locked.
		///</summary>
		///<returns>Returns the state of whether or not the allocator is locked</returns>
		bool MemoryManager::IsLocked()
		{
			return m_LockedAllocator != nullptr;
		}

		///<summary>
		/// Checks whether or not a static stack allocator can allocate memory for the given object.
		///</summary>
		///<param name="aObjectSize">The size of the object to be allocated.</param>
		///<param name="aAlignment">The alignment of the object to be allocated.</param>
		///<returns>Returns whether or not its possible to allocate the specified memory</returns>
        bool MemoryManager::CanAllocStack(UInt32 aObjectSize, UInt8 aAlignment)
        {
            if (m_StaticStack != nullptr)
            {
                return m_StaticStack->CanAlloc(aObjectSize, aAlignment);
            }
            return false;
        }

		///<summary>
		/// Checks whether or not a static frame allocator can allocate memory for the given object.
		///</summary>
		///<param name="aObjectSize">The size of the object to be allocated.</param>
		///<param name="aAlignment">The alignment of the object to be allocated.</param>
		///<returns>Returns whether or not its possible to allocate the specified memory</returns>
        bool MemoryManager::CanAllocFrame(UInt32 aObjectSize, UInt8 aAlignment)
        {
            if (m_StaticFrame != nullptr)
            {
                return m_StaticFrame->CanAlloc(aObjectSize, aAlignment);
            }
            return false;
        }

		///<summary>
		/// Checks whether or not any pool allocator or just the static can allocate memory for the given object.
		///</summary>
		///<param name="aObjectSize">The size of the object to be allocated.</param>
		///<param name="aAlignment">The alignment of the object to be allocated.</param>
		///<returns>Returns whether or not its possible to allocate the specified memory</returns>
        bool MemoryManager::CanAllocPool(UInt32 aObjectSize, UInt8 aAlignent, bool aStaticOnly)
        {
            if (aStaticOnly)
            {
                return CanAllocPoolStatic(aObjectSize, aAlignent);
            }
            else
            {
                if (CanAllocPoolStatic(aObjectSize, aAlignent))
                {
                    return true;
                }
                else
                {
                    return CanAllocPoolDynamic(aObjectSize, aAlignent);
                }
            }
            return false; // This code should never be reached.
        }

		///<summary>
		/// Checks whether or not a static pool allocator can allocate memory for the given object.
		///</summary>
		///<param name="aObjectSize">The size of the object to be allocated.</param>
		///<param name="aAlignment">The alignment of the object to be allocated.</param>
		///<returns>Returns whether or not its possible to allocate the specified memory</returns>
        bool MemoryManager::CanAllocPoolStatic(UInt32 aObjectSize, UInt8 aAlignment)
        {
			//Adjust the object size to fit the right allocator size.
            UInt32 correctedObjectSize = MemoryUtils::ConvertSizeToPoolSize(aObjectSize);

			//Search for an allocator that has the right object size and alignment. 
			//Check the allocator to see if it has the memory to allocate
            for (int i = 0; i < Config::Memory::DEFAULT_POOL_BLOCK_COUNT; i++)
            {
                if (m_StaticPool[i] != nullptr
                    && m_StaticPool[i]->GetObjectSize() == correctedObjectSize
                    && m_StaticPool[i]->GetObjectAlignment() == aAlignment)
                {
                    return m_StaticPool[i]->CanAlloc(correctedObjectSize, aAlignment);
                }
            }
            return false;
        }

		///<summary>
		/// Checks whether or not any dynamic pool allocator can allocate memory for the given object.
		///</summary>
		///<param name="aObjectSize">The size of the object to be allocated.</param>
		///<param name="aAlignment">The alignment of the object to be allocated.</param>\
		///<returns>Returns whether or not its possible to allocate the specified memory</returns>
        bool MemoryManager::CanAllocPoolDynamic(UInt32 aObjectSize, UInt8 aAlignment)
        {
			//Adjust the object size to fit the right allocator size.
            UInt32 correctedObjectSize = MemoryUtils::ConvertSizeToPoolSize(aObjectSize);

			//Search for an allocator that has the right object size and alignment.
			//Check the allocator to see if it has the memory to allocate.
			//In the event the allocator doesnt have enough memory search for the next match.
            for (int i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
            {
                if (m_DynamicPool[i] != nullptr
                    && m_DynamicPool[i]->GetObjectSize() == correctedObjectSize
                    && m_DynamicPool[i]->GetObjectAlignment() == aAlignment
					&& m_DynamicPool[i]->CanAlloc(correctedObjectSize, aAlignment))
                {
					return true;
                }
            }
            return false;
        }


		///<summary>
		/// Creates a pool allocator using the given parameters.
		///</summary>
		///<param name="aObjectSize">The size of the object that can be allocated by the pool allocator.</param>
		///<param name="aAlignment">The alignment of the object that can be allocated by the pool allocator.</param>
		///<returns>Returns the created allocator.</returns>
        PoolAllocator * MemoryManager::CreatePoolAllocator(UInt32 aObjectSize, UInt8 aAlignment)
        {
            UInt32 correctedObjectSize = MemoryUtils::ConvertSizeToPoolSize(aObjectSize);
            UInt32 memoryAllocationSize = Config::Memory::DYNAMIC_POOL_ALLOC_SIZES[MemoryUtils::ConvertSizeToIndex(correctedObjectSize)];

            //Find an available index
            for (UInt8 i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
            {
                if (m_DynamicPool[i] == nullptr)
                {
                    //Spot Found, Create the allocator
                    m_DynamicPool[i] = new PoolAllocator(memoryAllocationSize, i + 1, correctedObjectSize, aAlignment);
					return m_DynamicPool[i];
                }
            }
			return nullptr;
        }
				
		/// <summary>
		/// Destroys the pool allocator at the given index.
		/// </summary>
		/// <param name="aIndex">The index of the allocator to destroy.</param>
		void MemoryManager::DestroyPoolAllocator(UInt32 aIndex)
        {
            if (aIndex >= 0 && aIndex < Config::Memory::DYNAMIC_ALLOCATOR_COUNT)
            {
                if (m_DynamicPool[aIndex] != nullptr)
                {
                    DestroyPoolAllocator(m_DynamicPool[aIndex]);
                    m_DynamicPool[aIndex] = nullptr;
                }
            }
        }
	
		/// <summary>
		/// Properly destroys the pool allocator and removes all the allocation info related to the allocator.
		/// </summary>
		/// <param name="aAllocator">The allocator to destroy</param>
		void MemoryManager::DestroyPoolAllocator(PoolAllocator * aAllocator)
        {
            if (aAllocator != nullptr)
            {
                aAllocator->FlagForceDestroy();
                
                //TODO: Erase all allocations associated with this allocator.

				//Get the starting address
				//Loop over addresses based on the current allocations made and remove those from the map

                delete aAllocator;
            }
        }
        
        /// <summary>
        /// Gets the stack allocator.
        /// </summary>
        /// <param name="aRequireMemory">Whether or not to require memory from the allocator.</param>
        /// <returns>Returns the StackAllocator</returns>
        StackAllocator * MemoryManager::GetStackAllocator(bool aRequireMemory)
        {
            if (aRequireMemory)
            {
                if (m_StaticStack != nullptr && !m_StaticStack->OutOfMemory())
                {
                    return m_StaticStack;
                }
                return nullptr;
            }
            return m_StaticStack;
        }
        
        /// <summary>
        /// Gets the frame allocator.
        /// </summary>
        /// <param name="aRequireMemory">Whether or not to require memory from the allocator.</param>
        /// <returns>Returns the Frame allocator.</returns>
        FrameAllocator * MemoryManager::GetFrameAllocator(bool aRequireMemory)
        {
            if (aRequireMemory)
            {
                if (m_StaticFrame != nullptr && !m_StaticFrame->OutOfMemory())
                {
                    return m_StaticFrame;
                }
                return nullptr;
            }
            return m_StaticFrame;
        }
        
        /// <summary>
        /// Searches for a pool allocator. Searches the static ones first then the dynamic.
        /// </summary>
        /// <param name="aRequireMemory">Whether or not to require memory from the allocator.</param>
        /// <param name="aObjectSize">The allocator object size.</param>
        /// <param name="aAlignment">The allocator object alignment</param>
        /// <param name="aStaticOnly">Searches for static pool allocators only.</param>
        /// <returns>Returns the found pool allocator.</returns>
        PoolAllocator * MemoryManager::GetPoolAllocator(bool aRequireMemory, UInt32 aObjectSize, UInt8 aAlignment, bool aStaticOnly)
        {
            if (aStaticOnly)
            {
                return GetStaticPoolAllocator(aRequireMemory, aObjectSize, aAlignment);
            }
            else
            {
                PoolAllocator * poolAllocator = GetStaticPoolAllocator(aRequireMemory, aObjectSize, aAlignment);
                if (poolAllocator != nullptr)
                {
                    return poolAllocator;
                }
                else
                {
                    return GetDynamicPoolAllocator(aRequireMemory, aObjectSize, aAlignment);
                }
            }
        }        
        /// <summary>
        /// Searches for a static pool allocator that matches the object size / alignment
        /// If aRequireMemory is true the memory usage of the allocator is checked. This does not use the CanAlloc method
        /// </summary>
        /// <param name="aRequireMemory">Whether or not to check the memory usage</param>
        /// <param name="aObjectSize">Size of a object.</param>
        /// <param name="aAlignment">The alignment of the object</param>
        /// <returns>Returns an allocator that matches the description. (ObjectSize/Alignment/Memory). Returns null if none are found.</returns>
        PoolAllocator * MemoryManager::GetStaticPoolAllocator(bool aRequireMemory, UInt32 aObjectSize, UInt8 aAlignment)
        {
            if (aObjectSize == 0 || aObjectSize > 2048)
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                //DEBUG_LOG(Error::INVALID_ALLOCATION_SIZE);
    #endif
#endif
                return nullptr;
            }

            UInt32 correctedObjectSize = MemoryUtils::ConvertSizeToPoolSize(aObjectSize);

            for (int i = 0; i < Config::Memory::DEFAULT_POOL_BLOCK_COUNT; i++)
            {
                if (m_StaticPool[i] != nullptr
                    && m_StaticPool[i]->GetObjectSize() == correctedObjectSize
                    && m_StaticPool[i]->GetObjectAlignment() == aAlignment)
                {
                    if (aRequireMemory)
                    {
                        if (!m_StaticPool[i]->OutOfMemory())
                        {
                            return m_StaticPool[i];
                        }
                    }
                    else
                    {
                        return m_StaticPool[i];
                    }
                    break;
                }
            }
            return nullptr;
        }
        
        /// <summary>
        /// Searches for the first dynamic pool allocator who has the specific object size / alignment
        /// If aRequireMemory is true then a memory check is done to make sure the allocator is not full. This doesnt check if an allocation is possible.
        /// </summary>
        /// <param name="aRequireMemory">Whether or not to check memory usage.</param>
        /// <param name="aObjectSize">Size of a object.</param>
        /// <param name="aAlignment">The alignment of the object.</param>
        /// <returns>Returns the pool allocator matching the description. (ObjectSize/Alignment/MemoryUsage) or null if none could be found.</returns>
        PoolAllocator * MemoryManager::GetDynamicPoolAllocator(bool aRequireMemory, UInt32 aObjectSize, UInt8 aAlignment)
        {
            if (aObjectSize == 0 || aObjectSize > 2048)
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                //DEBUG_LOG(Error::INVALID_ALLOCATION_SIZE);
    #endif
#endif
                return nullptr;
            }

            UInt32 correctedObjectSize = MemoryUtils::ConvertSizeToPoolSize(aObjectSize);

            for (int i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
            {
				if (m_DynamicPool[i] != nullptr
					&& m_DynamicPool[i]->GetObjectSize() == correctedObjectSize
					&& m_DynamicPool[i]->GetObjectAlignment() == aAlignment)
                {
                    if (aRequireMemory)
                    {
						if (!m_DynamicPool[i]->OutOfMemory())
                        {
							return m_DynamicPool[i];
                        }
                    }
                    else
                    {
						return m_DynamicPool[i];
                    }
                }
            }

			return nullptr;
        }

		/// <summary>
		/// Searches for the allocator responsible for allocating the address.
		/// </summary>
		/// <param name="aAddress">The address to search for.</param>
		/// <returns>Allocator associated with the address.</returns>
		Allocator * MemoryManager::GetAllocator(void * aAddress)
		{
			if (aAddress == nullptr)
			{
				return nullptr;
			}

			MemoryHeader * header = (MemoryHeader*)MemoryUtils::SubtractPtr(aAddress, 4);
			UInt8 flags;
			UInt8 id;
			UInt16 size;
			header->Read(flags, id, size);

			if ((flags & MemoryFlags::STACK) == MemoryFlags::STACK)
			{
				return m_StaticStack;
			}
			else if ((flags & MemoryFlags::FRAME) == MemoryFlags::FRAME)
			{
				return m_StaticFrame;
			}
			else if ((flags & MemoryFlags::DYNAMIC) == MemoryFlags::DYNAMIC)
			{
				return m_DynamicPool[id];
			}
			else
			{
				SInt8 index = MemoryUtils::ConvertSizeToIndex(size);
				if (index != -1)
				{
					return m_StaticPool[index];
				}
			}
			return nullptr;

		}
        
        /// <summary>
        /// Records the allocation made.
        /// </summary>
        /// <param name="aAddress">The address allocated.</param>
        /// <param name="aFile">The file responsible of the leak.</param>
        /// <param name="aLine">The line within the file which caused the memory leak.</param>
        /// <param name="aAllocatorID">The allocator ID responsible for managing the allocation. This may change and a flag will be raised in case of leak.</param>
        /// <param name="aType">The type of allocation that was made.</param>
        void MemoryManager::RecordAllocation(IntPtr aAddress, const char * aFile, UInt32 aLine, UInt8 aAllocatorID, AllocatorType aType)
        {
            //Find the Allocation with the address
            std::map<IntPtr,AllocationInfo>::iterator element = m_RecordedAllocations.find(aAddress);

            //If the record already exists
            if (element != m_RecordedAllocations.end())
            {
                //Add a hit count to flag leaked memory.
                element->second.AddCount();
                //Check the allocator. If there was a mismatch flag that boolean as well.
                if (element->second.GetAllocatorID() != aAllocatorID)
                {
                       element->second.FlagAllocatorMismatch();
                       element->second.SetAllocatorID(aAllocatorID);
                }
            }
            else
            {
                //If it did not exist add a new entry for that allocation.
                std::pair<IntPtr, AllocationInfo> record;
                record.first = aAddress;
                record.second = AllocationInfo(aFile, aLine, aAllocatorID, aType, aAddress);

                m_RecordedAllocations.insert(record);
            }
        }
        
        /// <summary>
        /// Erases the allocation info associated with the address given.
        /// </summary>
        /// <param name="aAddress">The address which was allocated that needs to be removed from the leak detection records.</param>
        void MemoryManager::EraseAllocation(IntPtr aAddress)
        {
            std::map<IntPtr, AllocationInfo>::iterator element = m_RecordedAllocations.find(aAddress);

            if (element != m_RecordedAllocations.end())
            {
                element->second.SubtractCount();
                if (element->second.GetCount() == 0)
                {
                    m_RecordedAllocations.erase(element);
                }
            }
        }

		/// <summary>
		/// Allocates memory from the stack.
		/// </summary>
		/// <param name="aSize">The size of the allocation to be made.</param>
		/// <param name="aAlignment">The alignment of the allocation to be made.</param>
		void * MemoryManager::AllocateStack(UInt32 aSize, UInt8 aAlignment)
		{
			if (m_StaticStack != nullptr)
			{
				m_RecentAllocator = m_StaticStack;
				return m_StaticStack->Allocate(aSize, aAlignment);
			}
			return nullptr;
		}

		/// <summary>
		/// Allocates memory from the frame.
		/// </summary>
		/// <param name="aSize">The size of the allocation to be made.</param>
		/// <param name="aAlignment">The alignment of the allocation to be made.</param>
		void * MemoryManager::AllocateFrame(UInt32 aSize, UInt8 aAlignment)
		{
			if (m_StaticFrame != nullptr)
			{
				m_RecentAllocator = m_StaticFrame;
				return m_StaticFrame->Allocate(aSize, aAlignment);
			}
			return nullptr;
		}

		/// <summary>
		/// Allocates memory from the pool.
		/// </summary>
		/// <param name="aSize">The size of the allocation to be made.</param>
		/// <param name="aAlignment">The alignment of the allocation to be made.</param>
		void * MemoryManager::AllocatePool(UInt32 aSize, UInt8 aAlignment)
		{
			UInt32 objectSize = 0;
			SInt8 allocatorIndex = 0;
			MemoryUtils::GetCorrectObjectInfo(aSize, objectSize, allocatorIndex);

			if (objectSize == 0 || allocatorIndex == -1)
			{
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_VERBOSE_LOGGING
				//DEBUG_LOG(Error::INVALID_ALLOCATION_SIZE);
	#endif
#endif
				return nullptr;
			}


			//Static Alloc
			if (m_StaticPool[allocatorIndex]->CanAlloc(objectSize))
			{
				m_RecentAllocator = m_StaticPool[allocatorIndex];
				return m_StaticPool[allocatorIndex]->Allocate(objectSize, aAlignment);
			}
			else
			{
				//Search for an allocator that already exists and can allocate the object
				for (int i = 0; i < Config::Memory::DYNAMIC_ALLOCATOR_COUNT; i++)
				{
					if (m_DynamicPool[i] != nullptr &&
						m_DynamicPool[i]->GetObjectSize() == objectSize &&
						m_DynamicPool[i]->GetObjectAlignment() == aAlignment &&
						m_DynamicPool[i]->CanAlloc(objectSize))
					{
						m_RecentAllocator = m_DynamicPool[i];
						return m_DynamicPool[i]->Allocate(objectSize, aAlignment);
					}
				}

				//Create an allocator and allocate from that.
				PoolAllocator * allocator = CreatePoolAllocator(objectSize, aAlignment);
				if (allocator != nullptr)
				{
					m_RecentAllocator = allocator;
					return allocator->Allocate(objectSize, aAlignment);
				}
			}
			return nullptr;
		}


		/// <summary>
		/// Deallocates an address from the stack.
		/// </summary>
		/// <param name="aAddress">The address to deallocate</param>
		void * MemoryManager::DeallocateStack(void * aAddress)
		{
			if (m_StaticStack != nullptr && aAddress != nullptr)
			{
				MemoryHeader * header = (MemoryHeader*)MemoryUtils::SubtractPtr(aAddress, 4);
				UInt8 flags;
				UInt8 id;
				UInt16 size;
				header->Read(flags, id, size);

				if ((flags & MemoryFlags::STACK) != MemoryFlags::STACK)
				{
					//This is not a stack allocation
					return aAddress;
				}

				m_RecentAllocator = m_StaticStack;
				return m_StaticStack->Deallocate(aAddress);
			}
			return aAddress;
		}

		/// <summary>
		/// Deallocates an address from the frame.
		/// </summary>
		/// <param name="aAddress">The address to deallocate</param>
		void * MemoryManager::DeallocateFrame(void * aAddress)
		{
			//Frame Allocator cannot deallocate memory.
			if (m_StaticFrame != nullptr && aAddress != nullptr)
			{
				MemoryHeader * header = (MemoryHeader*)MemoryUtils::SubtractPtr(aAddress, 4);
				UInt8 flags;
				UInt8 id;
				UInt16 size;
				header->Read(flags, id, size);
				
				if ((flags & MemoryFlags::FRAME) != MemoryFlags::FRAME)
				{
					//This is not a frame allocation
					return aAddress;
				}

				m_RecentAllocator = m_StaticFrame;
				return m_StaticFrame->Deallocate(aAddress);
			}
			return aAddress;
		}

		/// <summary>
		/// Deallocates an address from a pool.
		/// </summary>
		/// <param name="aAddress">The address to deallocate</param>
		/// <param name="aObjectSize">The size of the object to deallocate</param>
		void * MemoryManager::DeallocatePool(void * aAddress, UInt32 aObjectSize)
		{
			if (aAddress == nullptr)
			{
				return aAddress;
			}
			UInt32 objectSize = 0;
			SInt8 allocatorIndex = 0;
			MemoryUtils::GetCorrectObjectInfo(aObjectSize, objectSize, allocatorIndex);

			MemoryHeader * header = (MemoryHeader*)MemoryUtils::SubtractPtr(aAddress, 4);
			UInt8 flags;
			UInt8 id;
			UInt16 size;
			header->Read(flags, id, size);

			if ((flags & MemoryFlags::POOL) != MemoryFlags::POOL)
			{
				//This is not a pool allocation
				return aAddress;
			}

			if ((flags & MemoryFlags::GC_FLAGGED) == MemoryFlags::GC_FLAGGED)
			{
				//This address was flagged for garbage collection abort the deallocation
				return aAddress;
			}

			//Deallocate static allocator memory
			if ((flags & MemoryFlags::STATIC) == MemoryFlags::STATIC)
			{
				m_RecentAllocator = m_StaticPool[allocatorIndex];
				return m_StaticPool[allocatorIndex]->Deallocate(aAddress);
			}
			//Deallocate dynamic allocator memory
			else if(m_DynamicPool[id - 1] != nullptr)
			{
				m_RecentAllocator = m_DynamicPool[id - 1];
				return m_DynamicPool[id - 1]->Deallocate(aAddress);
			}
			return aAddress;
		}

    }
	
}