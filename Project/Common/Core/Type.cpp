#include "Type.h"
#include "..\Reflection\Reflection.h"

namespace Gem
{
    Array<Type> Type::GetInterfaces() const
    {
        Array<Type> types(m_Interfaces.size());
        
        int index = 0;
        for (std::vector<std::string>::const_iterator it = m_Interfaces.begin(); it != m_Interfaces.end(); it++)
        {
            Type type = Reflection::Runtime::TypeOf(*it);
            types[index] = type;
            index++;
        }
        return types;
    }

    Type Type::GetInterface(const std::string & aName) const
    {
        for (std::vector<std::string>::const_iterator it = m_Interfaces.begin(); it != m_Interfaces.end(); it++)
        {
            if ((*it) == aName)
            {
                return Reflection::Runtime::TypeOf(*it);
            }
        }
        return Type();
    }

    Array<Reflection::MemberInfo> Type::GetMembers() const
    {
        Array<Reflection::MemberInfo> members;
        Array<Reflection::MemberInfo>::Copy(m_Members, members);
        return members;
    }

    Reflection::MemberInfo Type::GetMember(const std::string & aName) const
    {
        for (std::vector<Reflection::MemberInfo>::const_iterator it = m_Members.begin(); it != m_Members.end(); it++)
        {
            if (aName == (*it).GetMemberName())
            {
                return (*it);
            }
        }
        return Reflection::MemberInfo();
    }
}