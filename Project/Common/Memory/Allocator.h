#ifndef GAME_ENGINE_ALLOCATOR_H
#define GAME_ENGINE_ALLOCATOR_H

#pragma region CHANGE LOG
/// -- January, 31, 2015 - Nathan Hanlan - Added Allocator class/file
#pragma endregion

#include "../Core/ValueTypes.h"
#include "../Core/GemAPI.h"
#include "AllocatorType.h"
#include "MemoryInfo.h"

namespace Gem
{
    namespace Memory
    {
        /// <summary>
        /// The abstract base class for all memory allocated.
        ///
        /// Abstract Methods to Override.
        /// • Allocate
        /// • Deallocate
        /// • GetAllocatorType
        /// • CanAlloc
        /// • OutOfMemory
        /// </summary>
        class GEM_API Allocator
        {
        public:
            /// <summary>
            /// The Allocator constructor that assigns the ID to the allocator and sets the default memory usage to 0
            /// </summary>
            /// <param name="aID">The unique ID of the allocator.</param>
            Allocator(UInt8 aID);

            /// <summary>
            /// Finalizes an instance of the <see cref="Allocator"/> class.
            /// </summary>
            virtual ~Allocator();

            /// <summary>
            /// Gets the memory address allocated.
            /// </summary>
            /// <returns>Returns the address of the memory allocated.</returns>
            void * GetMemory();

            /// <summary>
            /// Gets the size of the allocation.
            /// </summary>
            /// <returns>Returns the size of the allocation.</returns>
            UInt32 GetSize();

            /// <summary>
            /// Gets the id of the allocator..
            /// </summary>
            /// <returns>Returns the ID of this allocator.</returns>
            UInt8 GetID();

            /// <summary>
            /// Gets the memory information of the allocator.
            /// </summary>
            /// <returns>Returns the memory information about the allocator.</returns>
            MemoryInfo GetInfo();

            /// <summary>
            /// Flags an allocator to be force destroyed. This will ignore memory leak error catching/reporting.
            /// </summary>
            void FlagForceDestroy();

            /// <summary>
            /// Allocates memory from the allocator.
            /// </summary>
            /// <param name="aSize">The size of the allocation.</param>
            /// <param name="aAlignment">The allignment of the allocation.</param>
            /// <returns>Returns a pointer to the memory allocated or nullptr if it failed to allocate.</returns>
            virtual void * Allocate(UInt32 aSize, UInt8 aAlignment) = 0;

            /// <summary>
            /// Deallocates memory from the allocator.
            /// </summary>
            /// <param name="aAddress">The address to deallocate</param>
            /// <returns>Returns a nullptr if the allocation was successful or returns the aAddress pointer if the allocation was not successful.</returns>
            virtual void * Deallocate(void * aAddress) = 0;

            /// <summary>
            /// Gets the type of allocator.
            /// </summary>
            /// <returns>Returns the type of allocator.</returns>
            virtual AllocatorType GetAllocatorType() = 0;

            /// <summary>
            /// Determines whether this instance can allocate the specified object size.
            ///
            /// Does a Alignment calculation to determine the alignment
            /// </summary>
            /// <param name="aObjectSize">Size of the object.</param>
            /// <param name="aObjectAlignment">Alignment of the object.</param>
            /// <returns>True if the allocation is possible, false if it is not possible.</returns>
            virtual bool CanAlloc(UInt32 aSize, UInt8 aAlignment) = 0;

            /// <summary>
            /// Determines whether this instance can allocate the specified object size.
            ///
            /// Does not perform an Alignment calculation. Assume aObjectSize includes the adjustment
            /// </summary>
            /// <param name="aObjectSize">Size of a object and the adjustment.</param>
            /// <returns>True if the allocation is possible, false if it is not possible.</return>
            virtual bool CanAlloc(UInt32 aSize) = 0;

            /// <summary>
            /// Returns whether or not this allocator is out of memory.
            /// </summary>
            /// <returns>Returns whether or not this allocator is out of memory.</returns>
            virtual bool OutOfMemory() = 0;


            

        protected:
            /// <summary>
            /// The address of the memory allocated.
            /// </summary>
            void * m_Memory;

            /// <summary>
            /// The size of the allocation made.
            /// </summary>
            UInt32 m_Size;

            /// <summary>
            /// The ID of the allocator.
            /// </summary>
            UInt8 m_ID;

            /// <summary>
            /// The information about the allocator.
            /// </summary>
            MemoryInfo m_MemoryInfo;

            /// <summary>
            /// This is used to let the allocator know that it was intended to be destroyed and should ignore potential memory leaks.
            /// </summary>
            bool m_ForceDestroy;
        };
    }

}
#pragma endregion

#endif