#ifndef GAME_ENGINE_MEMBER_ATTRIBUTE_H
#define GAME_ENGINE_MEMBER_ATTRIBUTE_H

#pragma region CHANGE LOG
/// -- April, 1, 2015 - Nathan Hanlan - 
#pragma endregion

#include "Attribute.h"
#include "MemberInfo.h"

namespace Engine
{
    namespace Reflection
    {
        class MemberAttribute : public Attribute
        {
        public:
            /**
            * The constructor for MemberAttribute, Requires the class name and attribute name of attribute as well as the value.
            * @param aClassName The name of the class to bind the attribute to.
            * @param aAttributeName The name of the attribute type. See MetaObjectLinker.h for reserved attribute types.
            * @param aValue The value of the attribute
            */
            MemberAttribute(const char * aClassName, const char * aAttributeName, MemberInfo aValue)
                : Attribute(aClassName, aAttributeName), m_Value(aValue)
            {
               
            }

            inline MemberInfo GetValue()
            {
                return m_Value;
            }
        private:
            /**
            *  The value of the attribute
            */
            MemberInfo m_Value;
        };

        TYPE_DEFINE(MemberAttribute)
    }
}

#endif