#include "FrameAllocator.h"
#include "../SystemAssert.h"
#include "../SystemError.h"
#include "MemoryUtils.h"
#include "MemoryHeader.h"
#include "MemoryFlags.h"
#include "MemoryConfig.h"
#include "../Utilities/Utilities.h"

namespace Engine
{
    namespace Memory
    {
        /// <summary>
        /// Initializes a new instance of the Frame Allocator. 
        /// Allocates the memory required for allocations from the Allocator.
        /// </summary>
        /// <param name="aMemorySize">Size of the memory to allocate and manage.</param>
        /// <param name="aID">The ID of the allocator</param>
        FrameAllocator::FrameAllocator(UInt32 aMemorySize, UInt8 aID)
            : Allocator(aID)
        {
            m_Memory = nullptr;

#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                Assert(aMemorySize > 0, Error::BAD_FRAME_ALLOCATOR_INIT);
    #endif
#endif
            m_Memory = malloc(aMemorySize);
            m_Size = aMemorySize;
            m_CurrentPosition = m_Memory;
            m_MemoryInfo.memoryAvailable = aMemorySize;
			m_MemoryInfo.totalMemory = aMemorySize;
        }

        /// <summary>
        /// Finalizes an instance of the class, free the memory that has been allocated.
        /// </summary>
        FrameAllocator::~FrameAllocator()
        {
            if (m_Memory != nullptr)
            {
                free(m_Memory);
                m_Memory = nullptr;
            }
        }
        
        /// <summary>
        /// Allocates the specified a size with the specified alignment.
        /// 
        /// The size must be greater than 0
        /// There must be enough memory available in order to allocate.
        /// </summary>
        /// <param name="aSize">The size of the object.</param>
        /// <param name="aAlignment">The alignment of the object.</param>
        /// <returns>Returns a valid pointer that has been allocated or a nullptr if there was a problem allocating.</returns>
        void * FrameAllocator::Allocate(UInt32 aSize, UInt8 aAlignment)
        {
            // Error Handle the size
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                ASSERT(aSize > 0, Error::BAD_FRAME_ALLOCATOR_ALLOCATION);
    #endif
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                if (aSize <= 0)
                {
                    DEBUG_LOG(Error::BAD_FRAME_ALLOCATOR_ALLOCATION);
                    return nullptr;
                }
    #endif
#endif

            // Calculate an alignment adjustment to make
            UInt8 adjustment = MemoryUtils::AlignForwardAdjustmentHeader(m_CurrentPosition, aAlignment, sizeof(MemoryHeader));

            // Check Available Memory
            if (!CanAlloc(aSize + (UInt32)adjustment))
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                ASSERT(false, Error::OUT_OF_FRAME_MEMORY);
    #endif
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::OUT_OF_FRAME_MEMORY);
    #endif  
#endif
                return nullptr;
            }
            ///Calculate the next aligned address
            IntPtr alignedAddress = (uintptr_t)m_CurrentPosition + adjustment;
            ///Write header info
            MemoryHeader * header = (MemoryHeader*)(reinterpret_cast<void*>(alignedAddress - sizeof(MemoryHeader)));
            UInt8 flags = MemoryFlags::VALID | (m_ID = 0 ? MemoryFlags::STATIC : MemoryFlags::DYNAMIC) | MemoryFlags::FRAME;
            UInt16 objectSize = (UInt16)aSize;
            header->Write(flags, m_ID,objectSize);
            //header->bytes[MemoryHeader::ALLOCATOR_ID] = m_ID;
            //header->bytes[MemoryHeader::OFFSET] = adjustment;
            //header->bytes[MemoryHeader::FLAGS] = MemoryFlags::VALID | (m_ID = 0 ? MemoryFlags::DEFAULT : MemoryFlags::DYNAMIC) | MemoryFlags::FRAME;
            //header->bytes[MemoryHeader::RESERVED] = 0;

            m_CurrentPosition = (void*)(alignedAddress + aSize);

            ///This is just extra information and can be optimized out using compilation macros.
            m_MemoryInfo.allocations++;
            m_MemoryInfo.memoryUsed += aSize + adjustment;
            m_MemoryInfo.memoryAvailable -= aSize + adjustment;

            return (void*)alignedAddress;
        }
        
        /// <summary>
        /// Frame allocators dont deallocate memory. This is just stubbed out to be an Allocator.
        /// Use the Reset method to reset the frame back to the start.
        /// </summary>
        /// <param name="aAddress">The addressto deallocate</param>
        /// <returns>Returns the address passed in.</returns>
        void * FrameAllocator::Deallocate(void * aAddress)
        {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                ASSERT(false, Error::BAD_FRAME_ALLOCATOR_DEALLOCATION);
    #endif
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::BAD_FRAME_ALLOCATOR_DEALLOCATION);
    #endif
#endif
            return aAddress;
        }
        
        /// <summary>
        /// Gets the type of allocator.
        /// </summary>
        /// <returns>Returns the type of the allocator </returns>
        AllocatorType FrameAllocator::GetAllocatorType()
        {
            return AllocatorType::Frame;
        }
        
        /// <summary>
        /// Resets the frame allocator back to the start.
        /// </summary>
        void FrameAllocator::Reset()
        {
            m_MemoryInfo.allocations = 0;
            m_MemoryInfo.memoryAvailable = m_Size;
            m_MemoryInfo.memoryUsed = 0;

            m_CurrentPosition = m_Memory;
        }
        
        /// <summary>
        /// Determines whether this instance can allocate the specified object size.
        ///
        /// Does a Alignment calculation to determine the alignment
        /// </summary>
        /// <param name="aObjectSize">Size of the object.</param>
        /// <param name="aObjectAlignment">Alignment of the object.</param>
        /// <returns>True if the allocation is possible, false if it is not possible.</returns>
        bool FrameAllocator::CanAlloc(UInt32 aObjectSize, UInt8 aObjectAlignment)
        {
            UInt8 adjustment = MemoryUtils::AlignForwardAdjustmentHeader(m_CurrentPosition, aObjectAlignment, sizeof(MemoryHeader));
            if (m_MemoryInfo.memoryUsed + adjustment + aObjectSize > m_Size)
            {
                return false;
            }
            return true;
        }
        
        /// <summary>
        /// Determines whether this instance can allocate the specified object size.
        ///
        /// Does not perform an Alignment calculation. Assume aObjectSize includes the adjustment
        /// </summary>
        /// <param name="aObjectSize">Size of a object and the adjustment.</param>
        /// <returns>True if the allocation is possible, false if it is not possible.</returns>
        bool FrameAllocator::CanAlloc(UInt32 aObjectSize)
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
        bool FrameAllocator::OutOfMemory()
        {
            if (m_MemoryInfo.memoryAvailable <= 0)
            {
                return true;
            }
            return false;
        }
    }
}
