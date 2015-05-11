#ifndef G_MEMORY_DEFINITIONS_H
#define G_MEMORY_DEFINITIONS_H

#include "../G_Config.h"
#ifdef GEM_REFLECTION
#include "../Reflection/G_Reflection.h"
#endif

#pragma region CHANGE LOG
/* December, 22, 2014 - Nathan Hanlan - Added Reflection ifdefs for Garbage Collection
*/
#pragma endregion

namespace Gem
{


	namespace Memory
	{
		///Constants

		///Byte Defintiions
		#define KILO_BYTE 1024				//1024
		#define MEGA_BYTE 1024 * KILO_BYTE	//1048576
		#define GIGA_BYTE 1024 * MEGA_BYTE	//1073741824

		const unsigned char MEMORY_HEADER_SIZE = sizeof(int);
		const unsigned char MEMORY_ALLOCATOR_COUNT = 12;
		const unsigned char MEMORY_ALIGNMENT = 4;

		const unsigned int BLOCK_8 = 0;
		const unsigned int BLOCK_12 = 1;
		const unsigned int BLOCK_16 = 2;
		const unsigned int BLOCK_24 = 3;
		const unsigned int BLOCK_32 = 4;
		const unsigned int BLOCK_48 = 5;
		const unsigned int BLOCK_64 = 6;
		const unsigned int BLOCK_96 = 7;
		const unsigned int BLOCK_128 = 8;
		const unsigned int BLOCK_256 = 9;
		const unsigned int BLOCK_512 = 10;
		const unsigned int BLOCK_1024 = 11;

		///The size of each allocation
		const unsigned int OBJECT_SIZES[MEMORY_ALLOCATOR_COUNT] =
		{
			8,
			12,
			16,
			24,
			32,
			48,
			64,
			96,
			128,
			256,
			512,
			1024
		};

		///Initial Allocation Sizes
		const unsigned int BLOCK_ALLOC_SIZES[MEMORY_ALLOCATOR_COUNT] =
		{
			KILO_BYTE,					//Block 8
			KILO_BYTE,					//Block 12
			KILO_BYTE,					//Block 16
			KILO_BYTE,					//Block 24
			512 * MEGA_BYTE,			//Block 32
			KILO_BYTE,					//Block 48
			KILO_BYTE,					//Block 64
			KILO_BYTE,					//Block 96
			KILO_BYTE,					//Block 128
			KILO_BYTE,					//Block 256
			KILO_BYTE,					//Block 512
			KILO_BYTE					//Block 1024
		};
		///Dynamic Allocation Sizes
		const unsigned int DYNAMIC_BLOCK_SIZES[MEMORY_ALLOCATOR_COUNT] =
		{
			MEGA_BYTE,						//Block 8
			MEGA_BYTE,						//Block 12
			MEGA_BYTE,						//Block 16
			MEGA_BYTE,						//Block 24
			MEGA_BYTE,						//Block 32
			MEGA_BYTE,						//Block 48
			MEGA_BYTE,						//Block 64
			MEGA_BYTE,						//Block 96
			MEGA_BYTE,						//Block 128
			MEGA_BYTE,						//Block 256
			MEGA_BYTE,						//Block 512
			MEGA_BYTE,						//Block 1024
		};

		///The types of modes available for DYNAMIC_DESTROY
		///The allocators never get destroyed
		const int DYNAMIC_DESTROY_NEVER = 0;
		///The allocators get destroyed immediately (within frame) when they are no longer needed.
		const int DYNAMIC_DESTROY_IMMEDIATE = 1;
		///The allocators get destroyed after x time of not being needed. They can be renewed when needed.
		const int DYNAMIC_DESTROY_TIME = 2;

		const int DYNAMIC_DESTROY_METHOD = DYNAMIC_DESTROY_TIME;

		///How much time a Dynamic Allocator can life for before destroyed under the DYNAMIC_DESTROY_TIME method.
		const float DYNAMIC_ALLOCATOR_LIFETIME = 30.0f;

		///Memory used within the memory management system.
		const unsigned int INTERNAL_MEMORY = 2 * KILO_BYTE;

		///Garbage Collection Default Configuration
		const float GC_DEFAULT_COLLECTION_TIME = 0.016f;
		const unsigned int GC_DEFAULT_CHUNK_SIZE = 32;
		
		///Memory Header Flags
		const int FLAG_INVALID = 0;
		const int FLAG_VALID = 1;
		const int FLAG_GC = 2;

		///Levels of error handling
#define MEMORY_ERROR_LEVEL_EXCEPTIONS
#define MEMORY_ERROR_LEVEL_ASSERT 
#define MEMORY_ERROR_LEVEL_NONE


		struct ArrayAlloc
		{
			void * address;
			unsigned int size;
		};

		///Types
		///An info struct used to read memory info from an allocator
		struct AllocatorMemInfo
		{
			unsigned int allocations;
			unsigned int maxAllocations;
			unsigned int objectSize;
		};

		struct MemoryReportInfo
		{
			unsigned int allocations;
			unsigned int maxAllocations;
		};

		struct MemoryReport
		{
			MemoryReportInfo info[MEMORY_ALLOCATOR_COUNT];
			unsigned int memoryUsed;
			unsigned int memoryAvailable;
		};

		enum class MemoryBlock
		{
			Block8,
			Block12,
			Block16,
			Block24,
			Block32,
			Block48,
			Block64,
			Block96,
			Block128,
			Block256,
			Block512,
			Block1024,
			Invalid
		};

		enum class MemoryReportType
		{
			All,
			Initial,
			Dynamic
		};

#ifdef GEM_REFLECTION
		struct GCItem
		{
			GCItem(): address(nullptr) {};
			GCItem(void * aAddress, Type & aType) : address(aAddress), type(aType) {}
			void * address;
			Type type;
		};
#endif


	}
}

#endif