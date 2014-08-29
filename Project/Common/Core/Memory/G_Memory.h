#ifndef G_MEMORY_H
#define G_MEMORY_H


#include "PoolAllocator.h"
#include "../Base Objects/G_BaseObjects.h"
#include "../Reflection/G_Operators.h"
#include "G_ISTMListener.h"
#include <vector>

//Byte Defintiions
#define KILO_BYTE 1024             //1024
#define MEGA_BYTE 1024 * KILO_BYTE //1048576
#define GIGA_BYTE 1024 * MEGA_BYTE //1073741824

#define ASSERT_MEMORY_LEAK 1

namespace Gem
{
    namespace ShortTermMemoryFlags
    {
        const int NONE              = 0;
        const int MOVE_ON_FINISH    = 1 << 1;
    };

    class ShortTermMemoryBlock : public Object
    {
    public:
        ShortTermMemoryBlock();
        ~ShortTermMemoryBlock();

        Object * ptr();
        byte frameCount();
        byte currentFrameCount();
        byte flags();

        void registerListener(IShortTermMemoryListener * aListener);
        

    private:

        void update();
        void release();

        Object * m_Ptr;
        byte m_FrameCount;
        byte m_CurrentFrameCount;
        byte m_Flags;
        //This class has no ownership to its listeners
        std::vector<IShortTermMemoryListener*> m_Listeners;


        friend class MemoryManager;

        virtual Reflection::Type * getType();
    private:
    };

    typedef ShortTermMemoryBlock STMBlock;

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

        const int STM_BLOCK_8_ALLOC_SIZE = 4 * MEGA_BYTE;
        const int STM_BLOCK_16_ALLOC_SIZE = 4 * MEGA_BYTE;
        const int STM_BLOCK_32_ALLOC_SIZE = 4 * MEGA_BYTE;
        const int STM_BLOCK_64_ALLOC_SIZE = 8 * MEGA_BYTE;
        const int STM_BLOCK_128_ALLOC_SIZE = 8 * MEGA_BYTE;

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

        template<class T>
        void instantiate(std::vector<Object*> & aArgs)
        {
            return MemoryManager::instance()->instantiate(aArgs);
        }
        template<class T>
        void instantiate(std::vector<Object> & aArgs)
        {
            //return MemoryManager::instance()->instantiate(aArgs);
        }
        template<class T>
        void instantiate(std::vector<Object*> & aPtrArgs,std::vector<Object> & aArgs)
        {

        }

        Object * destroySize(Object * aPtr, int aSize);


        class Component;
        Component * destroyComponent(Component * aPtr, int aSize);


        //STM Methods

        template<class T>
        Object * instantiateSTM(STMBlock & aBlock,byte aFrameCount = 1, bool aMoveOnFinish = false)
        {
            return MemoryManager::instance()->instantiateSTM<T>(aBlock,aFrameCount,aMoveOnFinish);
        }
        template<class T>
        Object * instantiateSTM(ISTMListener * aListener, byte aFrameCount = 1, bool aMoveOnFinish = false)
        {
            return MemoryManager::instance()->instantiateSTM<T>(aListener,aFrameCount,aMoveOnFinish);
        }

        template<class T>
        Object * destroySTM(Object * aPtr)
        {
            return MemoryManager::instance()->destroySTM<T>(aPtr);
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
        template<class T>
        T * instantiate(std::vector<Object*> & aArgs)
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr)
            {
                return nullptr;
            }
            return allocator->allocator<T>(aArgs);
        }
        template<class T>
        T * instantiate(std::vector<Object> & aArgs)
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr)
            {
                return nullptr;
            }
            return allocator->allocator<T>(aArgs);
        }
        template<class T>
        T * instantiate(std::vector<Object*> & aPtrArgs,std::vector<Object> & aArgs)
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getAllocator(size);
            if(allocator == nullptr)
            {
                return nullptr;
            }
            return allocator->allocator<T>(aPtrArgs,aArgs);
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
            T * address = allocator->allocate<T>(aLength);
            return address;
        }
        
        //Get the allocator of the proper size and deallocate for the object T
        //the ptr is returned as is if it was null or invalid allocator
        //a nullptr is returned after deallocating

        Object * destroySize(Object * aPtr, int aSize);

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
            allocator->deallocate<T>(aPtr,aLength);
            return nullptr;
        }
        
        virtual Reflection::Type * getType();

        int getTotalBytesUsed();
        int getBytesUsed(int aSize);
        int getTotalBytesFree();
        int getTotalBytesFree(Memory::ByteSize aByteSize);
        int getBytesFree(Memory::BlockSize aBlockSize);
        int getBytesFree(Memory::BlockSize aBlockSize, Memory::ByteSize aByteSize);


        

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
        PoolAllocator * m_Allocator[Memory::BlockSize::BLOCK_COUNT];


        //Short Term Memory blocks
        void * m_STMBlock8;
        void * m_STMBlock16;
        void * m_STMBlock32;
        void * m_STMBlock64;
        void * m_STMBlock128;

        //Other STM members
        bool m_STMRelease;
        PoolAllocator * m_STMAllocator[Memory::BlockSize::STM_BLOCK_COUNT];
        std::vector<STMBlock> m_STMBlocks;

        //STM Funcs
        PoolAllocator * getSTMAllocator(int aSize);
        void update();

        STMBlock registerSTMBlock(Object * aObj, byte aFrameCount, bool aMoveOnFinish);
        void registerSTMBlock(ISTMListener * aListener, Object * aObj, byte aFrameCount, bool aMoveOnFinish);
        bool unregisterSTMblock(Object * aObj);

    public:
        void releaseSTM();

        //*******WARNING*******
        //ONLY USE THIS IF U KNOW WHAT YOU'RE DOING
        //THIS ERASES ALL MEMORY ON THE STM Blocks
        //*******WARNING*******
        void clearSTM();


        template<class T>
        Object * instantiateSTM(STMBlock & aBlock, byte aFrameCount = 1, bool aMoveOnFinish = false)
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getSTMAllocator(size);
            if(allocator == nullptr)
            {
                //Bad Size
                return nullptr;
            }
            Object * obj =  (Object*)allocator->allocate<T>();
            aBlock = registerSTMBlock(obj,aFrameCount,aMoveOnFinish);

            return obj;
        }

        template<class T>
        Object * instantiateSTM(ISTMListener * aListener, byte aFrameCount = 1, bool aMoveOnFinish = false)
        {
            int size = sizeof(T);
            PoolAllocator * allocator = getSTMAllocator(size);
            if(allocator == nullptr)
            {
                //Bad Size
                return nullptr;
            }
            Object * obj =  (Object*)allocator->allocate<T>();
            registerSTMBlock(aListener, obj,aFrameCount,aMoveOnFinish);

            return obj;
        }

        template<class T>
        Object * destroySTM(Object * aObj)
        {
            if(unregisterSTMblock(aObj) == false)
            {
                return aObj;
            }

            int size = sizeof(T);
            PoolAllocator * allocator = getSTMAllocator(size);
            if(allocator == nullptr)
            {
                return aObj;
            }
            allocator->deallocate<T>(aObj);
            return nullptr;
        }

        


    private:

        //For memory leak  report
        friend Allocator;
        friend class Application;
        
    };

    
    
}

#endif