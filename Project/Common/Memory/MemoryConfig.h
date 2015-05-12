#ifndef GAME_ENGINE_MEMORY_CONFIG_H
#define GAME_ENGINE_MEMORY_CONFIG_H


namespace Gem
{
	namespace Config
	{
		static const int KILO_BYTE = 1024;
		static const int MEGA_BYTE = 1024 * 1024;
		static const int GIGA_BYTE = 1024 * 1024 * 1024;
			
		namespace Memory
		{
#define CONFIG_MEMORY_DEBUG			///Better perforamnce, Less conditionals are used in the memory module.
#define CONFIG_MEMROY_ASSERT_CRITICAL	///Assert for critical errors
#define CONFIG_MEMORY_ASSERT			///Assert for non-critical errors
#define CONFIG_MEMORY_TRACK			///Tracks memory allocations - Turn off for performance boost
#define CONFIG_MEMORY_VERBOSE_LOGGING	///If defined debug logs for the memory module will be made. - Turn off for performance boost.
			

			//Changes made to the pool block sizes / count should be reflected in the MemoryUtils ConvertSizeToIndex, ConvertSizeToPoolSize, and GetCorrectObjectInfo functions

			static const char DEFAULT_OBJECT_ALIGNMENT = __alignof(unsigned long);
			static const int DYNAMIC_ALLOCATOR_COUNT = 255;
			static const int DEFAULT_POOL_BLOCK_COUNT = 16;
			static const int DEFAULT_POOL_BLOCK_SIZES[]
			{
				4,
                8,
				12,
                16,
				20,
                24,
				28,
				32,
                48,
                64,
				96,
				128,
				256,
				512,
				1024,
                2048
			};

			static const int DEFAULT_POOL_ALLOC_SIZES[]
			{
				1 * MEGA_BYTE, //4
				1 * MEGA_BYTE, //8
				1 * MEGA_BYTE, //12
				1 * MEGA_BYTE, //16
				1 * MEGA_BYTE, //20
				1 * MEGA_BYTE, //24
				1 * MEGA_BYTE, //28
				1 * MEGA_BYTE, //32
				1 * MEGA_BYTE, //48
				1 * MEGA_BYTE, //64
				1 * MEGA_BYTE, //96
				1 * MEGA_BYTE, //128
                1 * MEGA_BYTE, //256
                1 * MEGA_BYTE, //512
                1 * MEGA_BYTE, //1024
                1 * MEGA_BYTE  //2048
			};
			static const int DEFAULT_STACK_ALLOC_SIZE = 513 * MEGA_BYTE;
			static const int DEFAULT_FRAME_ALLOC_SIZE = 513 * MEGA_BYTE;
			
			static const int DYNAMIC_POOL_ALLOC_SIZES[]
			{
                1 * MEGA_BYTE, //4
                1 * MEGA_BYTE, //8
                1 * MEGA_BYTE, //12
                1 * MEGA_BYTE, //16
                1 * MEGA_BYTE, //20
                1 * MEGA_BYTE, //24
                1 * MEGA_BYTE, //28
                1 * MEGA_BYTE, //32
                1 * MEGA_BYTE, //48
                1 * MEGA_BYTE, //64
                1 * MEGA_BYTE, //96
                1 * MEGA_BYTE, //128
                1 * MEGA_BYTE, //256
                1 * MEGA_BYTE, //512
                1 * MEGA_BYTE, //1024
                1 * MEGA_BYTE  //2048
			};
			static const int DYNAMIC_STACK_ALLOC_SIZE = 1 * KILO_BYTE;
			static const int DYNAMIC_FRAME_ALLOC_SIZE = 1 * KILO_BYTE;
		}
	}
}

#endif