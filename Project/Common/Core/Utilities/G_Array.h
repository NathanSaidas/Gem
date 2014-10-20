#ifndef OL_ARRAY_H
#define OL_ARRAY_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "../Memory/G_Memory.h"
#include "../Reflection/G_Reflection.h"

namespace Gem
{
    template<class TYPE,class DEREFERENCED_TYPE>
    class Array
    {
    public:
        Array()
        {
            m_Size = 0;
            m_Array = nullptr;
        }
        ~Array()
        {
            //m_Array = Memory::destroy<TYPE>(m_Array,m_Size);
        }

        
        int count()
        {
            return m_Size;
        }

        void addElement()
        {
            //Increase the size
            int prevSize = m_Size;
            m_Size ++;

            //Allocate for new size
            TYPE * newArray = Memory::instantiate<TYPE>(m_Size);
            //Copy elements over
            for(int i = 0; i < prevSize; i++)
            {
                newArray[i] = m_Array[i];
            }
            //Deallocate the old array
            m_Array = Memory::destroy<TYPE>(m_Array,prevSize);
            m_Array = newArray;

            //m_Array[m_Size -1] = Memory::instantiate<DEREFERENCED_TYPE>();

        }
        
        void addElement(TYPE aValue)
        {
            //Increase the size
            int prevSize = m_Size;
            m_Size ++;

            //Allocate for new size
			TYPE * newArray = nullptr; //Memory::instantiate<TYPE>(m_Size);
            //Copy elements over
            for(int i = 0; i < prevSize; i++)
            {
                newArray[i] = m_Array[i];
            }
            //Deallocate the old array
			m_Array = nullptr;//Memory::destroy<TYPE>(m_Array,prevSize);
            m_Array = newArray;

            m_Array[m_Size - 1] = aValue;
        }

        void removeAt(unsigned int aIndex)
        {
            if((aIndex >= 0 && aIndex < m_Size) == false)
            {
                return;
            }
            //Store the previous size
            int prevSize = m_Size;
            m_Size --;


            //Memory::destroy<DEREFERENCED_TYPE>(m_Array[aIndex]);
            //Allocate for new array
			TYPE * newArray = nullptr;//Memory::instantiate<TYPE>(m_Size);
            int indexModifier = 0;
            //Copy every element except the one at the specified index
            for(int i = 0; i < prevSize; i++)
            {
                if(i == aIndex)
                {
                    indexModifier++;
                    continue;
                }
                newArray[i - indexModifier] = m_Array[i];
            }
			m_Array = nullptr; // Memory::destroy<TYPE>(m_Array, prevSize);
            m_Array = newArray;

        }
        void removeElement(TYPE aValue)
        {
            int prevSize = m_Size;
            m_Size --;

            //Check if it exists
            bool exists = false;
            for(int i = 0; i < prevSize; i++)
            {
                if(m_Array[i] == aValue)
                {
                    //Memory::destroy<DEREFERENCED_TYPE>(m_Array[i]);
                    exists = true;
                    break;
                }
            }

            if(exists == false)
            {
                return;
            }

            
            TYPE * newArray = Memory::instantiate<TYPE>(m_Size);
            int indexModifier = 0;
            for(int i = 0; i < prevSize; i++)
            {
                if(m_Array[i] == aValue)
                {
                    indexModifier++;
                    continue;
                }
                newArray[i - indexModifier] = m_Array[i];
            }

            m_Array = Memory::destroy<TYPE>(m_Array,prevSize);
            m_Array = newArray;

        }

        TYPE & at(int aIndex)
        {
            return m_Array[aIndex];
        }


        TYPE & operator[](int aIndex)
        {
            return m_Array[aIndex];
        }

        Reflection::Type * getType()
        {
            return Reflection::Type::create("Array",Reflection::TypeID::ARRAY,sizeof(Array<TYPE,DEREFERENCED_TYPE>),Object::getType());
        }
        

    private:
        int m_Size;
        TYPE * m_Array;


        
    };

}

        

#endif