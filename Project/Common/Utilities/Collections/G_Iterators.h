#ifndef G_ITERATORS_H
#define G_ITERATORS_H

#pragma region CHANGE LOG
/* November,11,2014 - Nathan Hanlan, Added and implemented Bi Directional Iterator (Iterator), Forward Iterator, Backward Iterator
*
*/
#pragma endregion CHANGE LOG

#include "../../G_Object.h"

namespace Gem
{
    namespace Collections
    {
        ///Bi Directional iterator
        //Points to an address in memory
        //Points to the next member
        //Points to the previous member
        template<class T>
        class Iterator : public object
        {
        public:
            Iterator()
            {
				m_MemoryHandle = 0;
				m_SubObjectHandle = 0;
                m_Next = nullptr;
                m_Previous = nullptr;
            }
			~Iterator()
			{

			}
            //Returns the current pointer
            T Current()
            {
                return m_Current;
            }
            ///Gets the next iterator in the list
            Iterator<T> * GetNext()
            {
                return m_Next;
            }
            ///Gets the previous iterator in the list
            Iterator<T> * GetPrevious()
            {
                return m_Previous;
            }
            ///Sets the next iterator in the list
            void SetNext(Iterator<T> * aNext)
            {
                m_Next = aNext;
            }
            //Sets the previous iterator in the list
            void SetPrevious(Iterator<T> * aPrevious)
            {
                m_Previous = aPrevious;
            }
            ///Initialize the iterator with this function
            void Initialize(Iterator<T> * aNext, Iterator<T> * aPrevious, T aCurrent)
            {
                m_Next = aNext;
                m_Previous = aPrevious;
                m_Current = aCurrent;
            }
			void Initialize(int aSubObjectHandle,Iterator<T> * aNext, Iterator<T> * aPrevious, T aCurrent)
			{
				m_Next = aNext;
				m_Previous = aPrevious;
				m_Current = aCurrent;
				m_SubObjectHandle = aSubObjectHandle;
			}
			int MemoryHandle()
			{
				return m_MemoryHandle;
			}
			int SubObjectHandle()
			{
				return m_SubObjectHandle;
			}
		protected:
			void SetHandle(int aHandle)
			{
				m_MemoryHandle = aHandle;
			}
			int GetHandle()
			{
				return m_MemoryHandle;
			}
        private:
            ///The data this iterator points to
            T m_Current;
            ///The next iterator in the list
            Iterator<T> * m_Next;
            ///The previous iterator in the list
            Iterator<T> * m_Previous;
			///The location where the Iterator was allocated
			int m_MemoryHandle;
			int m_SubObjectHandle;

			friend class MemoryManager;
        };


        ///Points to the next iterator in the list
        template<class T>
        class ForwardIterator : public object
        {
        public:
            ForwardIterator()
            {
                m_Current = nullptr;
                m_Next = nullptr;
            }
            //Returns the current pointer
            T Current()
            {
                return m_Current;
            }
            ///Gets the next iterator in the list
            ForwardIterator<T> * GetNext()
            {
                return m_Next;
            }
            ///Sets the next pointer
            void SetNext(ForwardIterator<T> * aNext)
            {
                m_Next = aNext;
            }
            ///Initialize the iterator with this function
            void Initialize(ForwardIterator<T> * aNext, T aCurrent)
            {
                m_Next = aNext;
                m_Current = aCurrent;
            }
        private:
            ///The data this iterator points to in memory
            T m_Current;
            ///The next iterator in the list
            ForwardIterator<T> * m_Next;
        };

        ///Points to the previous iterator in the list
        template<class T>
        class BackwardIterator : public object
        {
        public:
            BackwardIterator()
            {
                m_Current = nullptr;
                m_Previous = nullptr;
            }
            //Returns the current pointer
            T Current()
            {
                return m_Current;
            }
            ///Gets the next iterator in the list
            BackwardIterator<T> * GetPrevious()
            {
                return m_Previous;
            }
            ///Sets the next pointer
            void SetPrevious(BackwardIterator<T> * aPrevious)
            {
                m_Previous = aPrevious;
            }
            ///Initialize the iterator with this function
            void Initialize(BackwardIterator<T> * aPrevious, T aCurrent)
            {
                m_Previous = aNext;
                m_Current = aCurrent;
            }
        private:
            ///The data this iterator points to in memory
            T m_Current;
            ///The next iterator in the list
            BackwardIterator<T> * m_Previous;
        };
    }
}

#endif