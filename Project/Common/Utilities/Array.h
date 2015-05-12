#ifndef GAME_ENGINE_ARRAY_H
#define GAME_ENGINE_ARRAY_H

//============================================================
// Date:			May,	11,		2015
// Description:		Contains a template class called Array. Helps manage dynamic arrays
//============================================================

#include "../Core/GemAPI.h"
#include "../Core/ValueTypes.h"
#include <vector>

namespace Gem
{

    //A basic class for array data types.
    //Any memory allocated with Allocate method needs to be released with the Release method to prevent memory leaks.
    template<typename TYPE>
    class GEM_API Array
    {
    public:
        Array()
        {
            m_Elements = nullptr;
            m_Count = 0;
        }
        Array(UInt32 aElements)
        {
            Allocate(aElements);
        }
        ~Array()
        {
           
        }

        static void Copy(const Array & aSource, Array & aDest)
        {
            aDest.Allocate(aSource.GetCount());
            for (unsigned int i = 0; i < aSource.GetCount(); i++)
            {
                aDest[i] = aSource[i];
            }
        }

		static void Copy(const std::vector<TYPE> & aSource, Array & aDest)
		{
			aDest.Allocate(aSource.size());
			for (unsigned int i = 0; i < aSource.size(); i++)
			{
				aDest[i] = aSource[i];
			}
		}

        UInt32 GetCount() const
        {
            return m_Count;
        }

        void Allocate(UInt32 aElements)
        {
            if (aElements == 0)
            {
                Release();
                m_Count = 0;
                return;
            }
            TYPE * newElements = new TYPE[aElements];
            if (m_Elements != nullptr)
            {
                //Copy Elements
                for (unsigned int i = 0; i < m_Count && i < aElements; i++)
                {
                    newElements[i] = m_Elements[i];
                }
                //Release
                delete[] m_Elements;
            }
            m_Elements = newElements;
            m_Count = aElements;
        }

        void Release()
        {
            if (m_Elements != nullptr)
            {
                delete[] m_Elements;
                m_Elements = nullptr;
            }
            m_Count = 0;
        }

        TYPE & operator[](const UInt32 aIndex) const
        {
            return m_Elements[aIndex];
        }

		TYPE * GetElements() const
		{
			return m_Elements;
		}


        TYPE * GetElements()
        {
            return m_Elements;
        }
    private:
        TYPE * m_Elements;
        UInt32 m_Count;
    };

}

#endif