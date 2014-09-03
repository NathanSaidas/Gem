#ifndef G_MEMORY_MANAGER_H
#define G_MEMORY_MANAGER_H

#include "G_Allocator.h"

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
        enum class BlockSize
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
            BLOCK_COUNT,
            STM_BLOCK_COUNT = 5
        };
        enum class ByteSize
        {
            BYTE,
            KILOBYTE,
            MEGABYTE,
            GIGABYTE
        };
    }



    class MemoryManager
    {
    public:
        static MemoryManager * instance();
        static void destroy();

        Reflection::Primitive * allocate(string & aTypename);
        Reflection::Primitive * allocate(integer & aTypeID);
        Reflection::Primitive * allocate(Reflection::Type * aType);
        Reflection::Primitive * deallocate(Reflection::Primitive * aPtr, string & aTypename);
        Reflection::Primitive * deallocate(Reflection::Primitive * aPtr, integer & aTypeID);
        Reflection::Primitive * deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType);

        template<class T>
        T * allocate()
        {
            int size = sizeof(T);
            Allocator * allocator = getAllocator(size);
            if (allocator == nullptr)
            {
                return nullptr;
            }
            return allocator->allocate<T>();
        }
        template<class T>
        T * deallocate(T * aAddress)
        {
            int size = sizeof(T);
            Allocator * allocator = getAllocator(size);
            if (allocator == nullptr)
            {
                return aAddress;
            }
            return allocator->deallocate<T>(aAddress);
        }

        template<class T>
        T * allocateArray(unsigned int aLength)
        {
            int size = sizeof(T) * aLength;
            Allocator * allocator = getAllocator(size);
            if (allocator == nullptr)
            {
                return nullptr;
            }
            return allocator->allocateArray<T>(aLength);
        }

        template<class T>
        T * deallocateArray(T * aAddress, unsigned int aLength)
        {
            int size = sizeof(T) * aLength;
            Allocator * allocator = getAllocator(size);
            if (allocator == nullptr)
            {
                return aAddress;
            }
            return allocator->deallocateArray<T>(aAddress, aLength);
        } 
    private:
        static MemoryManager * s_Instance;
        MemoryManager();
        ~MemoryManager();

        Allocator * getAllocator(int aSize);

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
        Allocator * m_Allocator[(unsigned int)Memory::BlockSize::BLOCK_COUNT];


    };

    namespace Memory
    {
        //Helpers
        Reflection::Primitive * instantiate(const char * aTypename);
        Reflection::Primitive * instantiate(string & aTypename);
        Reflection::Primitive * instantiate(integer & aTypeID);
        Reflection::Primitive * instantiate(Reflection::Type * aType);
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, const char * aTypename);
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, string & aTypename);
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, integer & aTypeID);
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, Reflection::Type * aType);
        template<class T>
        T * instantiate()
        {
            return MemoryManager::instance()->allocate<T>();
        }
        template<class T>
        T * destroy(T * aAddress)
        {
            return MemoryManager::instance()->deallocate<T>(aAddress);
        }
        template<class T>
        T * instantiateArray(unsigned int aLength)
        {
            return MemoryManager::instance()->allocateArray<T>(aLength);
        }
        template<class T>
        T * destroyArray(T * aAddress, unsigned int aLength)
        {
            return MemoryManager::instance()->deallocateArray<T>(aAddress, aLength);
        }
    }


}

#endif