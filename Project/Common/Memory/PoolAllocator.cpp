#include "PoolAllocator.h"
#include "../SystemAssert.h"
#include "../SystemError.h"
#include "MemoryHeader.h"
#include "MemoryUtils.h"
#include "MemoryFlags.h"
#include "MemoryConfig.h"
#include "../Utilities/Utilities.h"

namespace Engine
{
    namespace Memory
    {
        
        /// <summary>
        /// Initializes the PoolAllocator class. Allocates memory and sets up the linked list of pointers.
        /// </summary>
        /// <param name="aSize">The total size of the memory allocation</param>
        /// <param name="aID">The ID of the allocator</param>
        /// <param name="aObjectSize">The max size of the objects that can be allocated.</param>
        /// <param name="aObjectAlignment">The alignment of the objects</param>
        PoolAllocator::PoolAllocator(UInt32 aSize, UInt8 aID, UInt32 aObjectSize, UInt8 aObjectAlignment)
            : Allocator(aID)
        {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
            ASSERT(aSize > 0, Error::BAD_POOL_ALLOCATOR_INIT);
    #endif
#endif
            

            m_Memory = std::malloc(aSize);
            m_Size = aSize;
            m_ObjectSize = aObjectSize + sizeof(MemoryHeader);
            m_ObjectAlignment = aObjectAlignment;

            //Calculate the adjustment
            unsigned char adjustment = MemoryUtils::AlignForwardAdjustment(m_Memory, m_ObjectAlignment);

            m_CurrentPosition = (void**)MemoryUtils::AddPtr(m_Memory, adjustment);

            unsigned int count = (aSize - adjustment) / m_ObjectSize;
            m_MemoryInfo.memoryAvailable = count * m_ObjectSize;
			m_MemoryInfo.totalMemory = aSize;

            void ** ptr = m_CurrentPosition;
            ///Initialize Blocks
            for (unsigned int i = 0; i < count - 1; i++)
            {
                *ptr = (void*)((uintptr_t)ptr + m_ObjectSize);
                ptr = (void**)*ptr;
            }

            if (count == 1)
            {
                ptr = (void**)*ptr;
            }

            *ptr = nullptr;
        }
        
        /// <summary>
        /// Finalizes an instance of this class and free all of the memory;
        /// </summary>
        PoolAllocator::~PoolAllocator()
        {
            if (m_Memory != nullptr)
            {
                std::free(m_Memory);
                m_Memory = nullptr;
            }
        }
        
        /// <summary>
        /// Allocates an object of the given size with the given alignment.        
        ///
        /// For an allocation to work the method must.
        /// • Be passed a size and alignment that matches m_ObjectSize and m_ObjectAlignment
        /// • The allocator must have enough memory
        /// </summary>
        /// <param name="aSize">The size of the object to allocate</param>
        /// <param name="aAlignment">The alignment of the object to allocate</param>
        /// <returns>Returns a valid pointer if the allocation succeeded. Returns a nullptr if the allocation failed.</returns>
        void * PoolAllocator::Allocate(UInt32 aSize, UInt8 aAlignment)
        {
            //Verify valid allocation request
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
            ASSERT(aSize + sizeof(MemoryHeader) == m_ObjectSize && aAlignment == m_ObjectAlignment, Error::BAD_POOL_ALLOCATOR_ALLOCATION);
    #endif
#endif
            //Check current state
            if (m_CurrentPosition == nullptr)
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::OUT_OF_POOL_MEMORY);
    #endif
#endif
                return nullptr;
            }

            //TODO: Check Memory
            if (!CanAlloc(aSize + sizeof(MemoryHeader)))
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                ASSERT(false, Error::OUT_OF_POOL_MEMORY);
    #endif
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::OUT_OF_POOL_MEMORY);
    #endif
#endif
                return nullptr;
            }


            //Move the pointer down the list
            void * current = m_CurrentPosition;
            m_CurrentPosition = (void**)(*m_CurrentPosition);

            //Track the memory info
            m_MemoryInfo.allocations++;
            m_MemoryInfo.memoryAvailable -= m_ObjectSize;
            m_MemoryInfo.memoryUsed += m_ObjectSize;

            //Construct the header and write to it
            MemoryHeader * header = (MemoryHeader*)current;
            UInt8 flags = MemoryFlags::VALID | (m_ID == 0 ? MemoryFlags::STATIC : MemoryFlags::DYNAMIC) | MemoryFlags::POOL;
            UInt16 objectSize = (UInt16)m_ObjectSize;
            header->Write(flags, m_ID, objectSize);

            current = MemoryUtils::AddPtr(current, sizeof(MemoryHeader));

            return current;
        }
        
        /// <summary>
        /// Deallocates the specified address. 
        /// </summary>
        /// <param name="aAddress">The address to deallocate</param>
        /// <returns>Returns a nullptr if the deallocation was successful, returns the address that was sent in if the deallocation failed.</returns>
        void * PoolAllocator::Deallocate(void * aAddress)
        {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
            ASSERT(aAddress != nullptr, Error::BAD_POOL_ALLOCATOR_DEALLOCATION);
    #endif
#endif
            if (aAddress == nullptr)
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::BAD_POOL_ALLOCATOR_DEALLOCATION);
    #endif
#endif
                return nullptr;
            }
            //Read the header
            UInt8 flags = 0;
            UInt8 id = 0;
            UInt16 objectSize = 0;
            MemoryHeader * header = (MemoryHeader*)(MemoryUtils::SubtractPtr(aAddress, sizeof(MemoryHeader)));
            header->Read(flags, id, objectSize);

            if (id != m_ID)
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::BAD_POOL_ALLOCATOR_DEALLOCATION);
    #endif
#endif
                return aAddress;
            }
            else if (!((flags & MemoryFlags::POOL) == MemoryFlags::POOL))
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::BAD_POOL_ALLOCATOR_DEALLOCATION);
    #endif
#endif
                return aAddress;
            }
			else if (objectSize != m_ObjectSize)
			{
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_VERBOSE_LOGGING
				DEBUG_LOG(Error::BAD_POOL_ALLOCATOR_DEALLOCATION);
	#endif
#endif
				return aAddress;
			}
            //TODO Check Object Size
            
            //Adjust the free list pointer.
            *((void**)header) = m_CurrentPosition;
            m_CurrentPosition = (void**)header;

            m_MemoryInfo.allocations--;
            m_MemoryInfo.memoryAvailable += m_ObjectSize;
            m_MemoryInfo.memoryUsed -= m_ObjectSize;

            return nullptr;
        }
        
        /// <summary>
        /// Returns the type of allocator this is. (Pool)
        /// </summary>
        /// <returns>Returns the type of allocator this is. (Pool)</returns>
        AllocatorType PoolAllocator::GetAllocatorType()
        {
            return AllocatorType::Pool;
        }
        
        /// <summary>
        /// Returns the size of the objects that get allocated.
        /// </summary>
        /// <returns>Returns the size of the objects that get allocated </returns>
        UInt32 PoolAllocator::GetObjectSize()
        {
            return m_ObjectSize;
        }     

        /// <summary>
        /// Returns the alignment of the objects that get allocated
        /// </summary>
        /// <returns>Returns the alignment of the objects that get allocated</returns>
        UInt8 PoolAllocator::GetObjectAlignment()
        {
            return m_ObjectAlignment;
        }

        /// <summary>
        /// Determines whether this instance can allocate the specified object size.
        ///
        /// Does a Alignment calculation to determine the alignment
        /// </summary>
        /// <param name="aObjectSize">Size of the object.</param>
        /// <param name="aObjectAlignment">Alignment of the object.</param>
        /// <returns>True if the allocation is possible, false if it is not possible.</returns>
        bool PoolAllocator::CanAlloc(UInt32 aObjectSize, UInt8 aObjectAlignment)
        {
            return CanAlloc(aObjectSize);
        }

        /// <summary>
        /// Determines whether this instance can allocate the specified object size.
        ///
        /// Does not perform an Alignment calculation. Assume aObjectSize includes the adjustment
        /// </summary>
        /// <param name="aObjectSize">Size of a object and the adjustment.</param>
        /// <returns>True if the allocation is possible, false if it is not possible.</return>
        bool PoolAllocator::CanAlloc(UInt32 aObjectSize)
        {
            if (m_MemoryInfo.memoryUsed + aObjectSize > m_Size)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Returns whether or not this allocator is out of memory.
        /// </summary>
        /// <returns>Returns whether or not this allocator is out of memory.</returns>
        bool PoolAllocator::OutOfMemory()
        {
            if (m_MemoryInfo.memoryAvailable <= 0)
            {
                return true;
            }
            return false;
        }

    }
	
}