#include "G_TypeList.h"
#include <stdlib.h>


namespace Gem
{
    namespace Reflection
    {
        TypeInfo::TypeInfo()
        {
            m_ClassName = "";
            m_BaseClassName = "";
            m_TypeID = 0;
            m_Size = 0;
            m_Alignment = 0;
            m_CreateFunc = nullptr;
            m_DestroyFunc = nullptr;
        }
        TypeInfo::~TypeInfo()
        {

        }

        std::string TypeInfo::className() const
        {
            return m_ClassName;
        }
        unsigned int TypeInfo::typeID()
        {
            return m_TypeID;
        }
        unsigned int TypeInfo::size()
        {
            return m_Size;
        }
        unsigned int TypeInfo::alignment()
        {
            return m_Alignment;
        }
        CreatePrimitiveFunc TypeInfo::createFunc()
        {
            return m_CreateFunc;
        }
        DestroyPrimitiveFunc TypeInfo::DestroyFunc()
        {
            return m_DestroyFunc;
        }

        std::string TypeInfo::baseClassName() const
        {
            return m_BaseClassName;
        }
        TypeInfo * TypeInfo::create(std::string aName, unsigned int aID, unsigned int aSize, unsigned int aAlignment, CreatePrimitiveFunc aCreateFunc, DestroyPrimitiveFunc aDestroyFunc, std::string aBaseClassName)
        {
            TypeInfo * typeInfo = new TypeInfo();
            typeInfo->m_ClassName = aName;
            typeInfo->m_TypeID = aID;
            typeInfo->m_Size = aSize;
            typeInfo->m_Alignment = aAlignment;
            typeInfo->m_CreateFunc = aCreateFunc;
            typeInfo->m_DestroyFunc = aDestroyFunc;
            typeInfo->m_BaseClassName = aBaseClassName;
            return typeInfo;
        }

        bool TypeKey::compareName(const char * aName)
        {
            return strcmp(m_ClassName.c_str(), aName) == 0;
        }
        bool TypeKey::compareID(int aID)
        {
            return m_TypeID == aID;
        }
        TypeInfo * TypeList::GetType(std::string aName)
        {
            for (int i = 0; i < m_Keys.size(); i++)
            {
                if (m_Keys[i].compareName(aName.c_str()) == true)
                {
                    return m_Keys[i].m_Info;
                }
            }
            return nullptr;
        }
        TypeInfo * TypeList::GetType(const char * aName)
        {
            for (int i = 0; i < m_Keys.size(); i++)
            {
                if (m_Keys[i].compareName(aName) == true)
                {
                    return m_Keys[i].m_Info;
                }
            }
            return nullptr;
        }
        TypeInfo * TypeList::GetType(int aId)
        {
            for (int i = 0; i < m_Keys.size(); i++)
            {
                if (m_Keys[i].compareID(aId) == true)
                {
                    return m_Keys[i].m_Info;
                }
            }
            return nullptr;
        }
        int TypeList::GetTypeCount()
        {
            return m_Keys.size();
        }
        void TypeList::addKey(TypeInfo * aType)
        {
            if (aType == nullptr)
            {
                return;
            }

            if (GetType(aType->m_ClassName) == nullptr && GetType(aType->m_TypeID) == nullptr)
            {
                TypeKey key;
                key.m_ClassName = aType->m_ClassName;
                key.m_TypeID = aType->m_TypeID;
                key.m_Info = aType;
                m_Keys.push_back(key);
            }
        }
    }
}