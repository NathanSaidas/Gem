#ifndef G_MEMORY_UTILS_H
#define G_MEMORY_UTILS_H

#include "G_MemoryDefinitions.h"
#include <stdlib.h>

namespace Gem
{
	namespace Memory
	{
		

		///Calculates the next address of a pointer based on alignment.
		inline void * NextAlignedAddress(void * aAddress, unsigned char aAlignment)
		{
			return (void*)((reinterpret_cast<uintptr_t>(aAddress)+static_cast<uintptr_t>(aAlignment - 1)) & static_cast<uintptr_t>(~(aAlignment - 1)));
		}
		///Calculates the required adjustment of a pointer based on alignment.
		inline unsigned char AlignAdjustment(void * aAddress, unsigned char aAlignment)
		{
			unsigned char adjustment = aAlignment - (reinterpret_cast<uintptr_t>(aAddress)& static_cast<uintptr_t>(aAlignment - 1));
			if (adjustment == aAlignment)
			{
				return 0;
			}
			return adjustment;
		}

		///Returns an index to a memory block allocation based on an objects size.
		inline unsigned int SizeToBlockIndex(unsigned int aSize)
		{
			if (aSize >= 0 && aSize <= 8)
			{
				return 0;
			}
			else if (aSize >= 9 && aSize <= 12)
			{
				return 1;
			}
			else if (aSize >= 13 && aSize <= 16)
			{
				return 2;
			}
			else if (aSize >= 17 && aSize <= 24)
			{
				return 3;
			}
			else if (aSize >= 25 && aSize <= 32)
			{
				return 4;
			}
			else if (aSize >= 33 && aSize <= 48)
			{
				return 5;
			}
			else if (aSize >= 49 && aSize <= 64)
			{
				return 6;
			}
			else if (aSize >= 65 && aSize <= 96)
			{
				return 7;
			}
			else if (aSize >= 97 && aSize <= 128)
			{
				return 8;
			}
			else if (aSize >= 129 && aSize <= 256)
			{
				return 9;
			}
			else if (aSize >= 257 && aSize <= 512)
			{
				return 10;
			}
			else if (aSize >= 513 && aSize <= 1024)
			{
				return 11;
			}
			return -1;
		}
		inline MemoryBlock SizeToBlock(unsigned int aSize)
		{
			if (aSize >= 0 && aSize <= 8)
			{
				return MemoryBlock::Block8;
			}
			else if (aSize >= 9 && aSize <= 12)
			{
				return MemoryBlock::Block12;
			}
			else if (aSize >= 13 && aSize <= 16)
			{
				return MemoryBlock::Block16;
			}
			else if (aSize >= 17 && aSize <= 24)
			{
				return MemoryBlock::Block24;
			}
			else if (aSize >= 25 && aSize <= 32)
			{
				return MemoryBlock::Block32;
			}
			else if (aSize >= 33 && aSize <= 48)
			{
				return MemoryBlock::Block48;
			}
			else if (aSize >= 49 && aSize <= 64)
			{
				return MemoryBlock::Block64;
			}
			else if (aSize >= 65 && aSize <= 96)
			{
				return MemoryBlock::Block96;
			}
			else if (aSize >= 97 && aSize <= 128)
			{
				return MemoryBlock::Block128;
			}
			else if (aSize >= 129 && aSize <= 256)
			{
				return MemoryBlock::Block256;
			}
			else if (aSize >= 257 && aSize <= 512)
			{
				return MemoryBlock::Block512;
			}
			else if (aSize >= 513 && aSize <= 1024)
			{
				return MemoryBlock::Block1024;
			}
			return MemoryBlock::Invalid;
		}

		

		
	}
}

#endif