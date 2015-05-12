#include "MemoryHeader.h"

namespace Gem
{
    namespace Memory
    {
        
        /// <summary>
        /// Sets all the bit values to 0.
        /// </summary>
        void MemoryHeader::Clear()
        {
            mask = 0;
        }
        
        /// <summary>
        /// Writes to the memory header.
        /// </summary>
        /// <param name="aFlags">The flags of the memory allocated.</param>
        /// <param name="aID">The ID of the allocator that allocated the memory.</param>
        /// <param name="aSize">The size of the object allocated.</param>
        void MemoryHeader::Write(const UInt8 & aFlags, const UInt8 & aID, const UInt16 & aSize)
        {
            bytes[MemoryHeader::FLAGS] = aFlags;
            bytes[MemoryHeader::ID_OFFSET] = aID;
            bytes[MemoryHeader::SIZE] = (UInt8)aSize;
            bytes[MemoryHeader::SIZE + 1] = (UInt8)(aSize >> 8);
        }

        /// <summary>
        /// Reads the memory header values into the given arguments.
        /// </summary>
        /// <param name="aFlags">The flags of the memory allocated.</param>
        /// <param name="aID">The ID of the allocator that allocated the memory.</param>
        /// <param name="aSize">The size of the object allocated.</param>
        void MemoryHeader::Read(UInt8 & aFlags, UInt8 & aID, UInt16 & aSize)
        {
            aFlags = bytes[MemoryHeader::FLAGS];
            aID = bytes[MemoryHeader::ID_OFFSET];
            aSize = (UInt16)bytes[MemoryHeader::SIZE + 1] << 8;
            aSize |= bytes[MemoryHeader::SIZE];
        }
    }
}