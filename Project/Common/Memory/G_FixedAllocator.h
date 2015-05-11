#ifndef G_FIXED_ALLOCATOR_H
#define G_FIXED_ALLOCATOR_H

#pragma region CHANGE LOG
/*	December, 17, 2014 - Nathan Hanlan - Added class FixedAllocator
*	December, 17, 2014 - Nathan Hanlan - Implemented FixedAllocator features (Initialization, Disposal, Allocation, Deallocation)
*	December, 17, 2014 - Nathan Hanlan - Tested FixedAllocator Allocation / Deallocation, appears to be working 100%.
*/
#pragma endregion

#include "G_Allocator.h"

namespace Gem
{
	///This allocator allocates a giant heap of memory and aligns memory address based on the object size / memory alignment given.
	class FixedAllocator : public Allocator
	{
	public:
		FixedAllocator();
		virtual ~FixedAllocator();
		///Virtual Allocation/Deallocation Methods
		virtual void * Allocate();
		virtual void * Deallocate(void * aMemory);
		
		//Initializes the fixed allocator
		bool Initalize(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize, void * aMemory);
		bool Initalize(unsigned int aObjectSize, unsigned char aObjectAlignment, unsigned int aSize);
		///Remove any allocated memory
		virtual void Dispose();
		///Determines if the allocator can be used or not.
		bool Initialized();
	protected:
		void ** m_FreeList;

	private:
#pragma region OBSOLETE
		///Array Allocations are no longer supported as of December, 18, 2014
		//virtual void * Allocate(int aLength);
		//virtual void * Deallocate(void * aMemory, unsigned int aLength);
#pragma endregion
	};
}

#endif