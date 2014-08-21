#include "G_Memory.h"
#include "../Reflection/G_Reflection.h"
#include "../Utilities/G_Utilities.h"

namespace Gem
{
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
        //Create the Pool Allocators
        m_Allocator[(int)Memory::Allocator::BLOCK_8] = new PoolAllocator(8,8,Memory::BLOCK_8_ALLOC_SIZE,m_Block8);
        m_Allocator[(int)Memory::Allocator::BLOCK_16] = new PoolAllocator(16,8,Memory::BLOCK_16_ALLOC_SIZE,m_Block16);
        m_Allocator[(int)Memory::Allocator::BLOCK_32] = new PoolAllocator(32,8,Memory::BLOCK_32_ALLOC_SIZE,m_Block32);
        m_Allocator[(int)Memory::Allocator::BLOCK_64] = new PoolAllocator(64,8,Memory::BLOCK_64_ALLOC_SIZE,m_Block64);
        m_Allocator[(int)Memory::Allocator::BLOCK_128] = new PoolAllocator(128,8,Memory::BLOCK_16_ALLOC_SIZE,m_Block128);
        m_Allocator[(int)Memory::Allocator::BLOCK_256] = new PoolAllocator(256,8,Memory::BLOCK_256_ALLOC_SIZE,m_Block256);
        m_Allocator[(int)Memory::Allocator::BLOCK_512] = new PoolAllocator(512,8,Memory::BLOCK_512_ALLOC_SIZE,m_Block512);
        m_Allocator[(int)Memory::Allocator::BLOCK_1024] = new PoolAllocator(1024,8,Memory::BLOCK_1024_ALLOC_SIZE,m_Block1024);
        m_Allocator[(int)Memory::Allocator::BLOCK_BIG] = new PoolAllocator(Memory::BIG_BLOCK_SIZE,8,Memory::BIG_BLOCK_ALLOC_SIZE,m_BigBlock);
    }
    MemoryManager::~MemoryManager()
    {
        for(int i = 0; i < (int)Memory::Allocator::BLOCK_COUNT; i++)
        {
            if(m_Allocator[i] != 0)
            {
                delete m_Allocator[i];
                m_Allocator[i] = 0;
            }
        }
        free(m_Block8);
        free(m_Block16);
        free(m_Block32);
        free(m_Block64);
        free(m_Block128);
        free(m_Block256);
        free(m_Block512);
        free(m_Block1024);
        free(m_BigBlock);
    }

    PoolAllocator * MemoryManager::getAllocator(int aSize)
    {
        if(inBounds(aSize,1,8))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_8];
        }
        else if(inBounds(aSize,9,16))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_16];
        }
        else if(inBounds(aSize,17,32))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_32];
        }
        else if(inBounds(aSize,33,64))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_64];
        }
        else if(inBounds(aSize,65,128))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_128];
        }
        else if(inBounds(aSize,129,256))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_256];
        }
        else if(inBounds(aSize,257,512))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_512];
        }
        else if(inBounds(aSize,513,1024))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_1024];
        }
        else if(inBounds(aSize,1025,Memory::BIG_BLOCK_SIZE))
        {
            return m_Allocator[(int)Memory::Allocator::BLOCK_BIG];
        }
        return 0;
    }

    using namespace Reflection;

    Type MemoryManager::getType()
    {
        return TypeFactory::create("Memory_Manager",TypeID::MEMORY_MANAGER,sizeof(MemoryManager));
    }
    Type MemoryManager::baseType()
    {
        return TypeFactory::create("Object",TypeID::OBJECT,sizeof(Object));
    }
    Type * MemoryManager::instanceOf(int & aCount)
    {
        int prevSize = 0;
        Type * prevTypes = Object::instanceOf(prevSize);
        aCount = prevSize + 1;
        char ** names = new char * [1];
        int * typeIDs = new int[1];
        int * sizes = new int[1];

        names[0] = "Object";
        typeIDs[0] = TypeID::OBJECT;
        sizes[0] = sizeof(Object);

        Type * types = TypeFactory::create(names,typeIDs,sizes,aCount,prevTypes,prevSize);

        delete[]names;
        delete[]typeIDs;
        delete[]sizes;
        return types; 
    }
}