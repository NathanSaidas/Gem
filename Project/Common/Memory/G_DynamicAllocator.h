#ifndef G_DYNAMIC_ALLOCATOR_H
#define G_DYNAMIC_ALLOCATOR_H

#include "G_Allocator.h"

namespace Gem
{
	///Takes
	class DynamicAllocator : public Allocator
	{
	public:
		DynamicAllocator();
		DynamicAllocator(int aID,unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize);
		~DynamicAllocator();

		void Init(int aID, unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize);

		inline int GetID()
		{
			return m_ID;
		}
	private:
		int m_ID;
		void * m_Memory;
	};
}

#endif