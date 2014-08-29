#include "G_string.h"
#include "G_integer.h"

namespace Gem
{
    const string string::Empty = string("");

    string::string(string & aValue)
    {
        m_Value = aValue;
    }
    string::string(std::string aValue)
    {
        m_Value = aValue;
    }
    string::string()
    {

    }
    string::~string()
    {

    }
    Reflection::Type * string::getType()
    {
        return Reflection::Type::create("string",Reflection::TypeID::STRING,sizeof(string),Object::getType());
    }

    //string g_strConCat(char * lhs,const char * rhs)
    //{
    //    
    //    integer lSize = strnlen_s(lhs,1000);
    //    integer rSize = strnlen_s(rhs,1000);
    //
    //    if(lSize == 0 && rSize == 0)
    //    {
    //        return "";
    //    }
    //
    //    char * buffer = new char[lSize + rSize + 1];
    //    //char * buffer = lhs;
    //    for(int i = 0; i < lSize; i++)
    //    {
    //        buffer[i] = lhs[i];
    //    }
    //    for(int i = 0; i < rSize; i++)
    //    {
    //        int index = lSize + i;
    //        buffer[index] = rhs[i];
    //    }
    //    buffer[lSize+rSize] = '\0';
    //
    //    string result = buffer;
    //
    //    delete[] buffer;
    //    //errno_t error = strcat_s(buffer,lSize + rSize + 1,lhs);
    //    //error = strcat_s(buffer,lSize + rSize + 1,rhs);
    //    return result;
    //}

}