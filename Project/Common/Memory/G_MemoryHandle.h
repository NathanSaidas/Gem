#ifndef G_MEMORY_HANDLE_H
#define G_MEMORY_HANDLE_H


namespace Gem
{

	template<class T>
	class MemoryHandle
	{
	public:
		MemoryHandle()
		{

		}
		MemoryHandle(int aBlockID, T * aMemory) :m_BlockID(aBlockID), m_Memory(aMemory)
		{

		}
		~MemoryHandle()
		{

		}
		T * operator->()
		{
			return m_Memory;
		}
		operator T*()
		{
			return m_Memory;
		}
		int BlockID()
		{
			return m_BlockID;
		}
		T * Memory()
		{
			return m_Memory;
		}
		
	private:
		///Memory Handles with block ID of 0 is a fixed handle.
		///Any other ID represents a dynamic allocator.
		int m_BlockID;
		///Reference to the memory allocated. (Or possibly not allocated :o)
		T * m_Memory;

		friend class MemoryManager;
	};

}

#endif