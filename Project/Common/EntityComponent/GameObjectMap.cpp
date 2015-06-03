#include "GameObjectMap.h"
#include "../Memory/Memory.h"


namespace Gem
{
	RDEFINE_CLASS(GameObjectMap,object)

	GameObjectMap::GameObjectMap()
		: m_Elements(nullptr),
		m_Capacity(0),
		m_Count(0),
		m_HashFunction(HashString),
		m_Allocator(nullptr)
	{

	}

	GameObjectMap::GameObjectMap(UInt32 aCapacity)
		: m_Elements(new Key[aCapacity]),
		m_Capacity(aCapacity),
		m_Count(0),
		m_HashFunction(HashString)
	{
		m_Allocator = new Memory::PoolAllocator(sizeof(KeyIterator)* (m_Capacity + 1), 0, sizeof(KeyIterator), __alignof(KeyIterator));

	}

	GameObjectMap::GameObjectMap(UInt32 aCapacity, HashFunction aHashFunction)
		: m_Elements(new Key[aCapacity]),
		m_Capacity(aCapacity),
		m_Count(0)
	{
		SetHashFunction(aHashFunction);
		m_Allocator = new Memory::PoolAllocator(sizeof(KeyIterator)* (m_Capacity + 1), 0, sizeof(KeyIterator), __alignof(KeyIterator));
	
	}

	GameObjectMap::~GameObjectMap()
	{
		for (int i = 0; i < m_Capacity; i++)
		{
			m_Elements[i].Clear(*this, &GameObjectMap::DestroyKeyIterator);
		}
		delete[] m_Elements;
		delete m_Allocator;

	}

	bool GameObjectMap::InsertElement(GameObject* gameobject)
	{
		if (gameobject == nullptr)
		{
			return false;
		}
		if (m_Count >= m_Capacity)
		{
			return false;
		}
		//Hash the name to index and get the key.
		UInt32 hash = m_HashFunction.Invoke(gameobject->GetName(), m_Capacity);
		Key & key = m_Elements[hash];

		//Check for duplicate references.
		if (key.Exists(gameobject))
		{
			return false;
		}
		//Insert element onto key. 
		key.Insert(gameobject, *this, &GameObjectMap::CreateKeyIterator);
		m_Count++;
		return true;
	}

	bool GameObjectMap::RemoveElement(GameObject* gameobject)
	{
		UInt32 hash = m_HashFunction.Invoke(gameobject->GetName(), m_Capacity);
		Key & key = m_Elements[hash];
		if (key.Remove(gameobject, *this, &GameObjectMap::DestroyKeyIterator))
		{
			m_Count--;
			return true;
		}
		return false;
	}

	void GameObjectMap::Clear()
	{
		for (UInt32 i = 0; i < m_Capacity; i++)
		{
			m_Elements[i].Clear(*this, &GameObjectMap::DestroyKeyIterator);
		}
	}

	GameObject* GameObjectMap::Find(const std::string & name)
	{
		UInt32 hash = m_HashFunction.Invoke(name, m_Capacity);
		return Find(hash, name);
	}

	GameObject* GameObjectMap::Find(UInt32 hash, const std::string & name)
	{
		if (hash >= m_Capacity)
		{
			return nullptr;
		}
		Key & key = m_Elements[hash];

		//Search through all key elements.
		KeyIterator* iter = key.GetFirst();
		while (iter != nullptr)
		{
			if (iter->element->GetName() == name)
			{
				return iter->element;
			}
			iter = iter->next;
		}

		return nullptr;
	}

	Array<GameObject*> GameObjectMap::FindAll(const std::string & name)
	{
		UInt32 hash = m_HashFunction.Invoke(name, m_Capacity);
		Key & key = m_Elements[hash];

		std::vector<GameObject*> gameobjects;

		KeyIterator* iter = key.GetFirst();
		while (iter != nullptr)
		{
			if (iter->element->GetName() == name)
			{
				gameobjects.push_back(iter->element);
			}
			iter = iter->next;
		}

		Array<GameObject*> result;
		Array<GameObject*>::Copy(gameobjects, result);
		return result;
	}

	void GameObjectMap::Resize(UInt32 aCapacity)
	{
		GameObjectMap map = GameObjectMap();
		map.m_Elements = m_Elements;
		map.m_Capacity = m_Capacity;
		map.m_Count = m_Count;
		map.m_HashFunction = m_HashFunction;
		map.m_Allocator = m_Allocator;



		m_Elements = new Key[aCapacity];
		m_Capacity = aCapacity;
		m_Count = 0;
		m_Allocator = new Memory::PoolAllocator(sizeof(KeyIterator)* (m_Capacity + 1), 0, sizeof(KeyIterator), __alignof(KeyIterator));
		Copy(map);
	}

	void GameObjectMap::Copy(GameObjectMap & aMap)
	{
		Iterator it = aMap.GetIterator();
		while (it.MoveNext())
		{
			InsertElement(*it);
		}
	}

	int GameObjectMap::InsertRange(Array<GameObject*>& aArray)
	{
		int count = 0;
		for (int i = 0; i < aArray.GetCount(); i++)
		{
			if (!InsertElement(aArray[i]))
			{
				count++;
			}
		}
		return count;
	}

	int GameObjectMap::InsertRange(std::vector<GameObject*>& aVector)
	{
		int count = 0;
		for (std::vector<GameObject*>::iterator it = aVector.begin(); it != aVector.end(); it++)
		{
			if (!InsertElement(*it))
			{
				count++;
			}
		}
		return count;
	}

	GameObjectMap::KeyIterator* GameObjectMap::CreateKeyIterator()
	{
		KeyIterator* keyIterator = (KeyIterator*)m_Allocator->Allocate(sizeof(KeyIterator), __alignof(KeyIterator));
		return Memory::Instantiator<KeyIterator>::Construct(keyIterator);
	}
	void GameObjectMap::DestroyKeyIterator(GameObjectMap::KeyIterator* aKeyIterator)
	{
		Memory::Instantiator<KeyIterator>::Destruct(aKeyIterator);
		m_Allocator->Deallocate(aKeyIterator);
	}

	UInt32 GameObjectMap::HashString(const std::string & hash, UInt32 aCapacity)
	{
		if (hash.size() == 0)
		{
			return 0;
		}
		UInt32 total = 0;

		for (int i = 0; i < hash.size(); i++)
		{
			total += hash[i];
		}
		return (total / hash.size()) % aCapacity;
	}


	// Key
	void GameObjectMap::Key::Insert(GameObject* element,GameObjectMap & aInstance, GameObjectMap::Allocator aAllocator)
	{
		if (m_First == nullptr)
		{
			
			m_First = aAllocator.Invoke(aInstance);
			m_First->element = element;
			m_First->next = nullptr;
			m_First->prev = nullptr;
			m_Last = m_First;
		}
		else if (m_First == m_Last)
		{
			m_Last = aAllocator.Invoke(aInstance);
			m_Last->element = element;
			m_Last->next = nullptr;
			m_Last->prev = nullptr;

			m_First->next = m_Last;
			m_Last->prev = m_First;
		}
		else
		{
			KeyIterator* newIterator = aAllocator.Invoke(aInstance);
			newIterator->element = element;
			newIterator->next = nullptr;
			newIterator->prev = m_Last;
			m_Last->next = newIterator;
			m_Last = newIterator;
		}
	}

	bool GameObjectMap::Key::Remove(GameObject* element, GameObjectMap & aInstance, GameObjectMap::Deallocator aDeallocator)
	{
		KeyIterator* iter = m_First;
		while (iter != nullptr)
		{
			if (iter->element == element)
			{
				KeyIterator* prev = iter->prev;
				KeyIterator* next = iter->next;

				if (iter == m_First)
				{
					m_First = next;
				}
				if (iter == m_Last)
				{
					m_Last = prev;
				}

				aDeallocator.Invoke(aInstance, iter);

				if (prev != nullptr)
				{
					prev->next = next;
				}
				if (next != nullptr)
				{
					next->prev = prev;
				}
				return true;
			}
			iter = iter->next;
		}

		return false;
	}

	bool GameObjectMap::Key::Exists(GameObject* element)
	{
		KeyIterator* iter = m_First;
		while (iter != nullptr)
		{
			if (iter->element == element)
			{
				return true;
			}
			iter = iter->next;
		}
		return false;
	}

	void GameObjectMap::Key::Clear(GameObjectMap & aInstance, GameObjectMap::Deallocator aDealllocator)
	{
		KeyIterator* iter = m_Last;

		while (iter != nullptr)
		{
			KeyIterator* next = iter->prev;
			aDealllocator.Invoke(aInstance,iter);
			iter = next;
		}

		m_First = nullptr;
		m_Last = nullptr;
	}

	//Enumerator

	bool GameObjectMap::Iterator::MoveNext()
	{
		if (m_CurrentKeyIterator == nullptr || m_CurrentKeyIterator->next == nullptr)
		{
			m_CurrentKeyIterator = nullptr;
			while (m_CurrentKeyIterator == nullptr)
			{
				m_CurrentIndex++;
				if (m_CurrentIndex >= m_Capacity)
				{
					return false;
				}
				m_CurrentKeyIterator = m_Elements[m_CurrentIndex].GetFirst();
			}
		}
		else
		{
			m_CurrentKeyIterator = m_CurrentKeyIterator->next;
		}
		return true;
	}

}