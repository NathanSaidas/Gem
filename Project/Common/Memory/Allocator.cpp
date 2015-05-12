#include "Allocator.h"
#include <stdio.h>
#include "../Core/Debug.h"
#include "MemoryConfig.h"
#include "PoolAllocator.h"
#include <exception>

using namespace Gem::Debugging;

namespace Gem
{
    namespace Memory
    {
        /// <summary>
        /// The Allocator constructor that assigns the ID to the allocator and sets the default memory usage to 0
        /// </summary>
        /// <param name="aID">The unique ID of the allocator.</param>
        Allocator::Allocator(UInt8 aID)
        {
            m_Memory = nullptr;
            m_Size = 0;
            m_ID = aID;
            m_MemoryInfo.allocations = 0;
            m_MemoryInfo.memoryAvailable = 0;
            m_MemoryInfo.memoryUsed = 0;
            m_ForceDestroy = false;
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="Allocator"/> class.
        /// </summary>
        Allocator::~Allocator()
        {
#ifdef CONFIG_MEMORY_DEBUG
            if (m_MemoryInfo.memoryUsed > 0)
            {
                if(!m_ForceDestroy)
                {
					try
					{
						PoolAllocator * pool = (PoolAllocator*)(this);
						Debug::ErrorFormat("Memory",nullptr, "Allocator(%u,%u) leaked memory: %u(bytes) %u(allocations)\n", m_ID, pool->GetObjectSize(), m_MemoryInfo.memoryUsed, m_MemoryInfo.allocations);
					}
					catch (std::exception aException)
					{
						Debug::ErrorFormat("Memory", nullptr, "Allocator(%u) leaked memory: %u(bytes) %u(allocations)\n", m_ID, m_MemoryInfo.memoryUsed, m_MemoryInfo.allocations);
					}

                }
            }

            if (m_Memory != nullptr)
            {
                if (!m_ForceDestroy)
                {
					Debug::ErrorFormat("Memory", nullptr, "Allocator(%u) was not deallocated.", m_ID);
                }
                free(m_Memory);
            }
#else
            if (m_Memory != nullptr)
            {
                free(m_Memory);
            }
#endif
        }

        /// <summary>
        /// Gets the memory address allocated.
        /// </summary>
        /// <returns>Returns the address of the memory allocated.</returns>
        void * Allocator::GetMemory()
        {
            return m_Memory;
        }

        /// <summary>
        /// Gets the size of the allocation.
        /// </summary>
        /// <returns>Returns the size of the allocation.</returns>
        UInt32 Allocator::GetSize()
        {
            return m_Size;
        }

        /// <summary>
        /// Gets the id of the allocator..
        /// </summary>
        /// <returns>Returns the ID of this allocator.</returns>
        UInt8 Allocator::GetID()
        {
            return m_ID;
        }

        /// <summary>
        /// Gets the memory information of the allocator.
        /// </summary>
        /// <returns>Returns the memory information about the allocator.</returns>
        MemoryInfo Allocator::GetInfo()
        {
            return m_MemoryInfo;
        }
        
        /// <summary>
        /// Flags an allocator to be force destroyed. This will ignore memory leak error catching/reporting.
        /// </summary>
        void Allocator::FlagForceDestroy()
        {
            m_ForceDestroy = true;
        }

    }
}