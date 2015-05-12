#ifndef GAME_ENGINE_MEMORY_HEADER_H
#define GAME_ENGINE_MEMORY_HEADER_H

#pragma region CHANGE LOG
/// -- January  31  2015 - Nathan Hanlan - Added MemoryHeader struct/file
/// -- February 22  2015 - Nathan Hanlan - Changed the Layout of the MemoryHeader. Added additional helper functions.
#pragma endregion

#include "../Core/ValueTypes.h"

namespace Gem
{
    namespace Memory
    {
        /// <summary>
        /// A data structure for the 4 byte Memory Header on custom allocated objects.
        ///
        /// Use the Clear, Read, and Write methods for accessing members as opposed to accessing the bytes directly.
        /// </summary>
        struct MemoryHeader
        {
        public:
            /// <summary>
            /// The index of the mask bytes that represents the bit flags. 
            /// </summary>
            static const UInt8 FLAGS = 0;
            /// <summary>
            /// The index of the mask bytes that represents the id / offset flags.
            /// </summary>
            static const UInt8 ID_OFFSET = 1;
            /// <summary>
            /// The index of the mask bytes that represents the size flags. (Note that the size is actually 16 bits)
            /// </summary>
            static const UInt8 SIZE = 2;
            
            /// <summary>
            /// Sets all the bit values to 0.
            /// </summary>
            void Clear();

            /// <summary>
            /// Writes to the memory header.
            /// </summary>
            /// <param name="aFlags">The flags of the memory allocated.</param>
            /// <param name="aID">The ID of the allocator that allocated the memory.</param>
            /// <param name="aSize">The size of the object allocated.</param>
            void Write(const UInt8 & aFlags, const UInt8 & aID, const UInt16 & aSize);

            /// <summary>
            /// Reads the memory header values into the given arguments.
            /// </summary>
            /// <param name="aFlags">The flags of the memory allocated.</param>
            /// <param name="aID">The ID of the allocator that allocated the memory.</param>
            /// <param name="aSize">The size of the object allocated.</param>
            void Read(UInt8 & aFlags, UInt8 & aID, UInt16 & aSize);

            /// <summary>
            /// A bitmask that can be accessed an integer or bytes or words.
            /// </summary>
            union 
            {
                UInt32 mask;
                UInt16 words[2];
                UInt8 bytes[4];
            };
        };
        //--------------------
        // Use this for a test.
        //--------------------
        //MemoryHeader header;
        //header.Clear();
        //
        //{
        //    UInt8 flags = MemoryFlags::VALID | MemoryFlags::POOL;
        //    UInt8 id = 32;
        //    UInt16 size = 356;
        //
        //    header.Write(flags, id, size);
        //}
        //
        //{
        //    UInt8 flags = 0;
        //    UInt8 id = 0;
        //    UInt16 size = 0;
        //
        //    header.Read(flags, id, size);
        //}
    }
	
}

#endif