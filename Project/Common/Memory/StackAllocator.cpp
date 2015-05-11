#include "StackAllocator.h"
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
        /// Sets all the bit values to 0.
        /// </summary>
        void StackAllocator::StackHeader::Clear()
        {
            mask = 0;
            previous = nullptr;
        }


        /// <summary>
        /// Writes to the memory header.
        /// </summary>
        /// <param name="aFlags">The flags of the memory allocated.</param>
        /// <param name="aID">The ID of the allocator that allocated the memory.</param>
        /// <param name="aSize">The size of the object allocated.</param>
        /// <param name="aPreviousAddress">The previous address of the stack allocation.</param>
        void StackAllocator::StackHeader::Write(const UInt8 & aFlags, const UInt8 & aID, const UInt16 & aSize, void * aPreviousAddress)
        {
            bytes[MemoryHeader::FLAGS] = aFlags;
            bytes[MemoryHeader::ID_OFFSET] = aID;
            bytes[MemoryHeader::SIZE] = aSize;
            bytes[MemoryHeader::SIZE + 1] = aSize >> 8;
            previous = aPreviousAddress;
        }


        /// <summary>
        /// Reads the memory header values into the given arguments.
        /// </summary>
        /// <param name="aFlags">The flags of the memory allocated.</param>
        /// <param name="aID">The ID of the allocator that allocated the memory.</param>
        /// <param name="aSize">The size of the object allocated.</param>
        /// <returns>Returns the pointer to the previous address.</returns>
        void * StackAllocator::StackHeader::Read(UInt8 & aFlags, UInt8 & aID, UInt16 & aSize)
        {
            aFlags = bytes[MemoryHeader::FLAGS];
            aID = bytes[MemoryHeader::ID_OFFSET];
            aSize = (UInt16)bytes[MemoryHeader::SIZE + 1] << 8;
            aSize |= bytes[MemoryHeader::SIZE];
            return previous;
        }

        /// <summary>
        /// Initializes an allocator and allocates memory of the given size.
        /// </summary>
        /// <param name="aSize">The size of the memory allocations</param>
        /// <param name="aID">The ID of the allocator.</param>
        StackAllocator::StackAllocator(UInt32 aSize, UInt8 aID)
            : Allocator(aID)
        {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
              ASSERT(aSize > 0, Error::BAD_STACK_ALLOCATOR_INIT);
    #endif
#endif
            m_Memory = malloc(aSize);
            m_Size = aSize;
            m_CurrentPosition = m_Memory;
            m_PreviousPosition = nullptr;
            m_MemoryInfo.memoryAvailable = aSize;
			m_MemoryInfo.totalMemory = aSize;
        }        

        /// <summary>
        /// Finalizes an instance of the class and deallocates the memory of the allocator.
        /// </summary>
        StackAllocator::~StackAllocator()
        {
            if (m_Memory != nullptr)
            {
                free(m_Memory);
                m_Memory = nullptr;
            }
        }
        
        /// <summary>
        /// Allocates an object of the specific size and alignment off the top of the stack.
        /// </summary>
        /// <param name="aSize">The size of the allocation</param>
        /// <param name="aAlignment">The alignment of the allocation</param>
        /// <returns>Returns a valid address or a nullptr if something went wrong with the allocation.</returns>
        void * StackAllocator::Allocate(UInt32 aSize, UInt8 aAlignment)
        {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                ASSERT(aSize > 0, Error::BAD_STACK_ALLOCATOR_ALLOCATION);
    #endif
#endif
            //Calculate the adjustment
            UInt8 adjustment = MemoryUtils::AlignForwardAdjustmentHeader(m_CurrentPosition, aAlignment, sizeof(StackHeader));

            //Check Memory
            if (!CanAlloc(aSize, aAlignment))
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
                ASSERT(false, Error::OUT_OF_STACK_MEMORY);
    #endif
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::OUT_OF_STACK_MEMORY);
    #endif
#endif
                return nullptr;
            }

            //Adjust the pointer
            void * alignedAddress = MemoryUtils::AddPtr(m_CurrentPosition, adjustment);

            //Write to header
            StackHeader * header = (StackHeader*)(MemoryUtils::SubtractPtr(alignedAddress, sizeof(StackHeader)));
            UInt8 flags = MemoryFlags::VALID | (m_ID == 0 ? MemoryFlags::STATIC : MemoryFlags::DYNAMIC) | MemoryFlags::STACK;
            UInt16 size = (UInt16)aSize;
            header->Write(flags, adjustment, size, m_PreviousPosition);

            

            //Record Memory Usage
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_TRACK
            m_MemoryInfo.allocations++;
            m_MemoryInfo.memoryAvailable -= aSize + adjustment;
            m_MemoryInfo.memoryUsed += aSize + adjustment;
    #endif
#endif
            //Update Pointers.
            m_PreviousPosition = alignedAddress;
            m_CurrentPosition = MemoryUtils::AddPtr(alignedAddress, aSize);

            return alignedAddress;
        }
        
        /// <summary>
        /// Deallocates the address given if this address was the previous allocation made.
        /// Use Rollback when rolling back to a specific address.
        /// </summary>
        /// <param name="aAddress">The previous address allocated to deallocate.</param>
        /// <returns>Returns a nullptr if the deallocation was successful, otherwise it returns the address given.</returns>
        void * StackAllocator::Deallocate(void *aAddress)
        {
            //Make sure the address was the last one allocated.
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_ASSERT
            ASSERT(aAddress == m_PreviousPosition, Error::BAD_STACK_ALLOCATOR_DEALLOCATION);
            ASSERT(aAddress != nullptr, Error::BAD_STACK_ALLOCATOR_DEALLOCATION);
    #endif
#endif
            if (aAddress != m_PreviousPosition || aAddress == nullptr)
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::BAD_STACK_ALLOCATOR_DEALLOCATION);
    #endif
#endif
                return aAddress;
            }

            //Read the header.
            StackHeader * header = (StackHeader*)(MemoryUtils::SubtractPtr(aAddress, sizeof(StackHeader)));
            UInt8 flags;
            UInt8 offset;
            UInt16 size;
            void * previousAddress = header->Read(flags, offset, size);

            //Check state
            if (!((flags & MemoryFlags::STACK) == MemoryFlags::STACK))
            {
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_VERBOSE_LOGGING
                DEBUG_LOG(Error::BAD_STACK_ALLOCATOR_DEALLOCATION);
    #endif
#endif
                return aAddress;
            }

            //Record Memory Usage
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_TRACK
            unsigned int totalSize = (IntPtr)m_CurrentPosition - (IntPtr)aAddress + offset;
            m_MemoryInfo.allocations--;
            m_MemoryInfo.memoryAvailable += totalSize;
            m_MemoryInfo.memoryUsed -= totalSize;
    #endif
#endif
            //Update pointers.
            m_CurrentPosition = MemoryUtils::SubtractPtr(aAddress, offset);
            m_PreviousPosition = previousAddress;
            return nullptr;
        }

        /// <summary>
        /// Deallocates an address with minimal security in place. Use with caution.
		///	If an invalid pointer was used to deallocate with the stack will just deallocate whatever is on the top first.
        /// </summary>
        /// <param name="aAddress">The address last allocated on the stack.</param>
        /// <returns>Returns a nullptr or the address deallocated</returns>
        void * StackAllocator::FastDeallocate(void * aAddress)
        {
            if (m_PreviousPosition == nullptr)
            {
                return nullptr;
            }
            if (m_PreviousPosition == aAddress)
            {
                //Read Header
                StackHeader * header = (StackHeader*)(MemoryUtils::SubtractPtr(m_PreviousPosition, sizeof(StackHeader)));
                UInt8 flags;
                UInt8 offset;
                UInt16 size;
                void * previousAddress = header->Read(flags, offset, size);


                //Record Memory Usage
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_TRACK
                unsigned int totalSize = (uintptr_t)m_CurrentPosition - (uintptr_t)m_PreviousPosition + offset;
                m_MemoryInfo.allocations--;
                m_MemoryInfo.memoryAvailable += totalSize;
                m_MemoryInfo.memoryUsed -= totalSize;
    #endif
#endif

                //Update pointes.
                m_CurrentPosition = MemoryUtils::SubtractPtr(m_PreviousPosition, offset);
                m_PreviousPosition = header->previous;
                return nullptr;
            }
			else if (m_PreviousPosition != nullptr)
            {
                //Read Header
                StackHeader * header = (StackHeader*)(MemoryUtils::SubtractPtr(m_PreviousPosition, sizeof(StackHeader)));
                UInt8 flags;
                UInt8 offset;
                UInt16 size;
                void * previousAddress = header->Read(flags, offset, size);

                //Record Memory Usage
#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_TRACK
                unsigned int totalSize = (uintptr_t)m_CurrentPosition - (uintptr_t)m_PreviousPosition + offset;
                m_MemoryInfo.allocations--;
                m_MemoryInfo.memoryAvailable += totalSize;
                m_MemoryInfo.memoryUsed -= totalSize;
    #endif
#endif

                //Update pointers.
				void * pointerDeallocated = m_PreviousPosition;
                m_CurrentPosition = MemoryUtils::SubtractPtr(m_PreviousPosition, offset);
                m_PreviousPosition = header->previous;
				return pointerDeallocated;
            }
			return nullptr;
        }
        
        /// <summary>
        /// Deallocates until null or the specified address is deallocated
        /// </summary>
        /// <param name="aAddress">The address to deallocate to.</param>
        /// <returns>Returns a nullptr.</returns>
        void * StackAllocator::Rollback(void * aAddress)
        {
            while (m_PreviousPosition != nullptr)
            {
                //Read the header.
                StackHeader * header = (StackHeader*)(MemoryUtils::SubtractPtr(m_PreviousPosition, sizeof(StackHeader)));
                UInt8 flags;
                UInt8 offset;
                UInt16 size;
                void * previousAddress = header->Read(flags, offset, size);

#ifdef CONFIG_MEMORY_DEBUG
    #ifdef CONFIG_MEMORY_TRACK
                ///Add extra information. This could be optimized out.
                unsigned int totalSize = (IntPtr)m_CurrentPosition - (IntPtr)m_PreviousPosition + offset;
                m_MemoryInfo.allocations--;
                m_MemoryInfo.memoryAvailable += totalSize;
                m_MemoryInfo.memoryUsed -= totalSize;
    #endif
#endif
                //Update pointers
                m_CurrentPosition = MemoryUtils::SubtractPtr(m_PreviousPosition, offset);
                m_PreviousPosition = previousAddress;

                //Double check for if the address or null was reached to avoid error on the loop back.
                if (m_PreviousPosition == aAddress)
                {
                    if (m_PreviousPosition == nullptr)
                    {
                        break;
                    }
                    header = (StackHeader*)(MemoryUtils::SubtractPtr(m_PreviousPosition, sizeof(StackHeader)));
                    previousAddress = header->Read(flags, offset, size);
#ifdef CONFIG_MEMORY_DEBUG
	#ifdef CONFIG_MEMORY_TRACK
                    totalSize = (IntPtr)m_CurrentPosition - (IntPtr)m_PreviousPosition + offset;
                    m_MemoryInfo.allocations--;
                    m_MemoryInfo.memoryAvailable += totalSize;
                    m_MemoryInfo.memoryUsed -= totalSize;
	#endif
#endif
                    m_CurrentPosition = MemoryUtils::SubtractPtr(m_PreviousPosition, offset);
                    m_PreviousPosition = header->previous;
                    break;
                }
            }
            return nullptr;
        }
        
        

        /// <summary>
        /// Gets the type of the allocator (Stack)
        /// </summary>
        /// <returns>Returns the type of allocator (Stack)</returns>
        AllocatorType StackAllocator::GetAllocatorType()
        {
            return AllocatorType::Stack;
        }
        
        /// <summary>
        /// Gets the previous pointer position.
        /// </summary>
        /// <returns></returns>
        void * StackAllocator::GetPreviousPosition()
        {
            return m_PreviousPosition;
        }
        
        /// <summary>
        /// Gets the current position of the stack pointer.
        /// </summary>
        /// <returns>Returns the current position of the stack pointer.</returns>
        void * StackAllocator::GetCurrentPosition()
        {
            return m_CurrentPosition;
        }


        /// <summary>
        /// Determines whether this instance can allocate the specified object size.
        ///
        /// Does a Alignment calculation to determine the alignment
        /// </summary>
        /// <param name="aObjectSize">Size of the object.</param>
        /// <param name="aObjectAlignment">Alignment of the object.</param>
        /// <returns>True if the allocation is possible, false if it is not possible.</returns>
        bool StackAllocator::CanAlloc(UInt32 aSize, UInt8 aAlignment)
        {
            UInt8 adjustment = MemoryUtils::AlignForwardAdjustmentHeader(m_CurrentPosition, aAlignment, sizeof(StackHeader));
            if (m_MemoryInfo.memoryUsed + adjustment + aSize > m_Size)
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
        /// <returns>True if the allocation is possible, false if it is not possible.</return>
        bool StackAllocator::CanAlloc(UInt32 aSize)
        {
            if (m_MemoryInfo.memoryUsed + aSize > m_Size)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Returns whether or not this allocator is out of memory.
        /// </summary>
        /// <returns>Returns whether or not this allocator is out of memory.</returns>
        bool StackAllocator::OutOfMemory()
        {
            if (m_MemoryInfo.memoryAvailable <= 0)
            {
                return true;
            }
            return false;
        }
    }
	
}