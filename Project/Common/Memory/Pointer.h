#ifndef GAME_ENGINE_POINTER_H
#define GAME_ENGINE_POINTER_H

#pragma region CHANGE LOG
/// --	March	30	2015 - Nathan Hanlan - Added class/file Pointer.h.
/// --  April	 8  2015 - Nathan Hanlan - Added Cast function to cast one pointer to another.
/// --  April    8  2015 - Nathan Hanlan - Added in Terminate function which will deallocate the pointer resource and invert the reference count.
/// --  April    8  2015 - Nathan Hanlan - Added in Null static function which will return a null managed pointer.
/// --  April    9  2015 - Nathan Hanlan - Pointers only accept objects with a GetType function that returns a Type. 
/// --  -----    -  ---- - ------ ------ - (This includes anything with RDECLARE_ / RDEFINE reflection types for classes and interfaces)
#pragma endregion

#include "MemoryManager.h"
#include "../Core/GemAPI.h"
#include "../Core/Debug.h"
#include "../Reflection/Reflection.h"
//#include "../Utilities/Utilities.h"
//#include "../SystemAssert.h"
//#include "../SystemError.h"

namespace Gem
{

	//This class is designed to keep track of references to a pointer. 
	//
	template<typename TYPE>
	class GEM_API Pointer
	{
	public:
		///Default Constructor will allocate a instance of the TYPE and set the reference count to 1.
		Pointer()
		{
			//Set Defaults
			m_Pointer = nullptr;
			m_Count = nullptr;

			//Allocate and set count to 1.
			m_Count = new int;
			*m_Count = 1;
			Alloc();
		}
		///Copy Constructor will take the pointer from the incoming pointer and increment the reference count.
		Pointer(const Pointer & aPointer)
		{
//#ifdef CONFIG_MEMORY_DEBUG
//			Assert(aPointer.m_Pointer != nullptr, Error::BAD_POINTER_COPY);
//#endif
			//Take pointer / count and increment reference.
			m_Pointer = aPointer.m_Pointer;
			m_Count = aPointer.m_Count;
			AddReference();
		}
		
		///Destructor decrements a reference.
		~Pointer()
		{
			RemoveReference();
		}

		///Asignment operator removes previous reference and adds a reference to the incoming pointer.
		Pointer operator=(const Pointer & aPointer)
		{
			RemoveReference();
			m_Pointer = aPointer.m_Pointer;
			m_Count = aPointer.m_Count;
			AddReference();
			return *this;
		}

		///Access to the data.
		TYPE * operator->()
		{
			if (m_Count == nullptr || *m_Count <= 0)
			{
				m_Pointer = nullptr;
			}
			return m_Pointer;
		}

		///Access to the data.
		TYPE * operator->() const
		{
			if (m_Count == nullptr || *m_Count <= 0)
			{
				return nullptr;
			}
			return m_Pointer;
		}

		///Returns true if the 
		bool IsAlive()
		{
			if (m_Count == nullptr || *m_Count <= 0)
			{
				m_Pointer = nullptr;
			}
			return m_Pointer != nullptr;
		}

		///Returns true if the 
		bool IsAlive() const
		{
			return m_Pointer != nullptr && m_Count != nullptr && *m_Count > 0;
		}

		///Returns the reference count
		int GetReferenceCount()
		{
			return *m_Count;
		}

		
		/**
		* Removes a reference from the pointer and sets the count / pointer to nullptr;
		*/
		void Release()
		{
			RemoveReference();
			m_Pointer = nullptr;
			m_Count = nullptr;
		}

		/**
		* Releases the resource of the pointer. 
		* This function will only work on pointers allocated through a pool allocator.
		* Other managed pointers will have their count inverted signalling that the pointer is dead.
		*/
		void Terminate()
		{
			if (m_Pointer != nullptr)
			{
				Memory::MemoryHeader * header = (Memory::MemoryHeader*)Memory::MemoryUtils::SubtractPtr(m_Pointer, sizeof(Memory::MemoryHeader));
				UInt8 flags;
				UInt8 id;
				UInt16 size;
				header->Read(flags, id, size);
				if (!((flags & Memory::MemoryFlags::POOL) == Memory::MemoryFlags::POOL))
				{
					Debugging::Debug::Error("Memory", "Managed pointers can only terminate memory allocated through a pool allocator.");
					return;
				}
				Dealloc();
			}
			if (m_Count != nullptr)
			{
				*m_Count = -(*m_Count);
				m_Count = nullptr;
			}
		}

		///Makes this pointer a unique instance and uses the assignment operator.
		void MakeUnique()
		{
			if (m_Count != nullptr && *m_Count > 1)
			{
				TYPE * prev = m_Pointer;
				Release();
				
				m_Count = new int;
				*m_Count = 1;
				Alloc();

				*m_Pointer = *prev;
			}
			//Else this is already unique.
			
		}

		///Makes this pointer unique using an already allocated pointer.
		void MakeUnique(TYPE * aPointer)
		{
			if (aPointer == nullptr)
			{
				return;
			}
			Memory::MemoryHeader * header = (Memory::MemoryHeader*)Memory::MemoryUtils::SubtractPtr(aPointer, 4);
			UInt8 flags;
			UInt8 id;
			UInt16 size;
			header->Read(flags, id, size);

			if (!((flags & Memory::MemoryFlags::POOL) == Memory::MemoryFlags::POOL))
			{
				Debugging::Debug::Error("Memory", "Cannot make a pointer that is not allocated through a pool a managed pointer.");
				return;
			}

			RemoveReference();
			m_Pointer = aPointer;
			m_Count = new int;
			*m_Count = 1;
		}

		/**
		* This method will convert a managed pointer of a given TYPE into a a managed pointer of the CAST_TYPE specified
		*/
		template<typename CAST_TYPE>
		Pointer<CAST_TYPE> Cast() const
		{
			//Create a pointer and release resources allocated.
			Pointer<CAST_TYPE> pointer;
			pointer.Release();
			//Get the raw address
			CAST_TYPE * address = (CAST_TYPE*)(m_Pointer);
			//Use the hidden cast method to cast the data.
			pointer.HiddenCast(address, m_Count);

			return pointer;
		}

		/**
		* This will set information from one pointer to another and add a reference.
		* IMPORTANT: Do not call this method directly. Use Cast instead.
		* @param aAddress The address of the pointer making a casted instance
		* @param aCount The count pointer of the pointer making a casted instance.
		*/
		void HiddenCast(TYPE * aAddress, int * aCount)
		{
			Release();
			if (aAddress != nullptr)
			{
				m_Pointer = aAddress;
				m_Count = aCount;
				AddReference();
			}

		}

		/**
		* Creates a pointer with no count or reference to memory.
		* @return Returns a pointer with no count or reference to memory.
		*/
		static Pointer Null()
		{
			Pointer pointer;
			pointer.Release();
			return pointer;
		}

	private:
		
		
		///Adds a reference to the pointer.
		void AddReference()
		{
			if (m_Count != nullptr)
			{
				(*m_Count)++;
			}
		}

		///Removes a refrence from the pointer. Handles deallocation upon meeting 0 references.
		void RemoveReference()
		{
			if (m_Count != nullptr)
			{
				
				//Decrement reference count for alive pointers.
				if (*m_Count > 0)
				{
					(*m_Count)--;
				}
				//Increment reference count for dead pointers that were killed with the Terminate function.
				else if (*m_Count < 0)
				{
					(*m_Count)++;
					m_Count = nullptr;
					return;
				}

				//(*m_Count)--;
				if ((*m_Count) == 0)
				{
					delete m_Count;
					m_Count = nullptr;
					if (m_Pointer != nullptr)
					{
						Dealloc();
					}
				}
			}
		}
		
		///Allocates a pointer. If the pointer has 
		void Alloc()
		{
			if (m_Pointer != nullptr)
			{
				Debugging::Debug::Error("Memory", "Pointer has not been deallocated yet.");
				Dealloc();
			}
			m_Pointer = MEM_POOL_ALLOC_T(TYPE);
		}

		///Deallocates the memory associated with the pointer.
		void Dealloc()
		{
			Type type = m_Pointer->GetType();
			type.GetDestructor()(m_Pointer);
			m_Pointer = (TYPE*)MEM_POOL_DEALLOC(m_Pointer,type.GetSize());
		}

	
		TYPE * m_Pointer;
		int * m_Count;
	};

}

#endif