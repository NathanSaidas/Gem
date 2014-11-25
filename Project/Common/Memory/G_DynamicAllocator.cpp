#include "G_DynamicAllocator.h"

namespace Gem
{
	DynamicAllocator::DynamicAllocator()
	{
		m_ID = -1;
		m_Memory = nullptr;
	}
	DynamicAllocator::DynamicAllocator(int aID, unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize)
		: Allocator()
	{
		m_ID = aID;
		m_Memory = malloc(aSize);
		Allocator::Init(aObjectSize, aObjectAlignment, aSize, m_Memory);
	}
	DynamicAllocator::~DynamicAllocator()
	{
		free(m_Memory);
	}

	void DynamicAllocator::Init(int aID, unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize)
	{
		if (m_ID == -1)
		{
			m_ID = aID;
			if (m_Memory == nullptr)
			{
				m_Memory = malloc(aSize);
				Allocator::Init(aObjectSize, aObjectAlignment, aSize,m_Memory);
			}
		}
		
	}
}