#ifndef GAME_ENGINE_STACK_ALLOCATOR_H
#define GAME_ENGINE_STACK_ALLOCATOR_H

#pragma region CHANGE LOG
/// -- January  31  2015 - Nathan Hanlan - Added StackAllocator class/file
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
        /// The StackAllocator is an allocator that provides memory in an order from the bottom to the top of the stack.
        /// Memory can be allocated from the top of the stack but it must also be deallocated in the reverse order it was allocated. (top to bottom)
        ///
        /// Useful Methods.
        /// • Allocate
        /// • Deallocate
        /// • Rollback
        /// </summary>
        class StackAllocator : public Allocator
        {
        public:

            /// <summary>
            /// Initializes an allocator and allocates memory of the given size.
            /// </summary>
            /// <param name="aSize">The size of the memory allocations</param>
            /// <param name="aID">The ID of the allocator.</param>
            StackAllocator(UInt32 aSize, UInt8 aID);

            /// <summary>
            /// Finalizes an instance of the class and deallocates the memory of the allocator.
            /// </summary>
            ~StackAllocator();

            /// <summary>
            /// Allocates an object of the specific size and alignment off the top of the stack.
            /// </summary>
            /// <param name="aSize">The size of the allocation</param>
            /// <param name="aAlignment">The alignment of the allocation</param>
            /// <returns>Returns a valid address or a nullptr if something went wrong with the allocation.</returns>
            void * Allocate(UInt32 aSize, UInt8 aAlignment);

            /// <summary>
            /// Deallocates the address given if this address was the previous allocation made.
            /// Use Rollback when rolling back to a specific address.
            /// </summary>
            /// <param name="aAddress">The previous address allocated to deallocate.</param>
            /// <returns>Returns a nullptr if the deallocation was successful, otherwise it returns the address given.</returns>
            void * Deallocate(void *aAddress);

            /// <summary>
            /// Deallocates an address with minimal security in place. Use with caution.
            /// If an invalid pointer was used to deallocate with the stack will just deallocate whatever is on the top first.
			/// </summary>
            /// <param name="aAddress">The address last allocated on the stack.</param>
            /// <returns>Returns a nullptr or the address deallocated.</returns>
            void * FastDeallocate(void * aAddress);

            /// <summary>
            /// Deallocates until null or the specified address is deallocated
            /// </summary>
            /// <param name="aAddress">The address to deallocate to.</param>
            /// <returns>Returns a nullptr.</returns>
            void * Rollback(void * aAddress);

            /// <summary>
            /// Gets the type of the allocator (Stack)
            /// </summary>
            /// <returns>Returns the type of allocator (Stack)</returns>
            AllocatorType GetAllocatorType();

            /// <summary>
            /// Gets the previous pointer position.
            /// </summary>
            /// <returns></returns>
            void * GetPreviousPosition();

            /// <summary>
            /// Gets the current position of the stack pointer.
            /// </summary>
            /// <returns>Returns the current position of the stack pointer.</returns>
            void * GetCurrentPosition();

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

            struct StackHeader
            {
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
                /// <param name="aPreviousAddress">The previous address of the stack allocation.</param>
                void Write(const UInt8 & aFlags, const UInt8 & aID, const UInt16 & aSize, void * aPreviousAddress);

                /// <summary>
                /// Reads the memory header values into the given arguments.
                /// </summary>
                /// <param name="aFlags">The flags of the memory allocated.</param>
                /// <param name="aID">The ID of the allocator that allocated the memory.</param>
                /// <param name="aSize">The size of the object allocated.</param>
                /// <returns>Returns the pointer to the previous address.</returns>
                void * Read(UInt8 & aFlags, UInt8 & aID, UInt16 & aSize);


				/// <summary>
				/// A pointer to the previous member allocated.
				/// </summary>
				void * previous;

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

         private:

            /// <summary>
            /// A pointer to the current address of the top of the stack.
            /// </summary>
            void * m_CurrentPosition;

            /// <summary>
            /// A pointer to the last address allocated from the top of the stack.
            /// </summary>
            void * m_PreviousPosition;
        };


        //--------------------------------------
        // Stack Allocator Test / Example
        //--------------------------------------
        //StackAllocator * allocator = new StackAllocator(1024, 2);
        //
        //Data16 * a = (Data16*)allocator->Allocate(sizeof(Data16), __alignof(Data16));
        //Data24 * b = (Data24*)allocator->Allocate(sizeof(Data24), __alignof(Data24));
        //Data512 * c = (Data512*)allocator->Allocate(sizeof(Data512), __alignof(Data512));
        //
        //a->Test();
        //b->Test();
        //c->Test();
        //
        ////Manual Rollback
        //c = (Data512*)allocator->Deallocate(c);
        //b = (Data24*)allocator->Deallocate(b);
        //a = (Data16*)allocator->Deallocate(a);
        //
        ////Auto Rollback
        //a = (Data16*)allocator->Allocate(sizeof(Data16), __alignof(Data16));
        //b = (Data24*)allocator->Allocate(sizeof(Data24), __alignof(Data24));
        //c = (Data512*)allocator->Allocate(sizeof(Data512), __alignof(Data512));
        //
        //allocator->Rollback(b);
        //a = (Data16*)allocator->Deallocate(a);
        //
        //delete allocator;
    }
	
}

#endif