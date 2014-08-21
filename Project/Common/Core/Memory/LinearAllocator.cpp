#include "LinearAllocator.h"

namespace Gem
{
    LinearAllocator::LinearAllocator(u32 aSize, void * aStart)
        : Allocator(), m_Size(aSize),m_InitialPosition(aStart),m_CurrentPosition(aStart)
    {
        ASSERT(aSize > 0);
    }

    LinearAllocator::~LinearAllocator()
    {
        m_InitialPosition = nullptr;
        m_CurrentPosition = nullptr;

        m_Size = 0;
    }


    void * LinearAllocator::allocate(u32 aSize, u8 aAlignment)
    {
        ASSERT(aSize != 0);

        u8 adjustment = alignAdjustment(m_CurrentPosition,aAlignment);

        if(m_UsedMemory + adjustment + aSize > m_Size)
        {
            return nullptr;
        }

        uptr alignedAddress = (uptr)m_CurrentPosition + adjustment;

        m_CurrentPosition = (void*)(alignedAddress + aSize);

        m_UsedMemory += aSize + adjustment;
        m_NumberOfAllocations++;

        return (void*)alignedAddress;
    }

    void LinearAllocator::deallocate(void * aPointer)
    {
        ASSERT( false && "Use clear() instead");
    }

    void LinearAllocator::clear()
    {
        m_NumberOfAllocations = 0;
        m_UsedMemory = 0;
        m_CurrentPosition = m_InitialPosition;
    }

}