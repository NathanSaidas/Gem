#ifndef G_STRING_H
#define G_STRING_H
#include <string>

#include "../Reflection/G_Primitive.h"

namespace Gem
{
    namespace Reflection
    {
        class Type;
    }
    
    class string sealed: public Reflection::Primitive
    {
    public:
    

        string(std::string & aString);
        string(string & aString);
        string(const string & aString);
        string(const char * aString);
        string(char aString[]);
        string();
        ~string();
        //Setters
        void set(const char * aString);
        void set(string & aString);
        void set(std::string & aString);
        void operator= (const char * rhs);
        void operator= (string & rhs);
        void operator= (std::string & rhs);
        

        //Length
        inline const int __stdcall length()
        {
            return m_Length;
        }
        inline const int __stdcall calcLength() // More accurate
        {
            return stringLength(m_Characters);
        }
        bool operator> (const char * rhs);
        bool operator> (string & rhs);
        bool operator> (int & rhs);
        bool operator< (const char * rhs);
        bool operator< (string & rhs);
        bool operator< (int & rhs);
        bool operator>= (const char * rhs);
        bool operator>= (string & rhs);
        bool operator>= (int & rhs);
        bool operator<= (const char * rhs);
        bool operator<= (string & rhs);
        bool operator<= (int & rhs);
        //Append
        
        void append(string & aString);
        void append(const char * aString);
        string operator+(string & rhs);
        string operator+ (const string & rhs);
        void operator+=(const string & rhs);
        void operator+= (string & rhs);
        string operator+ (const char * rhs);
        void operator+= (const char * rhs);
        //Compare (Equals / Difference) (Contains exact match)
        bool equals(const char * aString);
        bool equals(string & aString);
        bool operator== (const char * rhs);
        bool operator== (string & rhs);
        bool operator!= (const char * rhs);
        bool operator!= (string & rhs);
        //SubString
        void subString(string & aSubString,int aEnd); //assume start as 0
        void subString(string & aSubString,int aBegin, int aEnd);
        //Accessor
        inline char * __stdcall c_str() const
        {
            return m_Characters;
        }
        inline char * __stdcall str()
        {
            return m_Characters;
        }
        char at(const int aIndex);
        char operator[] (const int aIndex) const;

        static string empty();
        Pointer<Reflection::Type> getType() override;

    protected:
        void onInstantiate();
        void onDestroy();
        
    private:
        //Helpers
        int getLength() const;
        char * getString() const;
        char getCharacterAtIndex(int aIndex) const;

        void setString(const char * aString);
        void setString(string & aString);
        void setString(std::string & aString);

        void appendChar(char aChar);
        void appendString(const char * aString);
        void appendString(string & aString);
        void appendString(const string & aString);
        bool compareString(const char * aString);
        bool compareString(string & aString);

        int stringLength(const char * aString);
        int stringLength(string & aString);
        int stringLength(const string & aString);

        //Allocation
        void resize(int newSize); //bad dont use
        char * allocString(int aSize);
        void deallocString(char * aString);
        //For faster substring only
        void allocBuffer(int aSize);
        void unsafeInsert(int aLocation, char aCharacter);

        //Members
        int m_Length;
        char * m_Characters;


    };
}
GEM_CLASS(string, Primitive)
#endif