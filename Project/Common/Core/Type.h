#ifndef GAME_ENGINE_TYPE_H
#define GAME_ENGINE_TYPE_H


//============================================================
// Date:			May,	11,		2015
// Description:		Contains the class Type. Provides runtime information about objects.
//============================================================

#pragma region CHANGE LOG
/// Nathan Hanlan - Added class/file Type.
/// Nathan Hanlan - Changed Collection return types from vector to Array
/// Nathan Hanlan - Added in class member inspection.
/// Nathan Hanlan - Runtime now adds all inherited methods regardless if the names existed before.
/// Nathan Hanlan - Add GetMethodInfo with aBaseClass argument
#pragma endregion

#include <string>
#include <vector>
#include "Func.h"
#include "../Reflection/Member.h"
#include "../Reflection/MemberInfo.h"
#include "../Utilities/Array.h"
#include "GemAPI.h"



template class GEM_API std::vector<Gem::Reflection::MemberInfo>;
template class GEM_API std::vector<Gem::Reflection::Member*>;
template class GEM_API Gem::Func<void*, void*>;

namespace Gem
{

    namespace Reflection
    {
        class Runtime;
    }


    class GEM_API Type
    {
    public:
        Type()
        {
            m_Name = "";
            m_BaseClass = "";
            m_Size = 0;
            m_Alignment = 0;
            m_TypeID = -1;
            m_IsClass = false;
            m_IsAbstract = false;
            m_IsInterface = false;
			m_IsEnum = false;
            m_Constructor = nullptr;
            m_Destructor = nullptr;
        }

        inline std::string GetName() const
        {
            return m_Name;
        }
        inline std::string GetBaseClass() const
        {
            return m_BaseClass;
        }
        inline int GetSize() const
        {
            return m_Size;
        }
        inline int GetAlignment() const
        {
            return m_Alignment;
        }
        inline int GetTypeID() const
        {
            return m_TypeID;
        }
        inline bool IsClass() const
        {
            return m_IsClass;
        }
        inline bool IsInterface() const
        {
            return m_IsInterface;
        }
        inline bool IsAbstract() const
        {
            return m_IsAbstract;
        }
		inline bool IsEnum() const
		{
			return m_IsEnum;
		}

        Array<Type> GetInterfaces() const;
        
        Type GetInterface(const std::string & aName) const;

        Array<Reflection::MemberInfo> GetMembers() const;
		/**
		* Searches for a member by name.
		* @ param aName The name of the member to search for.
		*/
        Reflection::MemberInfo GetMember(const std::string & aName) const;
        
		/**
		* Gets all of the methods.
		* @return Returns an array of all methods this type has.
		*/
		Array<Reflection::Member*> GetMethods() const;

		/**
		* Searches for a method with the specified name. Searches in root class before base classes.
		* @param aName The name of method to search for.
		* @return Returns a pointer to method info should it exist. Return nullptr otherwise.
		*/
		Reflection::Member * GetMethodInfo(const std::string & aName) const;

		/**
		* Searches for a method with the specified name and of the baseclass type.
		* @param aBaseClass The name of the baseclass the method needs to be of.
		* @param aName The name of method to search for.
		* @return Returns a pointer to the method should it exist. Return nullptr otherwise.
		*/
		Reflection::Member * GetMethodInfo(const std::string & aBaseClass, const std::string & aName) const;

        Func<void*, void*> GetConstructor() const
        {
            return m_Constructor;
        }
        Func<void*, void*> GetDestructor() const
        {
            return m_Destructor;
        }

        bool operator== (Type & aType) const
        {
            return m_TypeID == aType.m_TypeID;
        }
        
        //Property GetProperty(const std::string & aPropertyName) const
        //{
        //    for (std::vector<Property>::const_iterator it = m_Properties.begin(); it != m_Properties.end(); it++)
        //    {
        //        if ((*it).GetName() == aPropertyName)
        //        {
        //            return *it;
        //        }
        //    }
        //    return Property("", "");
        //}
        //
        //std::vector<Property> GetProperties() const
        //{
        //    return m_Properties;
        //}
        //
        //void InsertProperty(Property aProperty)
        //{
        //    if (s_IsCompiling)
        //    {
        //        m_Properties.push_back(aProperty);
        //    }
        //}


    private:
        std::string m_Name;
        std::string m_BaseClass;
        int m_Size;
        int m_Alignment;
        int m_TypeID;
        bool m_IsClass;
        bool m_IsInterface;
        bool m_IsAbstract;
		bool m_IsEnum;
        std::vector<std::string> m_Interfaces;
        std::vector<Reflection::MemberInfo> m_Members;
		std::vector<Reflection::Member*> m_Methods;
		std::vector<Reflection::MemberInfo> m_InheritedMembers;
		std::vector<Reflection::Member*> m_InheritedMethods;
        //std::vector<Property> m_Properties;
        Func<void*, void*> m_Constructor;
        Func<void*, void*> m_Destructor;

        static bool s_IsCompiling;
		friend Reflection::Runtime;
        //friend class PropertyBinder;
    };

    
}

#endif