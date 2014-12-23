#ifndef G_LIST_H
#define G_LIST_H

#include "G_Collection.h"
#include "G_Iterators.h"
#include "../../Memory/G_Memory.h"
#include "../../Reflection/G_Function.h"
#include <type_traits>

namespace Gem
{
    namespace Collections
    {
        //The list class outperforms the std::vector class in allocations and iterations by far no matter the size.
        //The list class underperformns the std::vector class in deallocations above size 1000 and is most efficient compared to std::vector at size 30000 (10% slower)
        //Deallocation Efficiency Chart
        //Size--|-vec/list--|-inverse
        //20000 |   0.80    | 0.20
        //25000 |   0.83    | 0.17
        //30000 |   0.90    | 0.10
        //35000 |   0.72    | 0.28
        //40000 |   0.85    | 0.15


        ///Add elements onto the list
        ///Remove elements from the list by reference equals / equals
        ///Remove elements from the list by index
        ///Remove by condition
        ///Get Elements off the list by index
        ///Get an iterator for the beginning and end of the list (BiDirectional)
        ///Clear all items off the list
        template<class T>
        class List : public Collection
        {
        public:
            List()
            {
                m_Front = nullptr;
                m_Back = nullptr;
                m_Count = 0;
            }
            ~List()
            {
                Clear();
            }

            Iterator<T> * GetFront()
            {
                return m_Front;
            }
            Iterator<T> * GetBack()
            {
                return m_Back;
            }
            int Count()
            {
                return m_Count;
            }

            void Add(T aElement)
            {
                if (m_Front == nullptr)
                {
					//m_Front = MemoryManager::Instance()->AllocateIterator<T>();
                    m_Front = Memory::Instantiate<Iterator<T>>();
                    m_Front->Initialize(nullptr, nullptr, aElement);
                    m_Back = m_Front;
                }
                else if (m_Front == m_Back)
                {
					//m_Back = MemoryManager::Instance()->AllocateIterator<T>();
                    m_Back = Memory::Instantiate<Iterator<T>>();
                    m_Back->Initialize(nullptr, m_Front, aElement);
                    m_Front->Initialize(m_Back, nullptr, m_Front->Current());
                }
                else
                {
					Iterator<T> * iterator = Memory::Instantiate<Iterator<T>>();
					//Iterator<T> * iterator = MemoryManager::Instance()->AllocateIterator<T>();
                    iterator->Initialize(nullptr, m_Back, aElement);
                    m_Back->Initialize(iterator, m_Back->GetPrevious(), m_Back->Current());
                    m_Back = iterator;
                }
                m_Count++;
            }
			void Add(int aMemoryHandle, T aElement)
			{
				if (m_Front == nullptr)
				{
					//m_Front = MemoryManager::Instance()->AllocateIterator<T>();
					m_Front = Memory::Instantiate<Iterator<T>>();
					m_Front->Initialize(aMemoryHandle,nullptr, nullptr, aElement);
					m_Back = m_Front;
				}
				else if (m_Front == m_Back)
				{
					//m_Back = MemoryManager::Instance()->AllocateIterator<T>();
					m_Back = Memory::Instantiate<Iterator<T>>();
					m_Back->Initialize(aMemoryHandle,nullptr, m_Front, aElement);
					m_Front->Initialize(m_Back, nullptr, m_Front->Current());
				}
				else
				{
					//Iterator<T> * iterator = MemoryManager::Instance()->AllocateIterator<T>();
					Iterator<T> * iterator = Memory::Instantiate<Iterator<T>>();
					iterator->Initialize(aMemoryHandle,nullptr, m_Back, aElement);
					m_Back->Initialize(iterator, m_Back->GetPrevious(), m_Back->Current());
					m_Back = iterator;
				}
				m_Count++;
			}
           

            T At(int aIndex)
            {
                Iterator<T> * enumerator = m_Front;
                int counter = 0;
                while (enumerator != nullptr)
                {
                    if (counter == aIndex)
                    {
                        return enumerator->Current();
                    }
                    counter++;
                    enumerator = enumerator->GetNext();
                }
                return nullptr;
            }

            T operator[](int aIndex)
            {
                Iterator<T> * enumerator = m_Front;
                int counter = 0;
                while (enumerator != nullptr)
                {
                    if (counter == aIndex)
                    {
                        return enumerator->Current();
                    }
                    counter++;
                    enumerator = enumerator->GetNext();
                }
                return 0;
            }

            bool Remove(T aElement)
            {
                Iterator<T> * enumerator = m_Front;
                while (enumerator != nullptr)
                {
                    if (enumerator->Current() == aElement)
                    {
                        ///If the element to remove is the first element in the list.
                        ///If the list had more than 1 element to begin with we initalize what would be the second element in the list
                        ///To now become the first element in the list
                        if (aElement == m_Front->Current())
                        {
							
                            Iterator<T> * first = m_Front->GetNext();
							//MemoryManager::Instance()->DeallocateIterator<T>(m_Front);
							Memory::Destroy<Iterator<T>>(m_Front);
							if (m_Count == 1)
							{
								m_Back = nullptr;
							}
							else if (m_Count == 2)
							{
								std::cout << "ye";
							}
                            m_Front = first;
                            //Is the list not empty?
                            if (m_Front != nullptr)
                            {
                                m_Front->Initialize(m_Front->GetNext(), nullptr, m_Front->Current());
                            }
                        }
                        ///If the element to remove is the last element in the list
                        ///We remove it. If the list had more than 1 element to begin with we initialize the previous element to become the last
                        else if (aElement == m_Back->Current())
                        {
                            Iterator<T> * previous = m_Back->GetPrevious();
							//MemoryManager::Instance()->DeallocateIterator<T>(m_Back);
							Memory::Destroy<Iterator<T>>(m_Back);
                            m_Back = previous;
                            if (m_Back != nullptr)
                            {
                                m_Back->Initialize(nullptr, m_Back->GetPrevious(), m_Back->Current());
                            }
                        }
                        else
                        {
                            Iterator<T> * previous = enumerator->GetPrevious();
                            Iterator<T> * next = enumerator->GetNext();
							//MemoryManager::Instance()->DeallocateIterator<T>(enumerator);
							Memory::Destroy<Iterator<T>>(enumerator);
                            if (previous != nullptr)
                            {
                                previous->Initialize(next, previous->GetPrevious(), previous->Current());
                            }
                            if (next != nullptr)
                            {
                                next->Initialize(next->GetNext(), previous, next->Current());
                            }
                            
                        }
                        m_Count--;
                        return true;
                    }
                    enumerator = enumerator->GetNext();
                }
            }

            bool RemoveAt(int aIndex)
            {
                int count = 0;
                Iterator<T> * enumerator = m_Front;
                while (enumerator != nullptr)
                {
                    if (count == aIndex)
                    {
                        if (enumerator == m_Front)
                        {
                            Iterator<T> * first = m_Front->GetNext();
							//MemoryManager::Instance()->DeallocateIterator<T>(m_Front);
							Memory::Destroy<Iterator<T>>(m_Front);
                            m_Front = first;
                            //Is the list not empty?
                            if (m_Front != nullptr)
                            {
                                m_Front->Initialize(m_Front->GetNext(), nullptr, m_Front->Current());
                            }
							else
							{
								m_Back = nullptr;
							}
                        }
                        ///If the element to remove is the last element in the list
                        ///We remove it. If the list had more than 1 element to begin with we initialize the previous element to become the last
                        else if (enumerator == m_Back)
                        {
                            Iterator<T> * previous = m_Back->GetPrevious();
							//MemoryManager::Instance()->DeallocateIterator<T>(m_Back);
							Memory::Destroy<Iterator<T>>(m_Back);
                            m_Back = previous;
                            if (m_Back != nullptr)
                            {
                                m_Back->Initialize(nullptr, m_Back->GetPrevious(), m_Back->Current());
                            }
							else
							{
								m_Back = nullptr;
							}
                        }
                        else
                        {
                            Iterator<T> * previous = enumerator->GetPrevious();
                            Iterator<T> * next = enumerator->GetNext();
							//MemoryManager::Instance()->DeallocateIterator<T>(enumerator);
							Memory::Destroy<Iterator<T>>(enumerator);
                            if (previous != nullptr)
                            {
                                previous->Initialize(next, previous->GetPrevious(), previous->Current());
                            }
                            if (next != nullptr)
                            {
                                next->Initialize(next->GetNext(), previous, next->Current());
                            }

                        }
                        m_Count--;
                        return true;
                    }
                    enumerator = enumerator->GetNext();
                }
                return false;
            }

            ///Removes elements from the list where the condition is met
			///Use with Primitives only.
            bool RemoveWhere(Reflection::Func<bool,T*,T*> & aCondition, T * aElement)
            {
                if (aElement == nullptr)
                {
                    return false;
                }
                Iterator<T> * enumerator = m_Front;
                bool conditionMet = false;
                while (enumerator != nullptr)
                {
					
					bool conditionResult = aCondition.Invoke(aElement, enumerator->Current());
                    if (conditionResult == true)
                    { 
                        if (enumerator == m_Front)
                        {
                            Iterator<T> * next = m_Front->GetNext();
							//MemoryManager::Instance()->DeallocateIterator<T>(m_Front);
							Memory::Destroy<Iterator<T>>(m_Front);
                            m_Front = next;
                            if (m_Front != nullptr)
                            {
                                m_Front->Initialize(m_Front->GetNext(), nullptr, m_Front->Current());
                            }
                        }
                        else if (enumerator == m_Back)
                        {
                            Iterator<T> * previous = m_Back->GetPrevious();
							//MemoryManager::Instance()->DeallocateIterator<T>(m_Back);
							Memory::Destroy<Iterator<T>>(m_Back);
                            m_Back = previous;
                            if (m_Back != nullptr)
                            {
                                m_Back->Initialize(nullptr, m_Back->GetPrevious(), m_Back->Current());
                            }
                        }
                        else
                        {
                            Iterator<T> * next = enumerator->GetNext();
                            Iterator<T> * previous = enumerator->GetPrevious();
							//MemoryManager::Instance()->DeallocateIterator<T>(enumerator);
							Memory::Destroy<Iterator<T>>(enumerator);
                            next->Initialize(next->GetNext(), previous, next->Current());
                            previous->Initialize(next, previous->GetPrevious(), previous->Current());
                            enumerator = previous;
                        }
                        m_Count--;
                        conditionMet = true;
                    }
                    enumerator = enumerator->GetNext();
                }
                
                return conditionMet;
            }

			///Destroys all the iterators, Does not affect elements at all
			///Iterators are destroyed immediately.
            void Clear()
            {
                if (m_Front && m_Back == nullptr)
                {
                    return;
                }

                if (m_Front != nullptr)
                {
                    Iterator<T> * enumerator = m_Front;
                    while (enumerator != nullptr)
                    {
                        Iterator<T> * next = enumerator;
                        enumerator = enumerator->GetNext();
						//MemoryManager::Instance()->DeallocateIterator<T>(next);
						Memory::Destroy<Iterator<T>>(next);
                    }
                }
                else if (m_Back != nullptr)
                {
                    Iterator<T> * enumerator = m_Back;
                    while (enumerator != nullptr)
                    {
                        Iterator<T> * previous = enumerator;
                        enumerator = enumerator->GetPrevious();
						//MemoryManager::Instance()->DeallocateIterator<T>(previous);
						Memory::Destroy<Iterator<T>>(previous);
                    }
                }
            }

			//Flags all elements for GC
            template<class _T>
            void DestroyAll()
            {
				//Iterator<T> * enumerator = m_Front;
				//while (enumerator != nullptr)
				//{
				//	Iterator<T> * temp = enumerator;
				//	enumerator = enumerator->GetNext();
				//	if (temp != nullptr)
				//	{
				//		if (temp->SubObjectHandle() == 0)
				//		{
				//			Memory::GCFlag(MemoryHandle<Reflection::Primitive>(temp->SubObjectHandle(), temp->Current()));
				//		}
				//		else
				//		{
				//			Memory::DestroyHandle(MemoryHandle<Reflection::Primitive>(temp->SubObjectHandle(), temp->Current()));
				//		}
				//		if (temp->MemoryHandle() == 0)
				//		{
				//			MemoryManager::Instance()->GCFlag<T>(temp);
				//		}
				//		else
				//		{
				//			MemoryManager::Instance()->DeallocateIterator<T>(temp);
				//		}
				//	}
				//}
                //m_Count = 0;
				//m_Front = nullptr;
				//m_Back = nullptr;
            }

			bool Contains(T aValue)
			{
				Iterator<T> * enumerator = m_Front;
				while (enumerator != nullptr)
				{
					if (enumerator->Current() == aValue)
					{
						return true;
					}
					enumerator = enumerator->GetNext();
				}
				return false;
			}
				

        private:
			///A reference to the front of the list
            Iterator<T> * m_Front;
			///A reference to the back of the list
            Iterator<T> * m_Back;
			///The amount of elements contained in the list
            int m_Count;
        };
    }
}

#endif