#ifndef G_LIST_H
#define G_LIST_H

#include "../../Primitives/G_PrimitiveObjects.h"
#include "G_ICollection.h"
#include "../../Reflection/G_Reflection.h"
namespace Gem
{
	namespace Collections
	{
		template<class T>
		class List : public ICollection
		{
		public:
			List()
			{
				m_Count = 0;

			}
			Pointer<Reflection::Type> getType() override
			{
				return typeOf("ICollection");
			}
			
			class Iterator
			{
			public:
				Iterator():m_Current(nullptr),m_Next(nullptr) {}
				Iterator(const Iterator & aIterator) :m_Current(aIterator.m_Current), m_Next(aIterator.m_Next){}
				~Iterator(){}

				T * ref()
				{
					return m_Current;
				}
				bool hasNext()
				{
					return m_Next != nullptr;
				}
				Iterator * next()
				{
					return m_Next;
				}
			protected:
				T * m_Current;
				Iterator * m_Next;
				friend List;
			};

			T * at(int aIndex)
			{
				int currentIndex = 0;
				Iterator * cleanUp = iterator();
				Iterator * start = cleanUp;
				while (start->hasNext())
				{
					start = start->next();
					if (currentIndex == aIndex)
					{
						T * element = start->ref();
						Memory::destroy<Iterator>(cleanUp);
						return start->ref();
					}
					currentIndex++;
				}
				return nullptr;

			}
			Iterator * iterator()
			{
				Iterator * start = Memory::instantiate<Iterator>();
				start->m_Next = m_Begin.ref();
				return start;
			}
			T * begin()
			{
				return m_Begin.ref()->ref();
			}
			T * end()
			{
				return m_End.ref()->ref();
			}

			void add(T * aElement)
			{
				ASSERT(aElement != nullptr);
				//if (aElement == nullptr)
				//{
				//	return;
				//}
				if (m_Begin.ref()->ref() == nullptr)
				{
					m_Begin.ref()->m_Current = aElement;
					return;
				}
				else if (m_Begin.ref()->ref() != nullptr && m_End.ref()->next() == nullptr)
				{
					Iterator * iter = Memory::instantiate<Iterator>();
					iter->m_Current = aElement;
					m_Begin.ref()->m_Next = iter;
					m_End.ref()->m_Next = iter;
				}
				else if (m_Begin.ref()->ref() != nullptr && m_End.ref()->next() != nullptr)
				{
					Iterator * iterEnd = m_End.ref()->next();
					Iterator * iter = Memory::instantiate<Iterator>();
					iter->m_Current = aElement;
					iterEnd->m_Next = iter;
					m_End.ref()->m_Next = iter;
				}
			}

			bool remove(T * aElement)
			{
				if (aElement == nullptr)
				{
					return false;
				}
				Iterator * cleanUp = iterator();
				Iterator * start = cleanUp;
				Iterator * previous = nullptr;
				while (start->hasNext())
				{
					previous = start;
					start = start->next();
					if (start == nullptr)
					{
						Memory::destroy<Iterator>(cleanUp);
						return false;
					}

					if (start->ref() == aElement)
					{
						if (m_Begin.ref()->ref() == aElement)
						{
							m_Begin.ref()->m_Current = start->m_Next->m_Current;
						}
						previous->m_Next = start->m_Next;
						return true;
					}
				}
				return false;
			}

			void removeAt(int aIndex)
			{
				int currentIndex = 0;
				Iterator * start = iterator();
				Iterator * previous = nullptr;
				while (start->hasNext())
				{
					previous = start;
					start = start->next();
					if (currentIndex == aIndex)
					{
						previous->m_Next = start->m_Next;
						return;
					}
					currentIndex++;
				}
				return;
			}


		private:
			int m_Count;
			Pointer<Iterator> m_Begin;
			Pointer<Iterator> m_End;
		};
	}
}
//GEM_CLASS(Collections::List,Object)

#endif