#include "G_UniqueNumberGenerator.h"

namespace Gem
{
	UniqueNumberGenerator::UniqueNumberGenerator()
	{
		m_NextNumber = 0;
	}
	UniqueNumberGenerator::UniqueNumberGenerator(int aStartID)
	{
		m_NextNumber = aStartID;
	}
	UniqueNumberGenerator::~UniqueNumberGenerator()
	{

	}
	///Retrieves the next available ID and increments the ID
	int UniqueNumberGenerator::Get()
	{
		//Get a unique number off the free list, cross reference check it with the reserved list
		int uniqueNumber = -1;
		while (m_FreeList.Count() > 0)
		{
			uniqueNumber = m_FreeList[0];
			m_FreeList.RemoveAt(0);
			if (!m_ReservedList.Contains(uniqueNumber))
			{
				if (uniqueNumber > m_LargestNumber)
				{
					m_LargestNumber = uniqueNumber;
				}
				return uniqueNumber;
			}
		}
		//Get a unique number by incrementing the next number value, cross reference check it with the reserved list.
		uniqueNumber = m_NextNumber;
		m_NextNumber++;
		while (m_ReservedList.Contains(uniqueNumber))
		{
			uniqueNumber = m_NextNumber;
			m_NextNumber++;
		}
		if (uniqueNumber > m_LargestNumber)
		{
			m_LargestNumber = uniqueNumber;
		}
		return uniqueNumber;
	}
	///Reserves an ID, Returns true if successful, false if the ID cannot be reserved
	bool UniqueNumberGenerator::Reserve(int aID)
	{
		if (aID < 0)
		{
			return false;
		}
		//Check the reserved list
		if (m_ReservedList.Contains(aID))
		{
			return false;
		}
		//Check the free list
		if (aID < m_NextNumber)
		{
			if (m_FreeList.Contains(aID))
			{
				m_FreeList.Remove(aID);
			}
			else
			{
				return false;
			}
		}
		if (aID > m_LargestNumber)
		{
			m_LargestNumber = aID;
		}
		m_ReservedList.Add(aID);
		return true;
	}
	///Frees a ID back so it can be reused.
	void UniqueNumberGenerator::Free(int aID)
	{
		if (aID == m_LargestNumber)
		{
			m_LargestNumber = 0;
		}
		if (m_ReservedList.Contains(aID))
		{
			m_ReservedList.Remove(aID);
		}
		if (!(aID >= m_NextNumber))
		{
			m_FreeList.Add(aID);
		}

		Collections::Iterator<int> * iter = m_ReservedList.GetFront();
		while (iter != nullptr)
		{
			if (iter->Current() > m_LargestNumber)
			{
				m_LargestNumber = iter->Current();
			}
			iter = iter->GetNext();
		}

		if (m_NextNumber > m_LargestNumber && !m_FreeList.Contains(m_NextNumber))
		{
			m_LargestNumber = m_NextNumber;
		}
	}
	///Resets the generator to the specified start ID.
	void UniqueNumberGenerator::Reset(int aStart)
	{
		m_NextNumber = aStart;
		m_LargestNumber = 0;
		m_FreeList.Clear();
		m_ReservedList.Clear();
	}
	///Returns true if the ID is on the free list.
	bool UniqueNumberGenerator::IsFree(int aID)
	{
		return m_FreeList.Contains(aID);
	}
	///Accessor to the m_NextNumber member
	int UniqueNumberGenerator::NextNumber()
	{
		return m_NextNumber;
	}
	///Accessor to the m_LargestNumber member.
	int UniqueNumberGenerator::LargestNumber()
	{
		return m_LargestNumber;
	}
}