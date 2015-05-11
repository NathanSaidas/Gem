#ifndef G_MEMORY_H
#define G_MEMORY_H

#pragma region CHANGE LOG
/*	December, 18, 2014 - Nathan Hanlan - Tested Memory Initialization and CleanUp - Working 100%
*	December, 18, 2014 - Nathan Hanlan - Tested Memory Instantiate and Destroy - Working 100% (Array and Single)
*/
#pragma endregion

#include "G_Allocator.h"
#include "G_FixedAllocator.h"
#include "G_MemoryDefinitions.h"
#include "G_MemoryUtils.h"
#include "G_MemoryManager.h"
#include "G_MemoryFunctions.h"



///Utilizes the exposed functions contained in this header file as well as MemoryFunctions.
///Do not mess with the MemoryManager directly unless you know what you're doing. :)

///Example use.
//using namespace Gem::Memory;
//
//void ExampleFunction()
//{
//  //Initialize the memory manager.
//  Initialize();
//
//  //Allocate memory for an integer pointer and 300 integers (array)
//	int * intPtr = Instantiate<int>();
//	int * intArray = Instantiate<int>(300);
//  
//  ///Modify attributes of the data allocated
//	(*intPtr) = 4;
//	intArray[0] = 0;
//	intArray[1] = 1;
//
//  //Free memory from the data allocated with the matching function prototypes.
//	intPtr = Destroy<int>(intPtr);
//	intArray = Destroy<int>(intArray,300);
//
//  //CleanUp the memory Allocated
//  CleanUp();
//}

namespace Gem
{
	namespace Memory
	{
		///Initializes the memory manager. Call this method at the start of the program.
		inline void Initialize()
		{
			Hidden::MemoryManager::Instance();
		}
		///Cleans up any memory leaks that the memory manager observed. 
		///The memory leaks will be printed out at the end of the program.
		///Alternatively one can use one of the GetMemoryReport functions to get the status of the memory manager.
		inline void CleanUp()
		{
			Hidden::MemoryManager::Destroy();
		}
	}
	

}

#endif