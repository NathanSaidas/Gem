#ifndef GAME_ENGINE_MEMORY_MANAGER_H
#define GAME_ENGINE_MEMORY_MANAGER_H

#pragma region CHANGE LOG
/// -- March	1	2015 - Nathan Hanlan - Improved the complexity of the memory manger and refactored the class to provide clearer function names.
/// -- March	1	2015 - Nathan Hanlan - Added additional comments.
/// -- March   21   2015 - Nathan Hanlan - Added additional TYPE macros to alloc/dealloc types. (These will call destructors and constructors)
#pragma endregion

#pragma region TODO
/// -- Garbage Collection Implementation. (This has been implemented in terms of detecting it for pool deallocations but the actual flagging / collecting needs to be implemented)
/// -- Dynamic Allocator erase allocations made in the allocator. See DestroyPoolAllocator method. 
#pragma endregion

#include <map> //For Memory Tracking

// -- Include basic types for UInt and SInt and IntPtr, and GemAPI for API export/import macro
#include "../Core/ValueTypes.h"
#include "../Core/GemAPI.h"

// -- Memory Standard Includes
#include "MemoryConfig.h"
#include "MemoryUtils.h"
#include "MemoryHeader.h"
#include "MemoryFlags.h"
#include "AllocationInfo.h"
#include "Instantiator.h"

// -- Allocator Includes
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "FrameAllocator.h"


//Export this template class.
template class GEM_API std::map<Gem::IntPtr, Gem::Memory::AllocationInfo>;

// -- Alloc Defines. Use these to avoid writing __FILE__ and __LINE__ and wrapping it in ifdefs

#ifdef CONFIG_MEMORY_DEBUG
#ifndef MEM_ALLOC
#define MEM_ALLOC(SIZE,ALIGNMENT,ALLOCATOR_TYPE) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,ALLOCATOR_TYPE,__FILE__,__LINE__)
#endif

#ifndef MEM_POOL_ALLOC
#define MEM_POOL_ALLOC(SIZE,ALIGNMENT) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,Memory::AllocatorType::Pool,__FILE__,__LINE__)
#endif

#ifndef MEM_STACK_ALLOC
#define MEM_STACK_ALLOC(SIZE,ALIGNMENT) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,Memory::AllocatorType::Stack,__FILE__,__LINE__)
#endif

#ifndef MEM_FRAME_ALLOC
#define MEM_FRAME_ALLOC(SIZE,ALIGNMENT) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,Memory::AllocatorType::Frame,__FILE__,__LINE__)
#endif

#else
#ifndef MEM_ALLOC
#define MEM_ALLOC(SIZE,ALIGNMENT,ALLOCATOR_TYPE) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,ALLOCATOR_TYPE)
#endif

#ifndef MEM_POOL_ALLOC
#define MEM_POOL_ALLOC(SIZE,ALIGNMENT) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,Memory::AllocatorType::Pool)
#endif

#ifndef MEM_STACK_ALLOC
#define MEM_STACK_ALLOC(SIZE,ALIGNMENT) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,Memory::AllocatorType::Stack)
#endif

#ifndef MEM_FRAME_ALLOC
#define MEM_FRAME_ALLOC(SIZE,ALIGNMENT) Memory::MemoryManager::GetInstance()->Allocate(SIZE,ALIGNMENT,Memory::AllocatorType::Frame)
#endif

#endif

#ifndef MEM_ALLOC_T
#define MEM_ALLOC_T(TYPE,ALLOCATOR_TYPE) new (MEM_ALLOC(sizeof(TYPE),__alignof(TYPE),ALLOCATOR_TYPE))T()
#endif

#ifndef MEM_POOL_ALLOC_T
#define MEM_POOL_ALLOC_T(TYPE,...) Gem::Memory::Instantiator<TYPE>::Construct(MEM_POOL_ALLOC(sizeof(TYPE),__alignof(TYPE)),__VA_A_ARGS__)
#endif

#ifndef MEM_STACK_ALLOC_T
#define MEM_STACK_ALLOC_T(TYPE) new (MEM_STACK_ALLOC(sizeof(TYPE),__alignof(TYPE)))TYPE()
#endif

#ifndef MEM_FRAME_ALLOC_T
#define MEM_FRAME_ALLOC_T(TYPE) new (MEM_FRAME_ALLOC(sizeof(TYPE),__alignof(TYPE)))TYPE()
#endif

#ifndef NEW_POOL
#define NEW_POOL(TYPE) new (MEM_POOL_ALLOC(sizeof(TYPE),__alignof(TYPE)))TYPE
#endif

#ifndef NEW_STACK
#define NEW_STACK(TYPE) new (MEM_STACK_ALLOC(sizeof(TYPE),__alignof(TYPE)))TYPE
#endif

#ifndef NEW_FRAME
#define NEW_FRAME(TYPE) new (MEM_FRAME_ALLOC(sizeof(TYPE),__alignof(TYPE)))TYPE
#endif

#ifndef MEM_DEALLOC
#define MEM_DEALLOC(ADDRESS,ALLOCATOR_TYPE,SIZE) Memory::MemoryManager::GetInstance()->Deallocate(ADDRESS,ALLOCATOR_TYPE,SIZE)
#endif

#ifndef MEM_POOL_DEALLOC
#define MEM_POOL_DEALLOC(ADDRESS,SIZE) Memory::MemoryManager::GetInstance()->Deallocate(ADDRESS,Memory::AllocatorType::Pool,SIZE)
#endif

#ifndef MEM_STACK_DEALLOC
#define MEM_STACK_DEALLOC(ADDRESS) Memory::MemoryManager::GetInstance()->Deallocate(ADDRESS,Memory::AllocatorType::Stack)
#endif

#ifndef MEM_FRAME_DEALLOC
#define MEM_FRAME_DEALLOC(ADDRESS) Memory::MemoryManager::GetInstance()->Deallocate(ADDRESS,Memory::AllocatorType::Frame)


#ifndef MEM_DEALLOC_T
#define MEM_DEALLOC_T(ADDRESS,TYPE, ALLOCATOR_TYPE) ADDRESS->~TYPE(); ADDRESS = (TYPE*)MEM_DEALLOC(ADDRESS,ALLOCATOR_TYPE,sizeof(TYPE))
#endif

#ifndef MEM_POOL_DEALLOC_T
#define MEM_POOL_DEALLOC_T(ADDRESS,TYPE) ADDRESS->~TYPE(); ADDRESS = (TYPE*)MEM_POOL_DEALLOC(ADDRESS,sizeof(TYPE))
#endif

#ifndef MEM_STACK_DEALLOC_T
#define MEM_STACK_DEALLOC_T(ADDRESS,TYPE) ADDRESS->~TYPE(); ADDRESS = (TYPE*)MEM_STACK_DEALLOC(ADDRESS)
#endif

#ifndef MEM_FRAME_DEALLOC_T
#define MEM_FRAME_DEALLOC_T(ADDRESS,TYPE) ADDRESS->~TYPE(); ADDRESS = (TYPE*)MEM_FRAME_DEALLOC(ADDRESS)
#endif


#endif



namespace Gem
{    
    namespace Memory
    {		
		/// <summary>
		///	This class manages operations with allocating and deallocating memory.
		/// 
		/// With the use of the three custom allocators created stack, frame and pool the memory manager can
		/// improve allocation speed by elimating the need for context switching between kernel mode and user mode
		/// and fragmenting memory. The memory manager has full control of the memory and any memory leaked can be tracked
		/// and properly disposed of with one call to Terminate after the memory manager has been initialized.
		///
		/// The memory manager allows for the user to choose to lock an allocator type which can improve allocation / deallocation times.
		///
		///
		/// In the future there will be automatic and semi automatic garbage collection.
		/// 
		/// </summary>
        class GEM_API MemoryManager
        {
        public:                  
            
            /// <summary>
            /// Initializes the memory manager by creating a singleton instance of MemoryManager
            /// </summary>
            static void Initialize();

            /// <summary>
            /// Terminates the memory manager by destroying the singleton instance of the MemoryManager
            /// </summary>
            static void Terminate();

            /// <summary>
            /// Gets the singleton instance of the Memory Manager.
            /// </summary>
            /// <returns>Returns the singleton instance of the memory manager.</returns>
            static MemoryManager * GetInstance();

			///<summary>
			/// Allocates an address for the specified size and alignment. The type of allocattion can be specified as well.
			///</summary>
			/// <param name="aSize">The size of the object to be allocated.</param>
			/// <param name="aAlignment">The alignment of the object to be allocated.</param>
			/// <param name="aFilename">The name of the file making the allocation.</param>
			/// <param name="aLine">The line where the allocation is made.</param>
#ifdef CONFIG_MEMORY_DEBUG
			void * Allocate(UInt32 aSize, UInt8 aAlignment, AllocatorType aType, const char * aFilename, UInt32 aLine);
#else
			void * Allocate(UInt32 aSize, UInt8 aAlignment, AllocatorType aType);		
#endif
			///<summary>
			/// Deallocates the address. This assumes the memory was allocated from the memory manager to begin with and utilizes the MemoryHeader for getting additional information about the allocation.
			///</summary>
			/// <param name="aAddress">The address to be deallocated.</param>
			/// <param name="aType">The type of allocator to deallocate from.</param>
			/// <param name="aObjectSize">The size of the object for pool deallocations for extra security and dynamic pool searching.</param>
			void * Deallocate(void * aAddress, AllocatorType aType, UInt32 aObjectSize = 0);

			/// <summary>
			/// Rollbacks the stack to the specified address.
			/// </summary>
			/// <param name="aAddress">The address to rollback to</param>
			/// <returns>Returns a nullptr</returns>
			void * RollbackStack(void * aAddress);

			/// <summary>
			/// Rollbacks the stack to a nullptr;
			/// </summary>
			/// <returns>Returns a nullptr</returns>
			void * RollbackStack();

			/// <summary>
			/// Resets the frame allocator memory back to the start.
			/// </summary>
			void ResetFrame();

			///<summary>
			/// Searches for the allocator using the address pased in and then locks the allocator of the given address.
			///</summary>
			///<param name="aAddress">The address to search for the allocator for.</param>
			void LockAllocator(void * aAddress);

			///<summary>
			/// Locks the stack allocator if no allocator is not locked
			///</summary>
			void LockStackAllocator();

			///<summary>
			/// Locks the frame allocator if no allocator is not locked
			///</summary>
			void LockFrameAllocator();

			///<summary>
			/// Locks the pool allocator if no allocator is not locked. Searches through static ones before dynamic.
			///</summary>
			///<param name="aObjectSize">The size of the object to compare to for pool allocator searches.</param>
			///<param name="aAlignment">The alignment of the object to compare to for pool allocator searches.</param>
			void LockPoolAllocator(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Locks the pool allocator if no allocator is not locked.
			///</summary>
			///<param name="aObjectSize">The size of the object to compare to for pool allocator searches.</param>
			///<param name="aAlignment">The alignment of the object to compare to for pool allocator searches.</param>
			void LockStaticPoolAllocator(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Locks the pool allocator if no allocator is not locked.
			///</summary>
			///<param name="aObjectSize">The size of the object to compare to for pool allocator searches.</param>
			///<param name="aAlignment">The alignment of the object to compare to for pool allocator searches.</param>
			void LockDynamicPoolAllocator(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Locks the pool allocator if no allocator is not locked.
			///</summary>
			///<param name="aIndex">The index of the dynamic pool allocator to lock.</param>
			void LockDynamicPoolAllocator(UInt8 aIndex);

			///<summary>
			/// Releases the lock on the allocator.
			///</summary>
			void ReleaseLock();

			///<summary>
			/// Gets the state of whether or not the allocator is locked.
			///</summary>
			///<returns>Returns the state of whether or not the allocator is locked</returns>
			bool IsLocked();

			///<summary>
			/// Checks whether or not a static stack allocator can allocate memory for the given object.
			///</summary>
			///<param name="aObjectSize">The size of the object to be allocated.</param>
			///<param name="aAlignment">The alignment of the object to be allocated.</param>
			///<returns>Returns whether or not its possible to allocate the specified memory</returns>
            bool CanAllocStack(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Checks whether or not a static frame allocator can allocate memory for the given object.
			///</summary>
			///<param name="aObjectSize">The size of the object to be allocated.</param>
			///<param name="aAlignment">The alignment of the object to be allocated.</param>
			///<returns>Returns whether or not its possible to allocate the specified memory</returns>
            bool CanAllocFrame(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Checks whether or not any pool allocator or just the static can allocate memory for the given object.
			///</summary>
			///<param name="aObjectSize">The size of the object to be allocated.</param>
			///<param name="aAlignment">The alignment of the object to be allocated.</param>
			///<returns>Returns whether or not its possible to allocate the specified memory</returns>
            bool CanAllocPool(UInt32 aObjectSize, UInt8 aAlignent, bool aStaticOnly);

			///<summary>
			/// Checks whether or not a static pool allocator can allocate memory for the given object.
			///</summary>
			///<param name="aObjectSize">The size of the object to be allocated.</param>
			///<param name="aAlignment">The alignment of the object to be allocated.</param>
			///<returns>Returns whether or not its possible to allocate the specified memory</returns>
            bool CanAllocPoolStatic(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Checks whether or not any dynamic pool allocator can allocate memory for the given object.
			///</summary>
			///<param name="aObjectSize">The size of the object to be allocated.</param>
			///<param name="aAlignment">The alignment of the object to be allocated.</param>\
			///<returns>Returns whether or not its possible to allocate the specified memory</returns>
            bool CanAllocPoolDynamic(UInt32 aObjectSize, UInt8 aAlignment);

			///<summary>
			/// Creates a pool allocator using the given parameters.
			///</summary>
			///<param name="aObjectSize">The size of the object that can be allocated by the pool allocator.</param>
			///<param name="aAlignment">The alignment of the object that can be allocated by the pool allocator.</param>
			///<returns>Returns the created allocator.</returns>
            PoolAllocator * CreatePoolAllocator(UInt32 aObjectSize, UInt8 aAlignment);

			/// <summary>
			/// Destroys the pool allocator at the given index.
			/// </summary>
			/// <param name="aIndex">The index of the allocator to destroy.</param>
            void DestroyPoolAllocator(UInt32 aIndex);

			/// <summary>
			/// Properly destroys the pool allocator and removes all the allocation info related to the allocator.
			/// </summary>
			/// <param name="aAllocator">The allocator to destroy</param>
            void DestroyPoolAllocator(PoolAllocator * aAllocator);

			/// <summary>
			/// Gets the stack allocator.
			/// </summary>
			/// <param name="aRequireMemory">Whether or not to require memory from the allocator.</param>
			/// <returns>Returns the StackAllocator</returns>
            StackAllocator * GetStackAllocator(bool aRequireMemory);
            
			/// <summary>
			/// Gets the frame allocator.
			/// </summary>
			/// <param name="aRequireMemory">Whether or not to require memory from the allocator.</param>
			/// <returns>Returns the Frame allocator.</returns>
			FrameAllocator * GetFrameAllocator(bool aRequireMemory);
            
			/// <summary>
			/// Searches for a pool allocator. Searches the static ones first then the dynamic.
			/// </summary>
			/// <param name="aRequireMemory">Whether or not to require memory from the allocator.</param>
			/// <param name="aObjectSize">The allocator object size.</param>
			/// <param name="aAlignment">The allocator object alignment</param>
			/// <param name="aStaticOnly">Searches for static pool allocators only.</param>
			/// <returns>Returns the found pool allocator.</returns>
			PoolAllocator * GetPoolAllocator(bool aRequireMemory, UInt32 aObjectSize, UInt8 aAlignment, bool aStaticOnly);
            
			/// <summary>
			/// Searches for a static pool allocator that matches the object size / alignment
			/// If aRequireMemory is true the memory usage of the allocator is checked. This does not use the CanAlloc method
			/// </summary>
			/// <param name="aRequireMemory">Whether or not to check the memory usage</param>
			/// <param name="aObjectSize">Size of a object.</param>
			/// <param name="aAlignment">The alignment of the object</param>
			/// <returns>Returns an allocator that matches the description. (ObjectSize/Alignment/Memory). Returns null if none are found.</returns>
			PoolAllocator * GetStaticPoolAllocator(bool aRequireMemory, UInt32 aObjectSize, UInt8 aAlignment);
            
			/// <summary>
			/// Searches for the first dynamic pool allocator who has the specific object size / alignment
			/// If aRequireMemory is true then a memory check is done to make sure the allocator is not full. This doesnt check if an allocation is possible.
			/// </summary>
			/// <param name="aRequireMemory">Whether or not to check memory usage.</param>
			/// <param name="aObjectSize">Size of a object.</param>
			/// <param name="aAlignment">The alignment of the object.</param>
			/// <returns>Returns the pool allocator matching the description. (ObjectSize/Alignment/MemoryUsage) or null if none could be found.</returns>
			PoolAllocator * GetDynamicPoolAllocator(bool aRequireMemory, UInt32 aObjectSize, UInt8 aAlignment);

			/// <summary>
			/// Searches for the allocator responsible for allocating the address.
			/// </summary>
			/// <param name="aAddress">The address to search for.</param>
			/// <returns>Allocator associated with the address.</returns>
			Allocator * GetAllocator(void * aAddress);

			/// <summary>
			/// Records the allocation made.
			/// </summary>
			/// <param name="aAddress">The address allocated.</param>
			/// <param name="aFile">The file responsible of the leak.</param>
			/// <param name="aLine">The line within the file which caused the memory leak.</param>
			/// <param name="aAllocatorID">The allocator ID responsible for managing the allocation. This may change and a flag will be raised in case of leak.</param>
			/// <param name="aType">The type of allocation that was made.</param>
            void RecordAllocation(IntPtr aAddress, const char * aFile, UInt32 aLine, UInt8 aAllocatorID, AllocatorType aType);
            
			/// <summary>
			/// Erases the allocation info associated with the address given.
			/// </summary>
			/// <param name="aAddress">The address which was allocated that needs to be removed from the leak detection records.</param>
			void EraseAllocation(IntPtr aAddress);
        private:
            static MemoryManager * s_Instance; 
            MemoryManager();
            ~MemoryManager();
            // -- Static Memory
            PoolAllocator * m_StaticPool[Config::Memory::DEFAULT_POOL_BLOCK_COUNT];
            StackAllocator * m_StaticStack;
            FrameAllocator * m_StaticFrame;
			// -- Dynamic Memory
			PoolAllocator * m_DynamicPool[Config::Memory::DYNAMIC_ALLOCATOR_COUNT];
			// -- Allocator currently locked for use.
            Allocator * m_LockedAllocator;
            // Memory Leak Detection
			Allocator * m_RecentAllocator;
			std::map<IntPtr, AllocationInfo> m_RecordedAllocations;

			/// <summary>
			/// Allocates memory from the stack.
			/// </summary>
			/// <param name="aSize">The size of the allocation to be made.</param>
			/// <param name="aAlignment">The alignment of the allocation to be made.</param>
			void * AllocateStack(UInt32 aSize, UInt8 aAlignment);

			/// <summary>
			/// Allocates memory from the frame.
			/// </summary>
			/// <param name="aSize">The size of the allocation to be made.</param>
			/// <param name="aAlignment">The alignment of the allocation to be made.</param>
			void * AllocateFrame(UInt32 aSize, UInt8 aAlignment);

			/// <summary>
			/// Allocates memory from the pool.
			/// </summary>
			/// <param name="aSize">The size of the allocation to be made.</param>
			/// <param name="aAlignment">The alignment of the allocation to be made.</param>
			void * AllocatePool(UInt32 aSize, UInt8 aAlignment);

			/// <summary>
			/// Deallocates an address from the stack.
			/// </summary>
			/// <param name="aAddress">The address to deallocate</param>
			void * DeallocateStack(void * aAddress);

			/// <summary>
			/// Deallocates an address from the frame.
			/// </summary>
			/// <param name="aAddress">The address to deallocate</param>
			void * DeallocateFrame(void * aAddress);

			/// <summary>
			/// Deallocates an address from a pool.
			/// </summary>
			/// <param name="aAddress">The address to deallocate</param>
			/// <param name="aObjectSize">The size of the object to deallocate</param>
			void * DeallocatePool(void * aAddress, UInt32 aObjectSize);

        };
    }
	
}

#endif