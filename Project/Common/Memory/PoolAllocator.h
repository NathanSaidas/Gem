#ifndef GAME_ENGINE_POOL_ALLOCATOR_H
#define GAME_ENGINE_POOL_ALLOCATOR_H

#pragma region CHANGE LOG
/// -- January, 31, 2015 - Nathan Hanlan - Added StackAllocator class/file
/// -- February 23  2015 - Nathan Hanlan - Added additional comments, added preprocessor ifdefs from the MemoryConfig.h
/// -- February 23  2015 - Nathan Hanlan - Added additioanl debug logging and a test case. See source code below the class definition.
/// -- February 23  2015 - Nathan Hanlan - Renamed HasMemory to CanAlloc and provided an additional method OutOfMemory
#pragma endregion

#include "Allocator.h"

namespace Gem
{
	namespace Memory
    {
        /// <summary>
        /// The PoolAllocator is designed to make small allocations of a fixed size/alignment.
        /// Unlike the FrameAllocator the memory allocated by the pool stays until it is deallocated or the PoolAllocator is destroyed.
        ///
        /// Useful methods.
        /// • Allocate
        /// • Deallocate
        /// </summary>
        class PoolAllocator : public Allocator
        {
        public:
            /// <summary>
            /// Initializes the PoolAllocator class. Allocates memory and sets up the linked list of pointers.
            /// </summary>
            /// <param name="aSize">The total size of the memory allocation</param>
            /// <param name="aID">The ID of the allocator</param>
            /// <param name="aObjectSize">The max size of the objects that can be allocated.</param>
            /// <param name="aObjectAlignment">The alignment of the objects</param>
            PoolAllocator(UInt32 aSize, UInt8 aID, UInt32 aObjectSize, UInt8 aObjectAlignment);

            /// <summary>
            /// Finalizes an instance of this class and free all of the memory;
            /// </summary>
            ~PoolAllocator();

            /// <summary>
            /// Allocates an object of the given size with the given alignment.        
            ///
            /// For an allocation to work the method must.
            /// • Be passed a size and alignment that matches m_ObjectSize and m_ObjectAlignment
            /// • The allocator must have enough memory
            /// </summary>
            /// <param name="aSize">The size of the object to allocate</param>
            /// <param name="aAlignment">The alignment of the object to allocate</param>
            /// <returns>Returns a valid pointer if the allocation succeeded. Returns a nullptr if the allocation failed.</returns>
            void * Allocate(UInt32 aSize, UInt8 aAlignment);

            /// <summary>
            /// Deallocates the specified address. 
            /// </summary>
            /// <param name="aAddress">The address to deallocate</param>
            /// <returns>Returns a nullptr if the deallocation was successful, returns the address that was sent in if the deallocation failed.</returns>
            void * Deallocate(void * aAddress);

            /// <summary>
            /// Returns the type of allocator this is. (Pool)
            /// </summary>
            /// <returns>Returns the type of allocator this is. (Pool)</returns>
            AllocatorType GetAllocatorType();

            /// <summary>
            /// Returns the size of the objects that get allocated.
            /// </summary>
            /// <returns>Returns the size of the objects that get allocated </returns>
            UInt32 GetObjectSize();

            /// <summary>
            /// Returns the alignment of the objects that get allocated
            /// </summary>
            /// <returns>Returns the alignment of the objects that get allocated</returns>
            UInt8 GetObjectAlignment();

            /// <summary>
            /// Determines whether this instance can allocate the specified object size.
            ///
            /// Does a Alignment calculation to determine the alignment
            /// </summary>
            /// <param name="aObjectSize">Size of the object.</param>
            /// <param name="aObjectAlignment">Alignment of the object.</param>
            /// <returns>True if the allocation is possible, false if it is not possible.</returns>
            bool CanAlloc(UInt32 aObjectSize, UInt8 aObjectAlignment);

            /// <summary>
            /// Determines whether this instance can allocate the specified object size.
            ///
            /// Does not perform an Alignment calculation. Assume aObjectSize includes the adjustment
            /// </summary>
            /// <param name="aObjectSize">Size of a object and the adjustment.</param>
            /// <returns>True if the allocation is possible, false if it is not possible.</return>
            bool CanAlloc(UInt32 aObjectSize);
            /// <summary>
            /// Returns whether or not this allocator is out of memory.
            /// </summary>
            /// <returns>Returns whether or not this allocator is out of memory.</returns>
            bool OutOfMemory();
        private:
            /// <summary>
            /// The maximum size of the objects that get allocated.
            /// </summary>
            UInt32 m_ObjectSize; 
            /// <summary>
            /// The allignment of the objects that get allocated.
            /// </summary>
            UInt8 m_ObjectAlignment;
            /// <summary>
            /// The current position of the linked list.
            /// </summary>
            void ** m_CurrentPosition;
        };

        //----------------------------------------------
        // PoolAllocator Test / Example
        //----------------------------------------------
        //PoolAllocator * allocator = new PoolAllocator(1024, 2, sizeof(Data16), __alignof(Data16));
        //
        //Data16 * a = (Data16*)allocator->Allocate(sizeof(Data16), __alignof(Data16));
        //Data16 * b = (Data16*)allocator->Allocate(sizeof(Data16), __alignof(Data16));
        //Data16 * c = (Data16*)allocator->Allocate(sizeof(Data16), __alignof(Data16));
        //
        //a->Test();
        //b->Test();
        //c->Test();
        //
        //a = (Data16*)allocator->Deallocate(a);
        //b = (Data16*)allocator->Deallocate(b);
        //c = (Data16*)allocator->Deallocate(c);
        //
        //delete allocator;

    }


}

#endif