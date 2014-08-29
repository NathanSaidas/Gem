#include "G_Memory.h"
#include "../Entity Component/G_Component.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Utilities.h"

namespace Gem
{
    namespace Memory
    {
        Object * destroySize(Object * aPtr, int aSize)
        {
            return MemoryManager::instance()->destroySize(aPtr,aSize);
        }
        Component * destroyComponent(Component * aPtr, int aSize)
        {
            return nullptr;
        }
    }
    using namespace Reflection;

    MemoryManager * MemoryManager::s_Instance;
    MemoryManager * MemoryManager::instance()
    {
        if(s_Instance == 0)
        {
            s_Instance = new MemoryManager();
        }
        return s_Instance;
    }
    void MemoryManager::destroy()
    {
        if(s_Instance != 0)
        {
            delete s_Instance;
            s_Instance = 0;
        }
    }
        
    MemoryManager::MemoryManager()
    {
        m_STMRelease = false;
        //Allocate Memory
        m_Block8 = malloc(Memory::BLOCK_8_ALLOC_SIZE);
        m_Block16 = malloc(Memory::BLOCK_16_ALLOC_SIZE);
        m_Block32 = malloc(Memory::BLOCK_32_ALLOC_SIZE);
        m_Block64 = malloc(Memory::BLOCK_64_ALLOC_SIZE);
        m_Block128 = malloc(Memory::BLOCK_128_ALLOC_SIZE);
        m_Block256 = malloc(Memory::BLOCK_256_ALLOC_SIZE);
        m_Block512 = malloc(Memory::BLOCK_512_ALLOC_SIZE);
        m_Block1024 = malloc(Memory::BLOCK_1024_ALLOC_SIZE);
        m_BigBlock = malloc(Memory::BIG_BLOCK_ALLOC_SIZE);

        m_STMBlock8 = malloc(Memory::STM_BLOCK_8_ALLOC_SIZE);
        m_STMBlock16 = malloc(Memory::STM_BLOCK_16_ALLOC_SIZE);
        m_STMBlock32 = malloc(Memory::STM_BLOCK_32_ALLOC_SIZE);
        m_STMBlock64 = malloc(Memory::STM_BLOCK_64_ALLOC_SIZE);
        m_STMBlock128 = malloc(Memory::STM_BLOCK_128_ALLOC_SIZE);

        //m_Block8 = ::operator new(Memory::BLOCK_8_ALLOC_SIZE);
        //m_Block16 = ::operator new(Memory::BLOCK_16_ALLOC_SIZE);
        //m_Block32 = ::operator new(Memory::BLOCK_32_ALLOC_SIZE);
        //m_Block64 = ::operator new(Memory::BLOCK_64_ALLOC_SIZE);
        //m_Block128 = ::operator new(Memory::BLOCK_128_ALLOC_SIZE);
        //m_Block256 = ::operator new(Memory::BLOCK_256_ALLOC_SIZE);
        //m_Block512 = ::operator new(Memory::BLOCK_512_ALLOC_SIZE);
        //m_Block1024 = ::operator new(Memory::BLOCK_1024_ALLOC_SIZE);
        //m_BigBlock = ::operator new(Memory::BIG_BLOCK_ALLOC_SIZE);
        //Create the Pool Allocators
        m_Allocator[(int)Memory::BlockSize::BLOCK_8] = new PoolAllocator(8,8,Memory::BLOCK_8_ALLOC_SIZE,m_Block8);
        m_Allocator[(int)Memory::BlockSize::BLOCK_16] = new PoolAllocator(16,8,Memory::BLOCK_16_ALLOC_SIZE,m_Block16);
        m_Allocator[(int)Memory::BlockSize::BLOCK_32] = new PoolAllocator(32,8,Memory::BLOCK_32_ALLOC_SIZE,m_Block32);
        m_Allocator[(int)Memory::BlockSize::BLOCK_64] = new PoolAllocator(64,8,Memory::BLOCK_64_ALLOC_SIZE,m_Block64);
        m_Allocator[(int)Memory::BlockSize::BLOCK_128] = new PoolAllocator(128,8,Memory::BLOCK_16_ALLOC_SIZE,m_Block128);
        m_Allocator[(int)Memory::BlockSize::BLOCK_256] = new PoolAllocator(256,8,Memory::BLOCK_256_ALLOC_SIZE,m_Block256);
        m_Allocator[(int)Memory::BlockSize::BLOCK_512] = new PoolAllocator(512,8,Memory::BLOCK_512_ALLOC_SIZE,m_Block512);
        m_Allocator[(int)Memory::BlockSize::BLOCK_1024] = new PoolAllocator(1024,8,Memory::BLOCK_1024_ALLOC_SIZE,m_Block1024);
        m_Allocator[(int)Memory::BlockSize::BLOCK_BIG] = new PoolAllocator(Memory::BIG_BLOCK_SIZE,8,Memory::BIG_BLOCK_ALLOC_SIZE,m_BigBlock);

        m_STMAllocator[(int)Memory::BlockSize::BLOCK_8] = new PoolAllocator(8,8,Memory::STM_BLOCK_8_ALLOC_SIZE,m_STMBlock8);
        m_STMAllocator[(int)Memory::BlockSize::BLOCK_16] = new PoolAllocator(16,8,Memory::STM_BLOCK_16_ALLOC_SIZE,m_STMBlock16);
        m_STMAllocator[(int)Memory::BlockSize::BLOCK_32] = new PoolAllocator(32,8,Memory::STM_BLOCK_32_ALLOC_SIZE,m_STMBlock32);
        m_STMAllocator[(int)Memory::BlockSize::BLOCK_64] = new PoolAllocator(64,8,Memory::STM_BLOCK_64_ALLOC_SIZE,m_STMBlock64);
        m_STMAllocator[(int)Memory::BlockSize::BLOCK_128] = new PoolAllocator(128,8,Memory::STM_BLOCK_128_ALLOC_SIZE,m_STMBlock128);
    }
    MemoryManager::~MemoryManager()
    {
        clearSTM();

        for(int i = 0; i < (int)Memory::BlockSize::BLOCK_COUNT; i++)
        {
            if(m_Allocator[i] != 0)
            {
                delete m_Allocator[i];
                m_Allocator[i] = 0;
            }
        }
        for(int i = 0; i < (int)Memory::BlockSize::STM_BLOCK_COUNT; i++)
        {
            if(m_Allocator[i] != 0)
            {
                delete m_Allocator[i];
                m_Allocator[i] = 0;
            }
        }
        //::operator delete(m_Block8);
        //::operator delete(m_Block16);
        //::operator delete(m_Block32);
        //::operator delete(m_Block64);
        //::operator delete(m_Block128);
        //::operator delete(m_Block256);
        //::operator delete(m_Block512);
        //::operator delete(m_Block1024);
        //::operator delete(m_BigBlock);        


        free(m_Block8);
        free(m_Block16);
        free(m_Block32);
        free(m_Block64);
        free(m_Block128);
        free(m_Block256);
        free(m_Block512);
        free(m_Block1024);
        free(m_BigBlock);

        free(m_STMBlock8);
        free(m_STMBlock16);
        free(m_STMBlock32);
        free(m_STMBlock64);
        free(m_STMBlock128);
    }
    Object * MemoryManager::destroySize(Object * aPtr, int aSize)
    {
        PoolAllocator * allocator = getAllocator(aSize);
        if(allocator == nullptr || aPtr == nullptr)
        {
            //Bad Size
            return aPtr;
        }
        allocator->deallocateSize(aPtr);
        return nullptr;
    }

    PoolAllocator * MemoryManager::getAllocator(int aSize)
    {
        if(inBounds(aSize,1,8))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_8];
        }
        else if(inBounds(aSize,9,16))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_16];
        }
        else if(inBounds(aSize,17,32))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_32];
        }
        else if(inBounds(aSize,33,64))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_64];
        }
        else if(inBounds(aSize,65,128))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_128];
        }
        else if(inBounds(aSize,129,256))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_256];
        }
        else if(inBounds(aSize,257,512))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_512];
        }
        else if(inBounds(aSize,513,1024))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_1024];
        }
        else if(inBounds(aSize,1025,Memory::BIG_BLOCK_SIZE))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_BIG];
        }
        return 0;
    }

    

    Type * MemoryManager::getType()
    {
        return Type::create("Memory_Manager",TypeID::MEMORY_MANAGER,sizeof(MemoryManager), Object::getType());
    }

    int MemoryManager::getTotalBytesUsed()
    {
        int memory = 0;

        memory += getBytesUsed(8);
        memory += getBytesUsed(16);
        memory += getBytesUsed(32);
        memory += getBytesUsed(64);
        memory += getBytesUsed(128);
        memory += getBytesUsed(256);
        memory += getBytesUsed(512);
        memory += getBytesUsed(1024);
        memory += getBytesUsed(1025);

        return memory;
    }
    int MemoryManager::getBytesUsed(int aSize)
    {
        PoolAllocator * allocator = getAllocator(aSize);
        if(allocator != nullptr)
        {
            return allocator->numberOfAllocations() * allocator->objectSize();
        }
        return 0;
    }
    int MemoryManager::getTotalBytesFree()
    {
        int memory = 0;

        memory += getBytesFree(Memory::BlockSize::BLOCK_8);
        memory += getBytesFree(Memory::BlockSize::BLOCK_16);
        memory += getBytesFree(Memory::BlockSize::BLOCK_32);
        memory += getBytesFree(Memory::BlockSize::BLOCK_64);
        memory += getBytesFree(Memory::BlockSize::BLOCK_128);
        memory += getBytesFree(Memory::BlockSize::BLOCK_256);
        memory += getBytesFree(Memory::BlockSize::BLOCK_512);
        memory += getBytesFree(Memory::BlockSize::BLOCK_1024);
        memory += getBytesFree(Memory::BlockSize::BLOCK_BIG);
        return memory;
    }
    int MemoryManager::getTotalBytesFree(Memory::ByteSize aByteSize)
    {
        int memory = getTotalBytesFree();

        switch (aByteSize)
        {
        case Gem::Memory::ByteSize::KILOBYTE:
            memory /= KILO_BYTE;
            break;
        case Gem::Memory::ByteSize::MEGABYTE:
            memory /= MEGA_BYTE;
            break;
        case Gem::Memory::ByteSize::GIGABYTE:
            memory /= GIGA_BYTE;
            break;
        }
        return memory;
    }
    int MemoryManager::getBytesFree(Memory::BlockSize aBlockSize)
    {
        int freeBytes = 0;

        switch (aBlockSize)
        {
        case Memory::BlockSize::BLOCK_8:
            freeBytes = Memory::BLOCK_8_ALLOC_SIZE - getBytesUsed(8);
            break;
        case Memory::BlockSize::BLOCK_16:
            freeBytes = Memory::BLOCK_16_ALLOC_SIZE - getBytesUsed(16);
            break;
        case Memory::BlockSize::BLOCK_32:
            freeBytes = Memory::BLOCK_32_ALLOC_SIZE - getBytesUsed(32);
            break;
        case Memory::BlockSize::BLOCK_64:
            freeBytes = Memory::BLOCK_64_ALLOC_SIZE - getBytesUsed(64);
            break;
        case Memory::BlockSize::BLOCK_128:
            freeBytes = Memory::BLOCK_128_ALLOC_SIZE - getBytesUsed(128);
            break;
        case Memory::BlockSize::BLOCK_256:
            freeBytes = Memory::BLOCK_256_ALLOC_SIZE - getBytesUsed(256);
            break;
        case Memory::BlockSize::BLOCK_512:
            freeBytes = Memory::BLOCK_512_ALLOC_SIZE - getBytesUsed(512);
            break;
        case Memory::BlockSize::BLOCK_1024:
            freeBytes = Memory::BLOCK_1024_ALLOC_SIZE - getBytesUsed(1024);
            break;
        case Memory::BlockSize::BLOCK_BIG:
            freeBytes = Memory::BIG_BLOCK_ALLOC_SIZE - getBytesUsed(1025);
            break;
        }

        return freeBytes;
    }
    int MemoryManager::getBytesFree(Memory::BlockSize aBlockSize, Memory::ByteSize aByteSize)
    {
        int memory = getBytesFree(aBlockSize);

        switch (aByteSize)
        {
        case Gem::Memory::ByteSize::KILOBYTE:
            memory /= KILO_BYTE;
            break;
        case Gem::Memory::ByteSize::MEGABYTE:
            memory /= MEGA_BYTE;
            break;
        case Gem::Memory::ByteSize::GIGABYTE:
            memory /= GIGA_BYTE;
            break;
        }
        return memory;
    }


    //STM MEMORY
    PoolAllocator * MemoryManager::getSTMAllocator(int aSize)
    {
        if(inBounds(aSize,1,8))
        {
            return m_STMAllocator[(int)Memory::BlockSize::BLOCK_8];
        }
        else if(inBounds(aSize,9,16))
        {
            return m_STMAllocator[(int)Memory::BlockSize::BLOCK_16];
        }
        else if(inBounds(aSize,17,32))
        {
            return m_STMAllocator[(int)Memory::BlockSize::BLOCK_32];
        }
        else if(inBounds(aSize,33,64))
        {
            return m_STMAllocator[(int)Memory::BlockSize::BLOCK_64];
        }
        else if(inBounds(aSize,65,128))
        {
            return m_STMAllocator[(int)Memory::BlockSize::BLOCK_128];
        }
        return nullptr;
    }
    void MemoryManager::update()
    {
        for(int i = m_STMBlocks.size() -1; i >= 0; i--)
        {
            m_STMBlocks[i].update();
            if(m_STMRelease == true)
            {
                m_STMBlocks.erase(m_STMBlocks.begin() + i);
                m_STMRelease = false;
            }
        }
    }
    STMBlock MemoryManager::registerSTMBlock(Object * aObj, byte aFrameCount, bool aMoveOnFinish)
    {
        STMBlock block;

        if(aObj == nullptr)
        {
            return block;
        }


        block.m_FrameCount = aFrameCount;
        block.m_Ptr = aObj;
        if(aMoveOnFinish == true)
        {
            block.m_Flags = ShortTermMemoryFlags::MOVE_ON_FINISH;
        }
        m_STMBlocks.push_back(block);
        return block;
    }
    void MemoryManager::registerSTMBlock(ISTMListener * aListener, Object * aObj, byte aFrameCount, bool aMoveOnFinish)
    {
        if(aObj == nullptr || aListener == nullptr)
        {
            return;
        }
        m_STMBlocks.push_back(STMBlock());
        m_STMBlocks[m_STMBlocks.size() - 1].m_FrameCount = aFrameCount;
        m_STMBlocks[m_STMBlocks.size() - 1].m_Ptr = aObj;
        if(aMoveOnFinish == true)
        {
            m_STMBlocks[m_STMBlocks.size() - 1].m_Flags = ShortTermMemoryFlags::MOVE_ON_FINISH;
        }
        m_STMBlocks[m_STMBlocks.size() - 1].registerListener(aListener);
    }
    bool MemoryManager::unregisterSTMblock(Object * aObj)
    {
        for(int i = 0; i < m_STMBlocks.size(); i++)
        {
            if(m_STMBlocks[i].ptr() == aObj)
            {
                m_STMBlocks[i].release();
                m_STMBlocks.erase(m_STMBlocks.begin() + i);
                return true;
            }
        }
        return false;
    }
    void MemoryManager::releaseSTM()
    {
        m_STMRelease = true;
    }
    void MemoryManager::clearSTM()
    {
        for(int i = m_STMBlocks.size() - 1; i >= 0; i--)
        {
            m_STMBlocks[i].release();
        }
        m_STMBlocks.clear();
    }
    


    ShortTermMemoryBlock::ShortTermMemoryBlock()
    {
        m_Ptr = nullptr;
        m_FrameCount = 1;
        m_CurrentFrameCount = 0;
        m_Flags = ShortTermMemoryFlags::NONE;
    }
    ShortTermMemoryBlock::~ShortTermMemoryBlock()
    {
        if(m_Ptr != nullptr)
        {
            release();
        }
        
        m_Listeners.clear();
    }

    Object * ShortTermMemoryBlock::ptr()
    {
        return m_Ptr;
    }
    byte ShortTermMemoryBlock::frameCount()
    {
        return m_FrameCount;
    }
    byte ShortTermMemoryBlock::currentFrameCount()
    {
        return m_CurrentFrameCount;
    }
    byte ShortTermMemoryBlock::flags()
    {
        return m_Flags;
    }

    void ShortTermMemoryBlock::registerListener(IShortTermMemoryListener * aListener)
    {
        for(int i = 0; i < m_Listeners.size(); i++)
        {
            if(m_Listeners[i] == aListener)
            {
                return;
            }
        }
        m_Listeners.push_back(aListener);
    }
    void ShortTermMemoryBlock::update()
    {
        m_CurrentFrameCount ++;
        if(m_CurrentFrameCount >= m_FrameCount)
        {
            //Tell memory manager to release it on update
            MemoryManager::instance()->releaseSTM();
            release();
        }
    }
    void ShortTermMemoryBlock::release()
    {
        //Check flags and release accordingly
        if(m_Flags & ShortTermMemoryFlags::MOVE_ON_FINISH == ShortTermMemoryFlags::MOVE_ON_FINISH)
        {
            for(int i = 0; i < m_Listeners.size(); i++)
            {
                if(m_Listeners[i] != nullptr)
                {
                    m_Listeners[i]->onMemoryMove(m_Ptr);
                    
                }
            }
            m_Ptr = nullptr;
        }
        else
        {
            for(int i = 0; i < m_Listeners.size(); i++)
            {
                if(m_Listeners[i] != nullptr)
                {
                    m_Listeners[i]->onMemoryClear(m_Ptr);
                    
                }
            }
            m_Ptr = nullptr;
        }
        
    }
    Type * ShortTermMemoryBlock::getType()
    {
        return Type::create("ShortTermMemoryBlock",TypeID::SHORT_TERM_MEMORY_BLOCK,sizeof(ShortTermMemoryBlock),Object::getType());
    }

}