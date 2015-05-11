#ifndef GAME_ENGINE_MEMORY_INFO_H
#define GAME_ENGINE_MEMORY_INFO_H

#pragma region CHANGE LOG
/// -- January, 31, 2015 - Nathan Hanlan - Added MemoryInfo struct/file
#pragma endregion

#include "../ValueTypes.h"

namespace Engine
{
    namespace Memory
    {
        ///Defines information about memory usage.
        struct MemoryInfo
        {
        public:
            UInt32 allocations;
            UInt32 memoryUsed;
            UInt32 memoryAvailable;
			UInt32 totalMemory;
        };

    } /// End Memory
} /// End Engine



#endif