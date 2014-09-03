#include "G_MemoryManager.h"
#include "../Primitives/G_string.h"
#include "../Primitives/G_integer.h"
#include "../Reflection/G_ReflectionOperators.h"
#include "../Reflection/G_Type.h"
#include <malloc.h>

namespace Gem
{
    //determine if the value is between the lower and higher bounds. Inclusive
    inline bool inBounds(const float & aValue, const float & aLower, const float & aHigher)
    {
        if (aValue >= aLower && aValue <= aHigher)
        {
            return true;
        }
        return false;
    }

    inline bool inBounds(const int & aValue, const int & aLower, const int & aHigher)
    {
        if (aValue >= aLower && aValue <= aHigher)
        {
            return true;
        }
        return false;
    }


    MemoryManager * MemoryManager::s_Instance;
    MemoryManager * MemoryManager::instance()
    {
        if (s_Instance == 0)
        {
            s_Instance = new MemoryManager();
        }
        return s_Instance;
    }
    void MemoryManager::destroy()
    {
        if (s_Instance != 0)
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

        m_Allocator[(int)Memory::BlockSize::BLOCK_8] = new Allocator(8, 8, Memory::BLOCK_8_ALLOC_SIZE, m_Block8);
        m_Allocator[(int)Memory::BlockSize::BLOCK_16] = new Allocator(16, 8, Memory::BLOCK_16_ALLOC_SIZE, m_Block16);
        m_Allocator[(int)Memory::BlockSize::BLOCK_32] = new Allocator(32, 8, Memory::BLOCK_32_ALLOC_SIZE, m_Block32);
        m_Allocator[(int)Memory::BlockSize::BLOCK_64] = new Allocator(64, 8, Memory::BLOCK_64_ALLOC_SIZE, m_Block64);
        m_Allocator[(int)Memory::BlockSize::BLOCK_128] = new Allocator(128, 8, Memory::BLOCK_16_ALLOC_SIZE, m_Block128);
        m_Allocator[(int)Memory::BlockSize::BLOCK_256] = new Allocator(256, 8, Memory::BLOCK_256_ALLOC_SIZE, m_Block256);
        m_Allocator[(int)Memory::BlockSize::BLOCK_512] = new Allocator(512, 8, Memory::BLOCK_512_ALLOC_SIZE, m_Block512);
        m_Allocator[(int)Memory::BlockSize::BLOCK_1024] = new Allocator(1024, 8, Memory::BLOCK_1024_ALLOC_SIZE, m_Block1024);
        m_Allocator[(int)Memory::BlockSize::BLOCK_BIG] = new Allocator(Memory::BIG_BLOCK_SIZE, 8, Memory::BIG_BLOCK_ALLOC_SIZE, m_BigBlock);
    }
    MemoryManager::~MemoryManager()
    {
        for (int i = 0; i < (int)Memory::BlockSize::BLOCK_COUNT; i++)
        {
            if (m_Allocator[i] != 0)
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
    Reflection::Primitive * MemoryManager::allocate(string & aTypename)
    {
        Pointer<Reflection::Type> type = typeOf(aTypename);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return allocate(type.ref());
    }
    Reflection::Primitive * MemoryManager::allocate(integer & aTypeID)
    {
        Pointer<Reflection::Type> type = typeOf(aTypeID);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return allocate(type.ref());
    }
    Reflection::Primitive * MemoryManager::allocate(Reflection::Type * aType)
    {
        if (aType == nullptr)
        {
            return nullptr;
        }

        Allocator * allocator = getAllocator(aType->size());
        if (allocator == nullptr)
        {
            return nullptr;
        }
        return allocator->allocate(aType);
    }
    Reflection::Primitive * MemoryManager::deallocate(Reflection::Primitive * aPtr, string & aTypename)
    {
        Pointer<Reflection::Type> type = typeOf(aTypename);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return deallocate(aPtr,type.ref());
    }
    Reflection::Primitive * MemoryManager::deallocate(Reflection::Primitive * aPtr, integer & aTypeID)
    {
        Pointer<Reflection::Type> type = typeOf(aTypeID);
        if (type.isAlive() == false)
        {
            return nullptr;
        }
        return deallocate(aPtr,type.ref());
    }
    Reflection::Primitive * MemoryManager::deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType)
    {
        if (aType == nullptr)
        {
            return aPtr;
        }
        Allocator * allocator = getAllocator(aType->size());
        if (allocator == nullptr)
        {
            return aPtr;
        }
        return allocator->deallocate(aPtr);
    }

    Allocator * MemoryManager::getAllocator(int aSize)
    {
        if (inBounds(aSize, 1, 8))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_8];
        }
        else if (inBounds(aSize, 9, 16))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_16];
        }
        else if (inBounds(aSize, 17, 32))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_32];
        }
        else if (inBounds(aSize, 33, 64))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_64];
        }
        else if (inBounds(aSize, 65, 128))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_128];
        }
        else if (inBounds(aSize, 129, 256))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_256];
        }
        else if (inBounds(aSize, 257, 512))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_512];
        }
        else if (inBounds(aSize, 513, 1024))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_1024];
        }
        else if (inBounds(aSize, 1025, Memory::BIG_BLOCK_SIZE))
        {
            return m_Allocator[(int)Memory::BlockSize::BLOCK_BIG];
        }
        return 0;
    }

    namespace Memory
    {
        Reflection::Primitive * instantiate(const char * aTypename)
        {
            return MemoryManager::instance()->allocate(string(aTypename));
        }
        Reflection::Primitive * instantiate(string & aTypename)
        {
            return MemoryManager::instance()->allocate(aTypename);
        }
        Reflection::Primitive * instantiate(integer & aTypeID)
        {
            return MemoryManager::instance()->allocate(aTypeID);
        }
        Reflection::Primitive * instantiate(Reflection::Type * aType)
        {
            return MemoryManager::instance()->allocate(aType);
        }
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, const char * aTypename)
        {
            return MemoryManager::instance()->deallocate(aAddress, string(aTypename));
        }
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, string & aTypename)
        {
            return MemoryManager::instance()->deallocate(aAddress, aTypename);
        }
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, integer & aTypeID)
        {
            return MemoryManager::instance()->deallocate(aAddress, aTypeID);
        }
        Reflection::Primitive * destroy(Reflection::Primitive * aAddress, Reflection::Type * aType)
        {
            return MemoryManager::instance()->deallocate(aAddress, aType);
        }
    }
}