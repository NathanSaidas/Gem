#ifndef G_ENUM_CONSTANT_H
#define G_ENUM_CONSTANT_H

#include "G_Primitive.h"
#include "G_Type.h"

#include "../Base Objects/G_Object.h"

namespace Gem
{
    namespace Reflection
    {
        //template<class TYPE,class DEREFERENCED_TYPE>
        //class Array;

        

        class Type;
        class Enum : public Primitive
        {
        public:
            Enum():Primitive(){}
            Enum(int aValue,char * aName):Primitive(){value = aValue; m_Name = aName;}
            ~Enum(){}
            int value;



            int & operator= (const int & aValue)
            {
                value = aValue;
                return value;
            }
            int operator+(const int & aValue)
            {
                return value + aValue;
            }
            int operator-(const int & aValue)
            {
                return value - aValue;
            }
            int operator+=(const int & aValue)
            {
                value += aValue;
                return value;
            }
            int operator-=(const int & aValue)
            {
                value -= aValue;
                return value;
            }
            bool operator==(int aValue)
            {
                return (aValue == value);
            }
            
            friend Type;
        };

        //class Enum : Type
        //{
        //public:
        //    Enum();
        //    ~Enum();
        //    
        //    //EnumConstant getEnum(char * aName);
        //    //EnumConstant getEnum(int aEnum);
        //    //void addEnum(char * aName);
        //    //int count();
        //private:
        //    //Array<EnumConstant,EnumConstant> m_Enums;
        //    int m_Count;
        //};
    }
}

#endif