#ifndef G_POINTER_H
#define G_POINTER_H

#include "G_Memory.h"
#include <boost\static_assert.hpp>
#include <boost\type_traits.hpp>
#include "../Reflection/G_Class.h"

namespace Gem
{
    

    template<class T>
    class Pointer //: public Reflection::Class
    {
        //BOOST_STATIC_ASSERT((boost::is_base_of<Object,T>::value));
        static_assert(boost::is_base_of<Object,T>::value || boost::is_base_of<Reflection::Type,T>::value , "Object of type T is not derived of Object");
        //static_assert(false,"derp");
    public:
        
        Pointer(bool aAllocate = true)
        {
            if(aAllocate == true)
            {
                m_Data = Memory::instantiate<T>();
                m_Owner = this;
                m_Next = nullptr;
                //m_ReferenceCount = 1;
            }
            else
            {
                m_Data = nullptr;
                m_Owner = this;
                m_Next = nullptr;
                //m_ReferenceCount = 0;
            }
        }
        //For construction of managed Type
        Pointer(Reflection::Type * aType)
        {
            if(aType != nullptr)
            {
                m_Data = aType;
                m_Owner = this;
                m_Next = nullptr;
            }
            else
            {
                m_Data = nullptr;
                m_Owner = this;
                m_Next = nullptr;
            }
        }

        Pointer(Pointer & aPtr)
        {
            assign(aPtr);
            m_Next = nullptr;
        }
        Pointer(T & aValue)
        {
            m_Data = Memory::instantiate<T>();
            m_Owner = this;
            //m_ReferenceCount = 1;

            *m_Data = aValue;
        }
        ~Pointer()
        {
            if(m_Owner != nullptr)
            {
                //if(m_Owner == this && m_References.size() > 0)
                //{
                //    m_Owner = m_References[0];
                //    m_Owner->m_ReferenceCount = m_ReferenceCount;
                //    m_Owner->transferOwnership(m_References);
                //}

                if(m_Owner == this && hasNext() == true)
                {
                    m_Owner = getNext();
                    m_Owner->transferOwnership(*this);
                }

                m_Owner->releaseReference(*this);
            }
            if(count() == 1)
            {
                m_Data = Gem::Memory::destroy<T>(m_Data);
            }
        }


        //Releases the Data from memory and updates all references to it
        virtual void release()
        {
            if(m_Data == nullptr)
            {
                return;
            }
            if(m_Owner == this)
            {
                Pointer<T> * ptr = getNext();
                while(ptr != nullptr)
                {
                    Pointer<T> * previous = ptr;

                    ptr->m_Data = nullptr;
                    ptr->m_Owner = ptr;;
                    ptr = ptr->getNext();
                    previous->setNext(nullptr);
                    
                }
                //for(int i = 0; i < m_References.size(); i++)
                //{
                //    m_References[i]->m_Data = nullptr;
                //    m_References[i]->m_Owner = nullptr;
                //}
                m_Data = Memory::destroy<T>(m_Data);
                m_Owner = this;
                m_Next = nullptr;
                //m_References.clear();
            }
            else if(m_Owner != nullptr)
            {
                m_Owner->release();
            }

        }
        virtual void allocate()
        {
            //Already allocated
            if(m_Data != nullptr)
            {
                return;
            }
            m_Data = Memory::instantiate<T>();
            //m_ReferenceCount = 1;
        }

        virtual void assign(Pointer<T> & aPtr)
        {
            m_Owner = &aPtr;
            if(m_Owner != nullptr)
            {
                m_Owner->addReference(*this);
                m_Data = m_Owner->m_Data;
            }
            else
            {
                allocate();
            }
        }

        int count()
        {
            if(m_Data == nullptr)
            {
                return 0;
            }
            //return m_Owner == nullptr ? m_ReferenceCount : m_Owner->m_ReferenceCount;
            int count = 0;
            Pointer<T> * ptr = m_Owner;
            while(ptr != nullptr)
            {
                count ++;
                ptr = ptr->getNext();
            }
            return count;
        }
        T * ref()
        {
            return m_Data;
        }
        T data()
        {
            return *m_Data;
        }
        bool isAlive()
        {
            return m_Data == nullptr ? false : true;
        }
        bool hasNext()
        {
            return m_Next == nullptr ? false : true;
        }
        bool isOwner()
        {
            return m_Owner == this ? true : false;
        }
        

        Pointer<T> & operator=(Pointer<T> & aValue)
        {
            if(m_Owner != nullptr)
            {
                if(m_Owner == this && hasNext())
                {
                    //m_Owner = m_References[0];
                    //m_Owner->m_ReferenceCount = m_ReferenceCount;
                    //m_Owner->transferOwnership(m_References);
                    m_Owner = getNext();
                    m_Owner->transferOwnership(*this);
                }
                m_Owner->releaseReference(*this);
            }
            //assign
            assign(aValue);
            return *this;
        }

        //Pointer<T> operator=(Pointer<T> aValue)
        //{
        //    return *this = aValue;
        //}

        T operator = (T aValue)
        {
            if(m_Data == nullptr)
            {
                return aValue;
            }
            return (*m_Data) = aValue;
        }
        //virtual Reflection::Type * getType()
        //{
        //    return Reflection::Type::create("Pointer",Reflection::TypeID::POINTER,sizeof(Pointer<T>),Reflection::Class::getType());
        //}
    private:
        //void addReference(Pointer<T> & aReference)
        //{
        //    m_ReferenceCount ++;
        //    m_References.push_back(&aReference);
        //}

        void addReference(Pointer<T> & aReference)
        {
            // ++;

            //Get the next pointer
            Pointer<T> * next = getNext();
            //If it was null or equal to this (this owner)
            //This this next to the reference
            if(next == nullptr || next == this)
            {
                setNext(&aReference);
            }
            else
            {
                //Else Traverse through the list of next pointers till a valid spot is found
                //valid = where null || this || temp = current
                do
                {
                    Pointer<T> * tmpPtr = next->getNext();
                    if(tmpPtr == nullptr || tmpPtr == next || tmpPtr == this)
                    {
                        next->setNext(&aReference);
                        //next->getNext()->setNext(&aReference);
                        break;
                    }
                    next = next->getNext();
                } while(next != nullptr);
            }
        }

        //void releaseReference(Pointer<T> & aReference)
        //{
        //    m_ReferenceCount --;
        //    //Remove the reference from the list
        //    for(int i = m_References.size() - 1; i >= 0; i--)
        //    {
        //        if(m_References[i] == &aReference)
        //        {
        //            m_References.erase(m_References.begin() + i);
        //            break;
        //        }
        //    }
        //    if(m_ReferenceCount <= 0)
        //    {
        //        if(m_Data != nullptr)
        //        {
        //            m_Data = Memory::destroy<T>(m_Data);
        //        }
        //        m_ReferenceCount = 0;
        //    }
        //}

        //Gets called on the owner
        void releaseReference(Pointer<T> & aExpired)
        {
            //Reduce count
            //m_ReferenceCount--;
            
            ////Get the next pointer
            ////Traverse through all of the references using the getNext() method
            ////Find : where getNext()->getNext() == aExpired
            ////and do getNext()->setNext(aNext)
            //Pointer<T> * next = getNext();
            //Pointer<T> * newNext = aExpired.getNext();
            //if(newNext == nullptr || newNext == &aExpired)
            //{
            //    newNext = this;
            //}
            ////Quick check to see if the immediate reference is our next
            //if(next == &aExpired)
            //{
            //    setNext(newNext);
            //    return;
            //}
            ////If not then traverse through the references
            //else
            //{
            //    while(next != nullptr && next != this)
            //    {
            //        Pointer<T> * tmpPtr = next->getNext();
            //        if(tmpPtr != nullptr && tmpPtr == &aExpired)
            //        {
            //            next->setNext(newNext);
            //            break;
            //        }
            //        next = tmpPtr;
            //    }
            //}

            //traverse through list
            //if anyone is pointing to the old owner
            //redirect them to null
            //lol what
            //int refCount = count();
            //if(refCount <= 0)
            //{
            //    if(m_Data != nullptr)
            //    {
            //        m_Data = Memory::destroy<T>(m_Data);
            //    }
            //    //m_ReferenceCount = 0;
            //}
        }

        //void transferOwnership(std::vector<Pointer<T>*> & aReferences)
        //{
        //    m_References.clear();
        //    for(int i = 0; i < aReferences.size(); i++)
        //    {
        //        if(aReferences[i] == this)
        //            continue;
        //
        //        m_References.push_back(aReferences[i]);
        //    }
        //
        //    for(int i = m_References.size() -1 ; i >= 0; i--)
        //    {
        //        if(m_References[i] == this)
        //        {
        //            m_References.erase(m_References.begin() + i);
        //            continue;
        //        }
        //        m_References[i]->m_Owner = this;
        //    }
        //    m_Owner = this;
        //}

        //gets called on the new owner
        //inclusive transfership
        void transferOwnership(Pointer<T> & aPreviousOwner)
        {
            //m_ReferenceCount = aPreviousOwner.m_ReferenceCount;
            //Traverse through link list
            //-Set everyones owner to this
            //-adjust the last member of the link list to point to this
            
            Pointer<T> * iter = aPreviousOwner.getNext();
            while(iter != nullptr)
            {
                iter->m_Owner = this;
                //skip new owner ptr check
                if(iter->getNext() == this)
                {
                    Pointer<T> * tmpPtr = iter->getNext()->getNext();
                    if(tmpPtr != nullptr)
                    {
                        iter->setNext(tmpPtr);
                        setNext(aPreviousOwner.getNext());
                        //skip owner and go to tmp
                        iter = tmpPtr;
                    }
                    else //end of list
                    {
                        break;
                    }
                }
                iter = iter->getNext();
            }
        }

        

        Pointer<T> * getNext()
        {
            return m_Next;
        }
        void setNext(Pointer<T> * aNext)
        {
            m_Next = aNext;
        }


        //Pointer to data
        T * m_Data;
        //Pointer to owner / top of the list
        Pointer<T> * m_Owner;
        //Pointer to next on the list
        Pointer<T> * m_Next;

        //Getting rid of
        //int m_ReferenceCount;
        //Getting rid of
        //std::vector<Pointer<T> *> m_References;
    };

    template<class T>
    inline bool instanceOf(Pointer<T> & aBase, Pointer<T> & aDerived)
    {
        return Reflection::Type::instanceOf(aBase.ref(),aDerived.ref());
    }
}


#endif