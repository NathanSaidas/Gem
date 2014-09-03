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
        setString(aString);
    }
    string::string(string & aString)
    {
        m_Length = 0;
        m_Characters = 0;
        setString(aString);
    }
    string::string(const string & aString)
    {
        m_Length = 0;
        m_Characters = 0;
        setString(aString.c_str());
    }
    string::string(const char * aString)
    {
        m_Length = 0;
        m_Characters = 0;
        setString(aString);
    }
    string::string(char aCharacters[])
    {
        m_Length = 0;
        m_Characters = 0;
        setString(aCharacters);
    }
    string::string()
    {
        m_Length = 0;
        m_Characters = 0;
    }
    string::~string()
    {
        deallocString(m_Characters);
        m_Length = 0;
    }
    void string::set(const char * aString)
    {
        setString(aString);
    }
    void string::set(string & aString)
    {
        setString(aString);
    }
    void string::set(std::string & aString)
    {
        setString(aString);
    }
    void string::operator= (const char * rhs)
    {
        setString(rhs);
    }
    void string::operator= (string & rhs)
    {
        setString(rhs);
    }
    void string::operator= (std::string & rhs)
    {
        setString(rhs);
    }
    //int String::length()
    //{
    //    return getLength();
    //}
    //int String::calcLength()
    //{
    //    
    //}
    bool string::operator> (const char * rhs)
    {
        return m_Length > stringLength(rhs);
    }
    bool string::operator> (string & rhs)
    {
        return m_Length > stringLength(rhs);
    }
    bool string::operator> (int & rhs)
    {
        return m_Length > rhs;
    }
    bool string::operator< (const char * rhs)
    {
        return m_Length < stringLength(rhs);
    }
    bool string::operator< (string & rhs)
    {
        return m_Length < stringLength(rhs);
    }
    bool string::operator< (int & rhs)
    {
        return m_Length < rhs;
    }
    bool string::operator>= (const char * rhs)
    {
        return m_Length >= stringLength(rhs);
    }
    bool string::operator>= (string & rhs)
    {
        return m_Length >= stringLength(rhs);
    }
    bool string::operator>= (int & rhs)
    {
        return m_Length >= rhs;
    }
    bool string::operator<= (const char * rhs)
    {
        return m_Length <= stringLength(rhs);
    }
    bool string::operator<= (string & rhs)
    {
        return m_Length <= stringLength(rhs);
    }
    bool string::operator<= (int & rhs)
    {
        return m_Length <= rhs;
    }

    void string::append(string & aString)
    {
        appendString(aString.m_Characters);
    }
    void string::append(const char * aString)
    {
        appendString(aString);
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
        appendString(rhs);
    }
    void string::operator+= (string & rhs)
    {
        appendString(rhs);
    }
    string string::operator+ (const char * rhs)
    {
        string result = (*this);
        result +=rhs;
        return result;
    }
    void string::operator+= (const char * rhs)
    {
        appendString(rhs);
    }
    //Comparison
    bool string::equals(const char * aString)
    {
        return compareString(aString);
    }
    bool string::equals(string & aString)
    {
        return compareString(aString);
    }
    bool string::operator== (const char * rhs)
    {
        return compareString(rhs);
    }
    bool string::operator== (string & rhs)
    {
        return compareString(rhs);
    }
    bool string::operator!= (const char * rhs)
    {
        return !compareString(rhs);
    }
    bool string::operator!= (string & rhs)
    {
        return !compareString(rhs);
    }

    void string::subString(string & aSubString,int aEnd) //assume start as 0
    {
        return subString(aSubString,0,aEnd);
    }
    void string::subString(string & aSubString, int aBegin, int aEnd)
    {
        aSubString.allocBuffer(aEnd - aBegin + 1);
        int index = 0;
        for(int i = aBegin; i < aEnd; i++)
        {
            aSubString.unsafeInsert(index,(getCharacterAtIndex(i)));
            index ++;
        }
        aSubString.unsafeInsert(index,'\0');
        aSubString.m_Length = aEnd - aBegin;
    }

    //Getters
    //const char * String::c_str()
    //{
    //    
    //}
    char string::at(const int aIndex)
    {
        return getCharacterAtIndex(aIndex);
    }
    char string::operator[] (const int aIndex) const
    {
        return getCharacterAtIndex(aIndex);
    }

    string string::empty()
    {
        return string("");
    }
    void string::onInstantiate()
    {

    }
    void string::onDestroy()
    {

    }
    Pointer<Reflection::Type> string::getType()
    {
        return typeOf("string");
    }

    //NEW
    int string::getLength() const
    {
        return m_Length;
    }
    char * string::getString() const
    {
        return m_Characters;
    }
    char string::getCharacterAtIndex(int aIndex) const
    {
        if(aIndex >= 0 && aIndex <= m_Length && m_Characters != 0)
        {
            return m_Characters[aIndex];
        }
        return '\0';
    }

    void string::setString(const char * aString)
    {
        int newSize = stringLength(aString);
        deallocString(m_Characters);
        m_Characters = allocString(newSize + 1); //+ 1 for null terminator
        m_Length = newSize;
        for(int i = 0; i < m_Length; i++)
        {
            m_Characters[i] = aString[i];
        }
        m_Characters[m_Length] = '\0';
    }
    void string::setString(string & aString)
    {
        int newSize = stringLength(aString);
        deallocString(m_Characters);
        m_Characters = allocString(newSize + 1);
        m_Length = newSize;
        for(int i = 0; i < m_Length; i++)
        {
            m_Characters[i] = aString[i];
        }
        m_Characters[m_Length] = '\0';
    }
    void string::setString(std::string & aString)
    {
        int newSize = aString.size();
        deallocString(m_Characters);
        m_Characters = allocString(newSize + 1);
        m_Length = newSize;
        for(int i = 0; i < m_Length; i++)
        {
            m_Characters[i] = aString[i];
        }
        m_Characters[m_Length] = '\0';
    }
    void string::appendChar(char aChar)
    {
        int bufferSize = 1 + m_Length;
        char * newString = allocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        newString[m_Length] = aChar;
        deallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    void string::appendString(const char * aString)
    {
        int stringSize = stringLength(aString);
        int bufferSize = stringSize + m_Length;
        char * newString = allocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        for(int i = 0; i < stringSize; i++)
        {
            newString[i + m_Length] = aString[i];
        }
        deallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    void string::appendString(string & aString)
    {
        int stringSize = stringLength(aString);
        int bufferSize = stringSize + m_Length;
        char * newString = allocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        for(int i = 0; i < stringSize; i++)
        {
            newString[i + m_Length] = aString[i];
        }
        deallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    void string::appendString(const string & aString)
    {
        int stringSize = stringLength(aString);
        int bufferSize = stringSize + m_Length;
        char * newString = allocString(bufferSize + 1);
        for(int i = 0; i < m_Length; i++)
        {
            newString[i] = m_Characters[i];
        }
        for(int i = 0; i < stringSize; i++)
        {
            newString[i + m_Length] = aString[i];
        }
        deallocString(m_Characters);
        m_Characters = newString;
        m_Length = bufferSize;
        m_Characters[m_Length] = '\0';
    }
    bool string::compareString(const char * aString)
    {
        int length = stringLength(aString);
        for(int i = 0; i < m_Length && i < length; i++)
        {
            if(m_Characters[i] != aString[i])
            {
                return false;
            }
        }
        return true;
    }
    bool string::compareString(string & aString)
    {
        return compareString(aString.getString());
    }

    int string::stringLength(const char * aString)
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
    int string::stringLength(string & aString)
    {
        char * characters = aString.getString();

        int index = 0;
        //While there hasnt been a null terminator or reach end of string length
        while(index < aString.getLength())
        {
            if(characters[index] == '\0')
            {
                break;
            }
            index ++;
        }
        return index;
    }
    int string::stringLength(const string & aString)
    {
        char * characters = aString.getString();

        int index = 0 ;
        while(index < aString.getLength())
        {
            if(characters[index] == '\0')
            {
                break;
            }
            index ++;
        }
        return index;
    }
    void string::resize(int newSize)
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
    char * string::allocString(int aSize)
    {
        if(aSize == 0)
        {
            return 0;
        }
        return new char[aSize];
    }
    void string::deallocString(char * aString)
    {
        if(aString != 0)
        {
            delete[] aString;
            aString = 0;
        }
    }
    void string::allocBuffer(int aSize)
    {
        deallocString(m_Characters);
        m_Characters = allocString(aSize);
    }
    void string::unsafeInsert(int aLocation, char aCharacter)
    {
        if(m_Characters != 0)
        {
            m_Characters[aLocation] = aCharacter;
        }
    }

}