#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "Allocator.h"


namespace Gem
{
    //From http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010
    class LinearAllocator : public Allocator
    {
    public:
        LinearAllocator(u32 aSize, void* aStart);
        ~LinearAllocator();

        void * allocate(u32 aSize, u8 aAlignment);
        void deallocate(void* p);

        void clear();
    private:
        LinearAllocator(const LinearAllocator&){};
        LinearAllocator & operator=(const LinearAllocator&){};

        void * m_InitialPosition;
        void * m_CurrentPosition;

        u32 m_Size;
    };
}
#endif