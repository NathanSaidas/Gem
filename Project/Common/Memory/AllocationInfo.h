#ifndef GAME_ENGINE_ALLOCATION_INFO_H
#define GAME_ENGINE_ALLOCATION_INFO_H

#pragma region CHANGE LOG
/// -- January 31, 2015 - Nathan Hanlan - Added AllocationInfo class/file
/// -- April    7, 2015 - Nathan Hanlan - No longer using BasicTypes.h, using ValueTypes.h instead.
/// -- April    7, 2015 - Nathan Hanlan - Converted to comment styles to doxygen.
#pragma endregion

#include <string>
#include "../Core/GemAPI.h"
#include "../Core/ValueTypes.h"
#include "AllocatorType.h"


namespace Gem
{
    namespace Memory
    {		
		/**
		* The class used to represent allocation info. The filename the line and information regarding the allocator responsible for the allocation.
		*/
        class GEM_API AllocationInfo
        {
        public:
			/**
			* Default Constructor, sets values to defaults.
			*/
			AllocationInfo()
			{
				m_Filename = "";
				m_Line = 0;
				m_AllocatorID = 0;
				m_AllocationType = AllocatorType::Pool;
				m_Address = 0;
				m_Count = 0;
				m_AllocatorMisMatch = false;
			}

			/**
			* Constructor which takes in information to build the class structure with.
			* @param aFilename The name of the file of where the allocation occured.
			* @param aLine The line within the file the allocation occured.
			* @param aAllocatorType The type of allocator responsible for the allocatoin
			* @param aAddress The address of the allocated memory.
			*/
            AllocationInfo(const char * aFilename, 
                UInt32 aLine, 
                UInt8 aAllocatorID,
                AllocatorType aAllocationType, 
                IntPtr aAddress)
            {
                m_Filename = aFilename;
                m_Line = aLine;
                m_AllocatorID = aAllocatorID;
                m_AllocationType = aAllocationType;
                m_Address = aAddress;
                m_Count = 1;
                m_AllocatorMisMatch = false;
            }
			
			/**
			* Destructor to clean up any information	
			*/
            ~AllocationInfo()
            {

            }
			

			/**
			* Gets the file name.
			* @return Returns the filename
			*/
			inline const char * GetFilename()
            {
                return m_Filename;
            }		

			/**
			* Gets the line.
			* Returns the line.
			*/
			inline UInt32 GetLine()
            {
                return m_Line;
            }
						
			/**
			* Gets the amount of times this address has been allocated.
			* @return Returns the amount of times this address has been allocated.
			*/
			inline unsigned GetCount()
            {
                return m_Count;
            }

			
			/**
			* Increments the count by one.
			*/
			inline void AddCount()
            {
                m_Count++;
            }
			
			/**
			* Decrements the count by one
			*/
			inline void SubtractCount()
            {
                m_Count--;
            }
			
			/**
			* Gets the id of the allocator responsible of the allocation made.
			* @Returns the allocator ID.
			*/
			inline UInt8 GetAllocatorID()
            {
                return m_AllocatorID;
            }

			/**
			* Sets the ID of the allocator made. If the ID does not match the previous flag allocator mismatch will be set to true.
			* @param aAllocatorID
			*/
			inline void SetAllocatorID(UInt8 aAllocatorID)
            {
                m_AllocatorID = aAllocatorID;
            }
			
			/**
			* Flags the allocator for giving a warning about a mismatch in allocations made. (Address used two times or more without being deallocated)
			*/
			inline void FlagAllocatorMismatch()
            {
                m_AllocatorMisMatch = true;
            }
			
			/**
			* Gets the type of allocator that made the allocation
			* @return The type of allocator responsible for allocating the memory.
			*/
			inline AllocatorType GetAllocatorType()
            {
                return m_AllocationType;
            }
			
			/**
			* Returns the address of the allocation.
			* @return Returns the address of the allocation made.
			*/
			inline IntPtr GetAddress()
            {
                return m_Address;
            }
            

        private:
			/**
			* The name of the file of where the allocation was made.
			*/
            const char * m_Filename;
			/**
			* The line within the file of where the allocation was made.
			*/
            UInt32 m_Line;
			/**
			* The amount of times an allocation was made.
			*/
            UInt32 m_Count;
			/**
			* The ID of the allocator making the Allocation.
			*/
            UInt8 m_AllocatorID;
			/**
			* The type of allocator responsible for the allocation.
			*/
            AllocatorType m_AllocationType;
			/**
			* Whether or not an error happened and memory was leaked.
			*/
            bool m_AllocatorMisMatch;
			/**
			* The address of the allocation.
			*/
            IntPtr m_Address;
            
        };
    }
}

#endif