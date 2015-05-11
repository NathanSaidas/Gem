#ifndef GAME_ENGINE_MEMORY_FLAGS_H
#define GAME_ENGINE_MEMORY_FLAGS_H

namespace Engine
{
    namespace Memory
    {
        class MemoryFlags
        {
        public:
            static const int INVALID = 0;
            static const int VALID = 1;
            static const int GC_FLAGGED = 2;
            static const int DYNAMIC = 4;
            static const int STATIC = 8;
            static const int STACK = 16;
            static const int POOL = 32;
            static const int FRAME = 64;
        };
    }

}

#endif