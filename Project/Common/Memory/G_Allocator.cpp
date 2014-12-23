#include "G_Allocator.h"

namespace Gem
{
	Allocator::Allocator()
	{
		m_ID = 0;
		m_Allocations = 0;
		m_UsedMemory = 0;
		m_TotalSize = 0;
		m_ObjectSize = 0;
		m_NumberOfObjects = 0;
		m_ObjectAlignment = 0;
		m_SelfAllocated = 0;
		m_Memory = nullptr;
		m_IsExpired = false;
		m_ExpiredTime = -1.0f;
	}
	Allocator::Allocator(const Allocator & aRef)
	{
		m_ID = 0;
		m_Allocations = 0;
		m_UsedMemory = 0;
		m_TotalSize = 0;
		m_ObjectSize = 0;
		m_NumberOfObjects = 0;
		m_ObjectAlignment = 0;
		m_SelfAllocated = 0;
		m_Memory = nullptr;
		m_IsExpired = false;
		m_ExpiredTime = -1.0f;
	}
	Allocator::~Allocator()
	{

	}
	unsigned char Allocator::ID()
	{
		return m_ID;
	}
	unsigned char Allocator::ID(unsigned char aID)
	{
		return m_ID = aID;
	}
	unsigned int Allocator::Allocations()
	{
		return m_Allocations;
	}
	unsigned int Allocator::UsedMemory()
	{
		return m_UsedMemory;
	}
	unsigned int Allocator::TotalSize()
	{
		return m_TotalSize;
	}
	unsigned int Allocator::ObjectSize()
	{
		return m_ObjectSize;
	}
	unsigned int Allocator::NumberOfObjects()
	{
		return m_NumberOfObjects;
	}
	unsigned char Allocator::ObjectAlignment()
	{
		return m_ObjectAlignment;
	}
	float Allocator::ExpiredTime()
	{
		return m_ExpiredTime;
	}
	float Allocator::ExpiredTime(float aValue)
	{
		return m_ExpiredTime = aValue;
	}
	bool Allocator::IsExpired()
	{
		return m_IsExpired;
	}
	bool Allocator::IsExpired(bool aValue)
	{
		return m_IsExpired = aValue;
	}

	bool Allocator::HasMemory(unsigned int aAmount)
	{
		return (m_NumberOfObjects - m_Allocations) > aAmount;
	}
	bool Allocator::OutOfMemory()
	{
		return (m_NumberOfObjects - m_Allocations) <= 0;
	}
	Memory::AllocatorMemInfo Allocator::GetMemoryInfo()
	{
		Memory::AllocatorMemInfo info;

		info.allocations = m_Allocations;
		info.maxAllocations = m_NumberOfObjects;
		info.objectSize = m_ObjectSize;

		return info;
	}
}