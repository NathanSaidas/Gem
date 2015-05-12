#ifndef GAME_ENGINE_MEMBER_INFO_H
#define GAME_ENGINE_MEMBER_INFO_H

//============================================================
// Date:			May,	11,		2015
// Description:		Contains the MemberInfo class.
//============================================================

#include <string>
#include "../Core/GemAPI.h"

namespace Gem
{
    class object;

    namespace Reflection
    {
		/**
		* This class provides information on a member of a class.
		*/
        class GEM_API MemberInfo
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