#ifndef G_STRING_H
#define G_STRING_H
///Include string for the use of compatibility with std::string
#include <string>
///Include primitive to allow string to be a primitive.
#include "../Reflection/G_Primitive.h"

namespace Gem
{
	///Declare type within the reflection namespace.
    namespace Reflection
    {
        class Type;
    }
    ///This is the basic string classes used without the Gem Framework. 
    class string sealed: public Reflection::Primitive
    {
    public:
		///Constructors
        string(std::string & aString);
        string(string & aString);
        string(const string & aString);
        string(const char * aString);
        string(char aString[]);
        string();
        ~string();
        //Setters
        void Set(const char * aString);
        void Set(string & aString);
        void Set(std::string & aString);
        string operator= (const char * rhs);
        string operator= (string & rhs);
		string operator= (std::string & rhs);
        //simply returns the length of the string
        inline const int __stdcall Length()
        {
            return m_Length;
        }
		//A more accurate version of finding the length of the string.
        inline const int __stdcall CalcLength() // More accurate
        {
            return StringLength(m_Characters);
        }
		///Compare Length of string to determine greater or lesser value
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

        //Append / Concatnate methods
        void Append(string & aString);
        void Append(const char * aString);
        string operator+(string & rhs);
        string operator+ (const string & rhs);
        void operator+=(const string & rhs);
        void operator+= (string & rhs);
        string operator+ (const char * rhs);
        void operator+= (const char * rhs);
        //Compare (Equals / Difference) (Contains exact match)
        bool Equals(const char * aString);
        bool Equals(string & aString);
        bool operator== (const char * rhs);
        bool operator== (string & rhs);
		bool operator==(const string & rhs);
        bool operator!= (const char * rhs);
        bool operator!= (string & rhs);
		///Creates a string out of the given string from the start index specified to the end of the string.
        static string SubString(string & aSubString,int aStart);
		///Creates a string out of the given string from the start index specified to the end index specified.
        static string SubString(string & aSubString,int aStart, int aEnd);
		///Replaces the old characters contained within the string given with the new characters.
		///Returns the given string if the old string could not be found or if the length of the string is invalid. (0-1)
		static string Replace(string & aString, char * aOld, char * aNew);
		static string Replace(string & aString, string & aOld, string & aNew);
        //Accessor
        inline char * __stdcall C_Str() const
        {
            return m_Characters;
        }
        inline char * __stdcall Str()
        {
            return m_Characters;
        }
        char At(const int aIndex);
        char operator[] (const int aIndex) const;

        static string Empty();
        Pointer<Reflection::Type> GetType() override;

    protected:
        void onInstantiate();
        void onDestroy();
        
    private:
        //Helpers
        int GetLength() const;
        char * GetString() const;
        char GetCharacterAtIndex(int aIndex) const;

        void SetString(const char * aString);
        void SetString(string & aString);
        void SetString(std::string & aString);

        void AppendChar(char aChar);
        void AppendString(const char * aString);
        void AppendString(string & aString);
        void AppendString(const string & aString);
        bool CompareString(const char * aString);
        bool CompareString(string & aString);

        int StringLength(const char * aString);
        int StringLength(string & aString);
        int StringLength(const string & aString);

        //Allocation
        void Resize(int newSize); //bad dont use
        char * AllocString(int aSize);
        void DeallocString(char * aString);
        //For faster substring only
        void AllocBuffer(int aSize);
        void UnsafeInsert(int aLocation, char aCharacter);

        //Members
        int m_Length;
        char * m_Characters;
    };
}
GEM_CLASS(string, Primitive)
#endif