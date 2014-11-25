#include "G_Allocator.h"
#include "../Reflection/G_Primitive.h"
#include "../Reflection/G_Type.h"
#include "../Primitives/G_integer.h"
#include "../Primitives/G_string.h"
#include "../Reflection/G_ReflectionOperators.h"
#include <new.h>



//Returns a pointer to the next alligned memory address
inline void * NextAlignedAddress(void* aAddress, unsigned char aAlignment)
{
    //return (void*)( ( (uintptr_t)aAddress + (aAlignment-1) ) & ~(aAlignment -1) );
    return (void*)((reinterpret_cast<uintptr_t>(aAddress)+static_cast<uintptr_t>(aAlignment - 1)) & static_cast<uintptr_t>(~(aAlignment - 1)));
}

inline unsigned char AlignAdjustment(void * aAddress, unsigned char aAlignment)
{
    unsigned char adjustment = aAlignment - ( reinterpret_cast<uintptr_t>(aAddress) & static_cast<uintptr_t>(aAlignment -1) );
        
    if(adjustment == aAlignment)
    {
        return 0;
    }
    return adjustment;
}

inline unsigned char AlignAdjustmentWithHeader(void* aAddress, unsigned char aAlignment, unsigned char aHeaderSize)
{
    //unsigned char adjustment = aAlignment - ( (uintptr_t)aAddress & ( aAlignment-1) );
    unsigned char adjustment = AlignAdjustment(aAddress, aAlignment);

    unsigned char neededSpace = aHeaderSize;

    if(adjustment < neededSpace)
    {
        neededSpace -= adjustment;

        adjustment += aAlignment * (neededSpace / aAlignment);

        if(neededSpace % aAlignment > 0)
        {
            adjustment += aAlignment;
        }
    }

    return adjustment;
}

namespace Gem
{
	Allocator::Allocator()
	{
		m_ObjectSize = 0;
		m_ObjectAlignment = 0;
		m_NumberOfObjects = 0;
		m_Allocations = 0;
		m_UsedMemory = 0;
		m_TotalSize = 0;
		m_FreeList = nullptr;
	}
    Allocator::Allocator(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void* aMem)
        :m_ObjectSize(aObjectSize), m_ObjectAlignment(aObjectAlignment), m_TotalSize(aSize)
    {
        m_Allocations = 0;
        m_UsedMemory = 0;
        ASSERT(aObjectSize >= sizeof(void*));

        unsigned char adjustment = AlignAdjustment(aMem, m_ObjectAlignment);

        m_FreeList = (void**)((uintptr_t)aMem + adjustment);

        m_NumberOfObjects = (aSize - adjustment) / aObjectSize;

        void ** ptr = m_FreeList;

        for (unsigned int i = 0; i < m_NumberOfObjects - 1; i++)
        {
            *ptr = (void*)((uintptr_t)ptr + aObjectSize);
            ptr = (void**)*ptr;
        }

        *ptr = nullptr;
    }
    Allocator::~Allocator()
    {
        m_FreeList = nullptr;
    }
	void Allocator::Init(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void* aMem)
	{
		if (m_FreeList != nullptr)
		{
			return;
		}
		m_Allocations = 0;
		m_UsedMemory = 0;
		m_ObjectAlignment = aObjectAlignment;
		m_ObjectSize = aObjectSize;
		m_TotalSize = aSize;
		ASSERT(aObjectSize >= sizeof(void*));

		unsigned char adjustment = AlignAdjustment(aMem, m_ObjectAlignment);

		m_FreeList = (void**)((uintptr_t)aMem + adjustment);

		m_NumberOfObjects = (aSize - adjustment) / aObjectSize;

		void ** ptr = m_FreeList;

		for (unsigned int i = 0; i < m_NumberOfObjects - 1; i++)
		{
			*ptr = (void*)((uintptr_t)ptr + aObjectSize);
			ptr = (void**)*ptr;
		}

		*ptr = nullptr;
	}
    Reflection::Primitive * Allocator::Allocate(string & aTypeName)
    {
        ASSERT(m_FreeList != nullptr);
        if (m_FreeList == nullptr)
        {
            return nullptr;
        }

        void * ptr = m_FreeList;

        m_FreeList = (void**)(*m_FreeList);

        m_UsedMemory += m_ObjectSize;
        m_Allocations++;


        //Make call to reflection system to create type for typeName
        ptr = Reflection::Hidden::createPrimitive((Reflection::Primitive*)ptr, aTypeName);

        return (Reflection::Primitive*)ptr;
    }
    Reflection::Primitive * Allocator::Allocate(integer & aTypeID)
    {
        ASSERT(m_FreeList != nullptr);
        if (m_FreeList == nullptr)
        {
            return nullptr;
        }

        void * ptr = m_FreeList;

        m_FreeList = (void**)(*m_FreeList);

        m_UsedMemory += m_ObjectSize;
        m_Allocations++;

        //Make call to reflection system to create type for typeID
        ptr = Reflection::Hidden::createPrimitive((Reflection::Primitive*)ptr, aTypeID);

        return (Reflection::Primitive*)ptr;
    }
    Reflection::Primitive * Allocator::Allocate(Reflection::Type * aType)
    {
        ASSERT(m_FreeList != nullptr);
        if (m_FreeList == nullptr || aType == nullptr)
        {
            return nullptr;
        }

        void * ptr = m_FreeList;

        m_FreeList = (void**)(*m_FreeList);

        m_UsedMemory += m_ObjectSize;
        m_Allocations++;

        //Make call to reflection system to create type for type
        aType->invokeCreate((Reflection::Primitive*)ptr);

        return (Reflection::Primitive*)ptr;
    }
    Reflection::Primitive * Allocator::Deallocate(Reflection::Primitive * aPtr, string & aTypeName)
    {
        ASSERT(aPtr != nullptr);
        if (aPtr == nullptr)
        {
            return nullptr;
        }

        aPtr = Reflection::Hidden::DestroyPrimitive((Reflection::Primitive*)aPtr, aTypeName);

        *((void**)aPtr) = m_FreeList;
        m_FreeList = (void**)aPtr;

        m_UsedMemory -= m_ObjectSize;
        m_Allocations--;
        return nullptr;
    }
    Reflection::Primitive * Allocator::Deallocate(Reflection::Primitive * aPtr, integer & aTypeID)
    {
        ASSERT(aPtr != nullptr);
        if (aPtr == nullptr)
        {
            return nullptr;
        }

        aPtr = Reflection::Hidden::DestroyPrimitive((Reflection::Primitive*)aPtr, aTypeID);

        *((void**)aPtr) = m_FreeList;
        m_FreeList = (void**)aPtr;

        m_UsedMemory -= m_ObjectSize;
        m_Allocations--;
        return nullptr;
    }
    Reflection::Primitive * Allocator::Deallocate(Reflection::Primitive * aPtr, Reflection::Type * aType)
    {
        ASSERT(aPtr != nullptr);
        if (aPtr == nullptr || aType == nullptr)
        {
            return nullptr;
        }

        aPtr = aType->invokeDestroy(aType);

        *((void**)aPtr) = m_FreeList;
        m_FreeList = (void**)aPtr;

        m_UsedMemory -= m_ObjectSize;
        m_Allocations--;
        return nullptr;
    }
}