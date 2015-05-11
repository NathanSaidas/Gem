#ifndef GAME_ENGINE_FRAME_ALLOCATOR_H
#define GAME_ENGINE_FRAME_ALLOCATOR_H

#pragma region CHANGE LOG
/// -- January, 31, 2015 - Nathan Hanlan - Added FrameAllocator class/file
/// -- February 23  2015 - Nathan Hanlan - Added additional comments, added preprocessor ifdefs from the MemoryConfig.h
/// -- February 23  2015 - Nathan Hanlan - Added additioanl debug logging and a test case. See source code below the class definition.
/// -- February 23  2015 - Nathan Hanlan - Renamed HasMemory to CanAlloc and provided an additional method OutOfMemory
#pragma endregion

#include "Allocator.h"

namespace Engine
{
    namespace Memory
    {
        /// <summary>
        /// The FrameAllocator is designed to make allocations based on the single frame.
        /// It allows any size / any alignment object to be allocated
        ///
        /// Useful methods.
        /// • Allocate
        /// • Reset
        /// </summary>
        class FrameAllocator : public Allocator
        {
        public:
            /// <summary>
            /// Initializes a new instance of the Frame Allocator. 
            /// Allocates the memory required for allocations from the Allocator.
            /// </summary>
            /// <param name="aMemorySize">Size of the memory to allocate and manage.</param>
            /// <param name="aID">The ID of the allocator</param>
            FrameAllocator(UInt32 aMemorySize, UInt8 aID);

            /// <summary>
            /// Finalizes an instance of the class, free the memory that has been allocated.
            /// </summary>
            ~FrameAllocator();

            /// <summary>
            /// Allocates the specified a size with the specified alignment.
            /// 
            /// The size must be greater than 0
            /// There must be enough memory available in order to allocate.
            /// </summary>
            /// <param name="aSize">The size of the object.</param>
            /// <param name="aAlignment">The alignment of the object.</param>
            /// <returns>Returns a valid pointer that has been allocated or a nullptr if there was a problem allocating.</returns>
            void * Allocate(UInt32 aSize, UInt8 aAlignment);

            /// <summary>
            /// Frame allocators dont deallocate memory. This is just stubbed out to be an Allocator.
            /// Use the Reset method to reset the frame back to the start.
            /// </summary>
            /// <param name="aAddress">The addressto deallocate</param>
            /// <returns>Returns the address passed in.</returns>
            void * Deallocate(void * aAddress);

            /// <summary>
            /// Gets the type of allocator.
            /// </summary>
            /// <returns>Returns the type of the allocator </returns>
            AllocatorType GetAllocatorType();

            /// <summary>
            /// Resets the frame allocator back to the start.
            /// </summary>
            void Reset();

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
            /// The current position of the frame allocator pointer.
            /// </summary>
            void * m_CurrentPosition;
        };

        //-------------------------------
        // FrameAllocator Test/Example
        //-------------------------------
        //FrameAllocator * allocator = new FrameAllocator(1024, 8);
        //
        //Data16 * a = (Data16*)allocator->Allocate(sizeof(Data16), __alignof(Data16));
        //Data24 * b = (Data24*)allocator->Allocate(sizeof(Data24), __alignof(Data24));
        //Data512 * c = (Data512*)allocator->Allocate(sizeof(Data512), __alignof(Data512));
        //
        //a->Test();
        //b->Test();
        //c->Test();
        //
        //allocator->Reset();
        //
        //delete allocator;
    }
	
}


#endif