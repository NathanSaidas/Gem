#ifndef G_MEMORY_H
#define G_MEMORY_H


#include "PoolAllocator.h"
#include "../Base Objects/G_Object.h"

//Byte Defintiions
#define KILO_BYTE 1024             //1024
#define MEGA_BYTE 1024 * KILO_BYTE //1048576
#define GIGA_BYTE 1024 * MEGA_BYTE //1073741824
namespace Gem
{
    namespace Memory
    {
        //Define your pre allocation sizes here
        //408 MegaByte allocation
        const int BLOCK_8_ALLOC_SIZE = 6 * MEGA_BYTE;
        const int BLOCK_16_ALLOC_SIZE = 12 * MEGA_BYTE;
        const int BLOCK_32_ALLOC_SIZE = 24 * MEGA_BYTE;
        const int BLOCK_64_ALLOC_SIZE = 48 * MEGA_BYTE;
        const int BLOCK_128_ALLOC_SIZE = 96 * MEGA_BYTE;
        const int BLOCK_256_ALLOC_SIZE = 96 * MEGA_BYTE;
        const int BLOCK_512_ALLOC_SIZE = 96 * MEGA_BYTE;
        const int BLOCK_1024_ALLOC_SIZE = 30 * MEGA_BYTE;

        //Big Block Memory allocation sizes are used as a last resort if the object you're attempting to allocate does not fit in any of the other blocks.
        //Each Allocation is 8192bytes  Big in this case
        //There are 7040 allocations
        //Thus 55 MB in BIG_BLOCK allocations
        const int BIG_BLOCK_SIZE = 8192;
        const int BIG_BLOCK_ALLOCS = 7040;
        const int BIG_BLOCK_ALLOC_SIZE = BIG_BLOCK_SIZE * BIG_BLOCK_ALLOCS;

        //There is currently 8 block sizes in powers of two
        //There is a ninth block size which is to be user defined.
        enum class Allocator
        {
            BLOCK_8,
            BLOCK_16,
            BLOCK_32,
            BLOCK_64,
            BLOCK_128,
            BLOCK_256,
            BLOCK_512,
            BLOCK_1024,
            BLOCK_BIG,
            BLOCK_COUNT
        };

        //Short hand functions for allocation and deallocation (instantiate and destroy)
        template<class T>
        T * instantiate()
        {
            return MemoryManager::instance()->instantiate<T>();
        }
        template<class T>
        T * instantiate(int aLength)
        {
            return MemoryManager::instance()->instantiate<T>(aLength);
        }
        template<class T>
        T * destroy(void * aPtr)
        {
            return MemoryManager::instance()->destroy<T>(aPtr);
        }
        template<class T>
        T * destroy(void * aPtr, int aLength)
        {
            return MemoryManager::instance()->destroy<T>(aPtr,aLength);
        }
    }

    class MemoryManager : public Object
    {
    public:
        //Singleton Accessors
        static MemoryManager * instance();
        static void destroy();

        //Get the allocator of the proper size and allocate for the object T
        //If the size is invalid ie greater than BIG_BLOCK_SIZE or 0 then a nullptr is returned
        template<class T>
        T * instantiate()
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr)
            {
                //Bad Size
                return nullptr;
            }
            return allocator->allocate<T>();
        }
        //Same as the other allocator except this is for arrays
        template<class T>
        T * instantiate(int aLength)
        {
            int size = sizeof(T) * aLength;
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr)
            {
                //Bad Size
                return nullptr;
            }
            return allocator->allocate<T>(aLength);
        }
        
        //Get the allocator of the proper size and deallocate for the object T
        //the ptr is returned as is if it was null or invalid allocator
        //a nullptr is returned after deallocating
        template<class T>
        T * destroy(void * aPtr)
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr || aPtr == nullptr)
            {
                //Bad Size
                return (T*)aPtr;
            }
            allocator->deallocate<T>(aPtr);
            return nullptr;
        }
        //Same as the other destroy function but this is for arrays
        template<class T>
        T * destroy(void * aPtr, int aLength)
        {
            int size = sizeof(T) * aLength;
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr || aPtr == nullptr)
            {
                //Bad Size
                return (T*)aPtr;
            }
            allocator->deallocate<T>(aPtr);
            return nullptr;
        }
        
        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);

    private:
        static MemoryManager * s_Instance;
        MemoryManager();
        ~MemoryManager();

        //Helper Method to get an allocator for the desired size
        PoolAllocator * getAllocator(int aSize);
        
        //Memory Blocks for each allocator
        void * m_Block8;         //0
        void * m_Block16;        //1
        void * m_Block32;        //2
        void * m_Block64;        //3
        void * m_Block128;       //4
        void * m_Block256;       //5
        void * m_Block512;       //6
        void * m_Block1024;      //7
        void * m_BigBlock;       //8
        //Array of Allocators
        PoolAllocator * m_Allocator[Memory::Allocator::BLOCK_COUNT];
        
    };

    
    
}

#endif