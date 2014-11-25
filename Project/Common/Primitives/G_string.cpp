#include "G_string.h"
#include "G_integer.h"
#include "../Reflection/G_Type.h"
#include "../Reflection/G_ReflectionOperators.h"


namespace Gem
{
    using namespace Reflection;

    string::string(std::string & aString)
    {
        m_Length = 0;
        m_Characters = 0;
        SetString(aString);
    }
    string::string(string & aString)
    {
        m_Length = 0;
        m_Characters = 0;
        SetString(aString);
    }
    string::string(const string & aString)
    {
        m_Length = 0;
        m_Characters = 0;
        SetString(aString.C_Str());
    }
    string::string(const char * aString)
    {
        m_Length = 0;
        m_Characters = 0;
        SetString(aString);
    }
    string::string()
    {
        m_Length = 0;
        m_Characters = 0;
    }
    string::~string()
    {
        DeallocString(m_Characters);
        m_Length = 0;
    }
    void string::Set(const char * aString)
    {
        SetString(aString);
    }
    void string::Set(string & aString)
    {
        SetString(aString);
    }
    void string::Set(std::string & aString)
    {
        SetString(aString);
    }
	string string::operator= (const char * rhs)
    {
        SetString(rhs);
		return *this;
    }
	string string::operator= (string & rhs)
    {
        SetString(rhs);
		return *this;
    }
	string string::operator= (std::string & rhs)
    {
        SetString(rhs);
		return *this;
    }
    //int String::length()
    //{
    //    return GetLength();
    //}
    //int String::calcLength()
    //{
    //    
    //}
    bool string::operator> (const char * rhs)
    {
        return m_Length > StringLength(rhs);
    }
    bool string::operator> (string & rhs)
    {
        return m_Length > StringLength(rhs);
    }
    bool string::operator> (int & rhs)
    {
        return m_Length > rhs;
    }
    bool string::operator< (const char * rhs)
    {
        return m_Length < StringLength(rhs);
    }
    bool string::operator< (string & rhs)
    {
        return m_Length < StringLength(rhs);
    }
    bool string::operator< (int & rhs)
    {
        return m_Length < rhs;
    }
    bool string::operator>= (const char * rhs)
    {
        return m_Length >= StringLength(rhs);
    }
    bool string::operator>= (string & rhs)
    {
        return m_Length >= StringLength(rhs);
    }
    bool string::operator>= (int & rhs)
    {
        return m_Length >= rhs;
    }
    bool string::operator<= (const char * rhs)
    {
        return m_Length <= StringLength(rhs);
    }
    bool string::operator<= (string & rhs)
    {
        return m_Length <= StringLength(rhs);
    }
    bool string::operator<= (int & rhs)
    {
        return m_Length <= rhs;
    }

    void string::Append(string & aString)
    {
        AppendString(aString.m_Characters);
    }
    void string::Append(const char * aString)
    {
        AppendString(aString);
    }
    string string::operator+(string & rhs)
    {
        string result = (*this);
        result += rhs;
        return result;
    }
    string string::operator+ (const string & rhs)
    {
        string result = (*this);
        result += rhs;
        return result;
    }
    void string::operator+=(const string & rhs)
    {
        AppendString(rhs);
    }
    void string::operator+= (string & rhs)
    {
        AppendString(rhs);
    }
    string string::operator+ (const char * rhs)
    {
        string result = (*this);
        result +=rhs;
        return result;
    }
    void string::operator+= (const char * rhs)
    {
        AppendString(rhs);
    }
    //Comparison
    bool string::Equals(const char * aString)
    {
        return CompareString(aString);
    }
    bool string::Equals(string & aString)
    {
        return CompareString(aString);
    }
    bool string::operator== (const char * rhs)
    {
        return CompareString(rhs);
    }
    bool string::operator== (string & rhs)
    {
        return CompareString(rhs);
    }
    bool string::operator!= (const char * rhs)
    {
        return !CompareString(rhs);
    }
    bool string::operator!= (string & rhs)
    {
        return !CompareString(rhs);
    }
	bool string::operator==(const string & rhs)
	{
		return CompareString(rhs.C_Str());
	}
    string string::SubString(string & aSubString,int aStart) 
    {
        return SubString(aSubString,aStart,aSubString.Length()-1);
    }
    string string::SubString(string & aSubString, int aStart, int aEnd)
    {
		aEnd += 1;
		integer stringLength = aSubString.Length();
		if (stringLength <= 1)
		{
			return aSubString;
		}
		string newString("");
		integer index = 0;
		integer newStringLength = aEnd - aStart + 1;
		newString.AllocBuffer(newStringLength);
		for (int i = aStart; i < aEnd; i++)
		{
			newString.UnsafeInsert(index, aSubString[i]);
			index++;
		}
		newString.UnsafeInsert(index, '\0');
		newString.m_Length = newStringLength - 1;
		return newString;
    }

	string string::Replace(string & aString, char * aOld, char * aNew)
	{
		return Replace(aString, string(aOld), string(aNew));
	}
	string string::Replace(string & aString, string & aOld, string & aNew)
	{
		integer stringLength = aString.Length();
		integer oldStringLength = aOld.Length();
		//find a index with a matching first character

		//iterate the next characters to verify the string
		integer index = 0;
		bool matchFound = false;
		for (int i = 0; i < stringLength; i++)
		{
			if (aString[i] == aOld[0])
			{
				index = i + 1;
				for (int j = 1; j < oldStringLength; j++)
				{
					if (index >= stringLength)
					{
						return aString;
					}
					//mismatch break out and start over
					if (aString[index] != aOld[j])
					{
						i = index - 1;
						break;
					}
					//Match Found
					else if (j == oldStringLength - 1)
					{
						index = index - j;
						matchFound = true;
						break;
					}
					index++;
				}
			}
			if (matchFound)
			{
				break;
			}
		}

		if (matchFound)
		{
			string newString;
			newString.Append(SubString(aString, 0, index));
			newString.Append(aNew);
			newString.Append(SubString(aString, index + oldStringLength, stringLength));
			return newString;
		}

		return aString;
	}
    //Getters
    //const char * String::c_str()
    //{
    //    
    //}
    char string::At(const int aIndex)
    {
        return GetCharacterAtIndex(aIndex);
    }
    char string::operator[] (const int aIndex) const
    {
        return GetCharacterAtIndex(aIndex);
    }

    string string::Empty()
    {
        return string("");
    }
    void string::onInstantiate()
    {

    }
    void string::onDestroy()
    {

    }
    Pointer<Reflection::Type> string::GetType()
    {
        return typeOf("string");
    }

    //NEW
    int string::GetLength() const
    {
        return m_Length;
    }
    char * string::GetString() const
    {
        return m_Characters;
    }
    char string::GetCharacterAtIndex(int aIndex) const
    {
        if(aIndex >= 0 && aIndex <= m_Length && m_Characters != 0)
        {
            return m_Characters[aIndex];
        }
        return '\0';
    }

    void string::SetString(const char * aString)
    {
        int newSize = StringLength(aString);
        DeallocString(m_Characters);
        m_Characters = AllocString(newSize + 1); //+ 1 for null terminator
        m_Length = newSize;
        for(int i = 0; i < m_Length; i++)
        {
            m_Characters[i] = aString[i];
        }
        m_Characters[m_Length] = '\0';
    }
    void string::SetString(string & aString)
    {
        int newSize = StringLength(aString);
        DeallocString(m_Characters);
        m_Characters = AllocString(newSize + 1);
        m_Length = newSize;
        for(int i = 0; i < m_Length; i++)
        {
            m_Characters[i] = aString[i];
        }
        m_Characters[m_Length] = '\0';
    }
    void string::SetString(std::string & aString)
    {
        int newSize = aString.size();
        DeallocString(m_Characters);
        m_Characters = AllocString(newSize + 1);
        m_Length = newSize;
        for(int i = 0; i < m_Length; i++)
        {
            m_Characters[i] = aString[i];
        }
        m_Characters[m_Length] = '\0';
    }
    void string::AppendChar(char aChar)
    {
        int bufferSize = 1 + m_Length;
        char * newString = AllocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        newString[m_Length] = aChar;
        DeallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    void string::AppendString(const char * aString)
    {
        int stringSize = StringLength(aString);
        int bufferSize = stringSize + m_Length;
        char * newString = AllocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        for(int i = 0; i < stringSize; i++)
        {
            newString[i + m_Length] = aString[i];
        }
        DeallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    void string::AppendString(string & aString)
    {
        int stringSize = StringLength(aString);
        int bufferSize = stringSize + m_Length;
        char * newString = AllocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        for(int i = 0; i < stringSize; i++)
        {
            newString[i + m_Length] = aString[i];
        }
        DeallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    void string::AppendString(const string & aString)
    {
        int stringSize = StringLength(aString);
        int bufferSize = stringSize + m_Length;
        char * newString = AllocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        for(int i = 0; i < stringSize; i++)
        {
            newString[i + m_Length] = aString[i];
        }
        DeallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    bool string::CompareString(const char * aString)
    {
        int length = StringLength(aString);
        for(int i = 0; i < m_Length && i < length; i++)
        {
            if(m_Characters[i] != aString[i])
            {
                return false;
            }
        }
        return true;
    }
    bool string::CompareString(string & aString)
    {
        return CompareString(aString.GetString());
    }

    int string::StringLength(const char * aString)
    {
        bool foundNull = false;
        int index = 0;
        //While there hasnt been a null terminator
        while(!foundNull)
        {
            if(aString[index] == '\0')
            {
                break;
            }
            index ++;
        }
        return index;
    }
    int string::StringLength(string & aString)
    {
        char * characters = aString.GetString();

        int index = 0;
        //While there hasnt been a null terminator or reach end of string length
        while(index < aString.GetLength())
        {
            if(characters[index] == '\0')
            {
                break;
            }
            index ++;
        }
        return index;
    }
    int string::StringLength(const string & aString)
    {
        char * characters = aString.GetString();

        int index = 0 ;
        while(index < aString.GetLength())
        {
            if(characters[index] == '\0')
            {
                break;
            }
            index ++;
        }
        return index;
    }
    void string::Resize(int newSize)
    {
        if(m_Characters != 0)
        {
            delete[] m_Characters;
            m_Characters = 0;
        }
            
        if(m_Characters == 0)
        {
            m_Characters = new char [newSize];
            m_Length = newSize;
        }
    }
    char * string::AllocString(int aSize)
    {
        if(aSize == 0)
        {
            return 0;
        }
        return new char[aSize];
    }
    void string::DeallocString(char * aString)
    {
        if(aString != 0)
        {
            delete[] aString;
            aString = 0;
        }
    }
    void string::AllocBuffer(int aSize)
    {
        DeallocString(m_Characters);
        m_Characters = AllocString(aSize);
    }
    void string::UnsafeInsert(int aLocation, char aCharacter)
    {
        if(m_Characters != 0)
        {
            m_Characters[aLocation] = aCharacter;
        }
    }

}