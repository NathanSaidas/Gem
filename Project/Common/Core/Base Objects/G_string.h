#ifndef G_STRING_H
#define G_STRING_H

#include "G_Object.h"
#include <string>

namespace Gem
{
    class string sealed: public Object
    {
    public:
        string(const string & aValue):m_Value(aValue.m_Value){}
        string(string & aValue);
        string(std::string aValue);
        string();
        ~string();


        inline std::string get()
        {
            return m_Value;
        }
        inline void set(std::string aValue)
        {
            m_Value = aValue;
        }

        inline operator std::string()
        {
            return m_Value;
        }

        inline string append(const char * aValue)
        {
            m_Value.append(aValue);
            return m_Value;
        }

        inline int count()
        {
            return m_Value.size();
        }

        inline const char * c_str()
        {
            return m_Value.c_str();
        }

        const static string Empty;

        inline string operator+(const char * rhs)
        {
            return string(m_Value.append(rhs));
        }
        inline char operator[](int aIndex)
        {
            if(aIndex >= 0 && aIndex < m_Value.size())
            {
                return m_Value[aIndex];
            }
            return '\0';
        }
        inline bool operator==(const char * aString)
        {
            return m_Value == aString;
        }
        inline bool operator==(string aString)
        {
            return m_Value == aString.m_Value;
        }
        inline bool operator!=(string aString)
        {
            return m_Value != aString.m_Value;
        }
        

        virtual Reflection::Type * getType() override;
    private:
        std::string m_Value;
    };

    inline string operator+=(string lhs, const char * rhs)
    {
        return lhs.append(rhs);
    }
    /*inline int operator-=(real lhs, const int & rhs)
    {
        return int(lhs - rhs);
    }
    inline int operator*=(real lhs, const int & rhs)
    {
        return int(lhs * rhs);
    }
    inline int operator/=(real lhs, const int & rhs)
    {
        return int(lhs / rhs);
    }*/
    


}

#endif