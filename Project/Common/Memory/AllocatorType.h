#ifndef GAME_ENGINE_ALLOCATOR_TYPE_H
#define GAME_ENGINE_ALLOCATOR_TYPE_H

#pragma region CHANGE LOG
/// -- January, 31, 2015 - Nathan Hanlan - Added AllocatorType enum/file
#pragma endregion

namespace Engine
{
	namespace Memory
	{
		enum class AllocatorType
		{
			Frame,
			Stack,
			Pool
		};
	}
}

#endif