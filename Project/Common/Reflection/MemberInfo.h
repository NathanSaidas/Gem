#ifndef GAME_ENGINE_MEMBER_INFO_H
#define GAME_ENGINE_MEMBER_INFO_H


#include <string>

namespace Engine
{
    class object;

    namespace Reflection
    {
        class MemberInfo
        {
        public:
            MemberInfo(char * aClassTypeName, char * aMemberName, char * aMemberTypename, size_t aOffset, bool aIsPublic);
            MemberInfo();
            ~MemberInfo();
            char * GetClassTypeName() const;
            char * GetMemberName() const;
            char * GetMemberTypename() const;
            size_t GetOffset() const;
            bool IsPublic() const;

            object * GetOffsetPointer(object * aObject);

        private:
            char * m_ClassTypeName;
            char * m_MemberName;
            char * m_MemberTypename;
            size_t m_Offset;
            bool m_IsPublic;
        };
    }
}

#endif