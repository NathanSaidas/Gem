#include "PoolAllocator.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Utilities.h"
namespace Gem
{
    PoolAllocator::PoolAllocator(u32 aObjectSize, u8 aObjectAlignment, u32 aSize, void* aMem) 
        : Allocator(),m_ObjectSize(aObjectSize),m_ObjectAlignment(aObjectAlignment),m_Size(aSize)
    {
        ASSERT(aObjectSize >= sizeof(void*));

        u8 adjustment = alignAdjustment(aMem,aObjectAlignment);
        
        m_FreeList = (void**)((uptr)aMem + adjustment);

        m_NumberOfObjects = (aSize-adjustment)/aObjectSize;

        void ** p = m_FreeList;

        //Initialize the list
        for(u32 i = 0; i < m_NumberOfObjects-1; i++)
        {
            *p = (void*)( (uptr) p + aObjectSize);
            p = (void**) *p;
        }

        *p = nullptr;
    }
    PoolAllocator::~PoolAllocator() 
    {
        log("Memory Leak Report (" + I2S(m_ObjectSize) +")");
        log("Allocations: " + I2S(m_NumberOfAllocations));
        log("Memory Used: " + I2S(m_UsedMemory));

        m_FreeList = nullptr;
    }

    void * PoolAllocator::allocate(u32 aSize, u8 aAlignment)
    {
        ASSERT(aSize == m_ObjectSize && aAlignment == m_ObjectAlignment);
        if(m_FreeList == nullptr)
        {
            return nullptr;
        }
    
        void * p = m_FreeList;
    
        m_FreeList = (void**)(*m_FreeList);
        m_UsedMemory += aSize;
        m_NumberOfAllocations ++;
    
        return p;
    }
    
    
    void PoolAllocator::deallocate(void * p)
    {
        ASSERT(p != 0);
        if(p == 0)
        {
            return;
        }
        *((void**)p) = m_FreeList;
    
        m_FreeList = (void**)p;
        m_UsedMemory -= m_ObjectSize;
        m_NumberOfAllocations--;
    }
    
    using namespace Reflection;

    Type * PoolAllocator::getType()
    {
        return Type::create("Pool_Allocator",TypeID::POOL_ALLOCATOR,sizeof(PoolAllocator),Allocator::getType());
    }



}