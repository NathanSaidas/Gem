#ifndef GEM_GAME_OBJECT_MAP_H
#define GEM_GAME_OBJECT_MAP_H

//============================================================
// Date:			June,	2,		2015
// Description:		Contains the class GameObjectMap and its internal dependencies.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Class.
#pragma endregion

//Basic types and API macros.
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
//GameObject
#include "../EntityComponent/GameObject.h"
//PoolAllocator.
#include "../Memory/PoolAllocator.h"

namespace Gem
{
	FORCE_EXPORT_META(GameObjectMap);
	template class GEM_API Func<UInt32, const std::string&, UInt32>;
	/** 
	* This class provides a way to store and manage gameobjects.
	* This class also provides fast search functions by name. 
	* The default hash function is O(n) based on the length of the string being hashed.
	* The indexing from the hash is O(1) time however. So if the strings a hashed during initialization and the 
	* hash code + the name of the gameobject is used. Search times can be reduced greatly.
	* If two strings collide (get the same hash code) the chaining method is used. In that case search time goes to O(n) after indexing.
	*
	* Its possible to iterate through all elements using the custom iterator. It's also possible to iterate through all elements using a functor. (Static Function Object)
	* All KeyIterators which is the internal data structure keeping the data is allocated through a custom PoolAllocator. All memory is managed internally no need to call clear.
	* 
	*/
	class GEM_API GameObjectMap : public object
	{
	private:
		RDECLARE_CLASS(GameObjectMap)
		/** This class wraps the GameObject pointer providing a reference to the next and previous elements in a Key. */
		class KeyIterator
		{
		public:
			/** A pointer to the next iterator in the linked list.*/
			KeyIterator* next;
			/** A pointer to the previous iterator in the linked list.*/
			KeyIterator* prev;
			/** A pointer to the element of this iterator.*/
			GameObject* element;
		};

		typedef Method<GameObjectMap, KeyIterator*> Allocator;
		typedef Method<GameObjectMap, void, KeyIterator*> Deallocator;

		/** This class holds a linked list of KeyIterators. Keys are used in the GameObjectMap */
		class Key
		{
		private:
			/** A pointer to the first element in the list.*/
			KeyIterator* m_First;
			/** A pointer to the last element in the list*/
			KeyIterator* m_Last;

		public:
			Key() : m_First(nullptr), m_Last(nullptr) {}
	

			/** 
			* Adds an element to the linked list.
			* @param element The element to add to the linked list.
			*/
			void Insert(GameObject* element, GameObjectMap& aInstance, Allocator aAllocator);
			/** 
			* Removes an element from the linked list.
			* @param element The element to remove
			* @return Returns true if removal was successful
			*/
			bool Remove(GameObject* element, GameObjectMap& aInstance, Deallocator aDeallocator);

			/**
			* Searchs for the element in the linked lis.
			* @param The element to search for.
			* @return Returns true if an element exists within the linked list.
			*/
			bool Exists(GameObject* element);

			/** 
			* Clears all of the iterators. Deallocating them from memory.
			*/
			void Clear(GameObjectMap& aInstance, GameObjectMap::Deallocator aDeallocator);

			/** Returns an iteator of the first element in the list.*/
			inline KeyIterator* GetFirst() const
			{
				return m_First;
			}

			/** Returns an iterator of the last element in the list.*/
			inline KeyIterator* GetLast() const
			{
				return m_Last;
			}
		};

	public:
		/** 
		* This is the hash function used internally to hash strings for the map.
		* The first param is the string to hash
		* The second param is the capacity of the GameObjectMap
		* The return should indicate index in the map and should be between 0 and m_Capacity.
		*/
		typedef Func<UInt32, const std::string&, UInt32> HashFunction;

		

		/** This class wraps pointers to elements within the map and can advance to a position.*/
		class Iterator
		{
		public:

			Iterator(UInt32 aCount, Key* aElements)
				: m_Capacity(aCount),
				m_Elements(aElements),
				m_CurrentIndex(-1),
				m_CurrentKeyIterator(nullptr)
			{}

			/** Overloaded the -> operator to directly access members of the gameobject.*/
			GameObject* operator->()
			{
				if (m_CurrentKeyIterator != nullptr)
				{
					return m_CurrentKeyIterator->element;
				}
				return nullptr;
			}

			/** Overloaded the  * operator to dereference this object and get a pointer to the element.*/
			GameObject* operator*()
			{
				if (m_CurrentKeyIterator != nullptr)
				{
					return m_CurrentKeyIterator->element;
				}
				return nullptr;
			}

			/** This is the equivalent of * operator. */
			inline GameObject* GetElement()
			{
				if (m_CurrentKeyIterator != nullptr)
				{
					return m_CurrentKeyIterator->element;
				}
				return nullptr;
			}

			/** 
			*Advances the iterator to the next element
			* @return Returns false if the iterate moves to the end of the map.
			*/
			bool MoveNext();

		private:
			/** The index of the map the iterator is currently at.*/
			SInt32 m_CurrentIndex;
			/** The internal key iterator the iterator is at.*/
			KeyIterator* m_CurrentKeyIterator;
			/** The number of elements there are.*/
			UInt32 m_Capacity;
			/** A pointer to all the elements.*/
			Key* m_Elements;
		};

		/**
		* Initialized the gameobject map with the specified capacity using the default hash function.
		* @param aCapacity The maximum number of elements there can be in the map.
		*/
		GameObjectMap(UInt32 aCapacity);
		/** 
		* Initializes the gameobject map with the specified hash function.
		* @param aCapacity The maximum number of elements there can be in the map.
		* @param aHashFunction The function used to hash strings.
		*/
		GameObjectMap(UInt32 aCapacity, HashFunction aHashFunction);
		~GameObjectMap();

		/** 
		* Adds a gameobject to the map.
		* The name of the gameobject is used to create a hashcode. This hashcode can be used to retrieve the key where
		* of where the element is in the map.
		* @param element The gameobject to insert.
		* @return Returns false if the element already exists in the map.
		*/
		bool InsertElement(GameObject* element);
		/** 
		* Removes a gameobject from the map.
		* The name of the gameobject is used to create a hashcode.
		* @param element The gameobject to insert.
		* @return Returns true if the gameobject was successfuly removed from the map.
		*/
		bool RemoveElement(GameObject* element);

		/** Clears all elements from the map but maintains the same size.*/
		void Clear();

		/** 
		* Uses the name in a hash function to point to the specific key.
		* Using the hash as the index, a linear search O(n) will be performaned on the linked list of the key
		* to find the first gameobject with the exact match of the name.
		* @param name The name of the gameobject to find.
		* @return Returns the gameobject if found or null if not found.
		*/
		GameObject* Find(const std::string & name);
		/** 
		* Uses the hash provided and the name to search for the element. See above function for details of function performance.
		* @param hash The hash to use for searching
		* @param name The name of the gameobject to find.
		* @return Returns the gameobject if found or null if not found.
		*/
		GameObject* Find(UInt32 hash, const std::string & name);

		/**
		* Uses the name in a hash function to point to specific key. From there the search will perform a linear serach
		* O(n) through all of the elements within that specific key adding them to a vector then copying the elements to an fixed size array.
		* @param name The name of the gameobjects to find.
		* @return Returns the gameobjects found. Expect an array of size 0 if nothing was found.
		*/
		Array<GameObject*> FindAll(const std::string & name);

		/** 
		* Uses the hash provided and the name to search for the element. See above function for details of function performance.
		* @param hash The hash to use for searching.
		* @param name The name of the gameobjects to find.
		* @return Returns an array of gameobjects found. Size of 0 means there was none found.
		*/
		Array<GameObject*> FindAll(UInt32 hash, const std::string & name);

		/** Resizes the map to the specified capacity. This could be an expensive operation. All of the elements have to be deallocated and reallocated.*/
		void Resize(UInt32 aCapacity);

		/** Copies all elements from the map to this map.*/
		void Copy(GameObjectMap & aMap);

		/** This is similar to copy but for arrays.
		* @return Returns the number of elements that failed to be inserted.
		*/
		int InsertRange(Array<GameObject*>& aArray);
		
		/** This is similar to copy but for vectors.
		* @return Returns the number of elements that failed to be inserted.
		*/
		int InsertRange(std::vector<GameObject*>& aVector);

		/**
		* Gets an iterator from map. Use this to iterate through the entire map.
		* @return Returns an iterator.
		*/
		inline Iterator GetIterator()
		{
			return Iterator(m_Capacity, m_Elements);
		}

		/** 
		* This method will iterate through all of the gameobjects using an GameObjectMap::Iterator
		* Each iteration will make a call to the function object parameter.
		* The function object is required to have a void return type and the first argument be the gameobject.
		*/
		template<typename ... ARGS>
		void Foreach(Func<void, GameObject*, ARGS...> func, ARGS ... args)
		{
			Iterator it = GetIterator();
			while (it.MoveNext())
			{
				func.Invoke(*it, args...);
			}
		}

		/** @return Returns the number of elements in the map.*/
		inline UInt32 GetCount() const
		{
			return m_Count;
		}

		/** @return Returns the maximum number of elements in the map.*/
		inline UInt32 GetCapacity() const
		{
			return m_Capacity;
		}
		
		/** Sets the hash function. The function pointer is null the default hash function is used instead.*/
		inline void SetHashFunction(HashFunction aHashFunction)
		{
			if (aHashFunction == nullptr)
			{
				m_HashFunction = HashString;
			}
			else
			{
				m_HashFunction = aHashFunction;
			}
		}

		/** @return Returns the hash function being used.*/
		inline HashFunction GetHashFunction() const
		{
			return m_HashFunction;
		}
		
		inline UInt32 HashString(const std::string & aName) const
		{
			return HashString(aName, m_Capacity);
		}
		

	private:
		/** Internal constructor. Does not intialize the map.*/
		GameObjectMap();

		/** The amount of elements the map currently has.*/
		UInt32 m_Count;
		/** The amount of elements the map can contain.*/
		UInt32 m_Capacity;
		/** An array of keys pointing to elements the map contains.*/
		Key * m_Elements;
		/** A function used for hashing.*/
		HashFunction m_HashFunction;
		/** An allocator used for creating / destroying keys.*/
		Memory::PoolAllocator * m_Allocator;

		/** This method creates a KeyIterator using the allocator.*/
		KeyIterator* CreateKeyIterator();
		/** This method destroys a KeyIterator using the custom allocator.*/
		void DestroyKeyIterator(KeyIterator* aKeyIterator);

		/** This is the default hash function used.*/
		static UInt32 HashString(const std::string & hash, UInt32 aCapacity);

		friend class Reflection::MetaObject<GameObjectMap>;
	};

	TYPE_DEFINE(GameObjectMap)
}

#endif // GEM_GAME_OBJECT_MAP