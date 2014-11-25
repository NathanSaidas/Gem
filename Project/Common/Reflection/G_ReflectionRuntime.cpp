#include "G_ReflectionRuntime.h"
#include <vector>
#include "G_Type.h"
#include "../Primitives/G_integer.h"
#include "../Primitives/G_string.h"
#include "G_ReflectionRegristration.h"
namespace Gem
{
    namespace Reflection
    {
        Runtime * Runtime::s_Instance = nullptr;
        Runtime * Runtime::Instance()
        {
            if (s_Instance == nullptr)
            {
                s_Instance = new Runtime();
            }
            return s_Instance;
        }
        void Runtime::Destroy()
        {
            if (s_Instance != nullptr)
            {
                delete s_Instance;
                s_Instance = nullptr;
            }
        }


        Type * Runtime::GetType(string & aTypename)
        {
            for (int i = 0; i < m_TypeCount; i++)
            {
                if (m_Types[i] != nullptr && aTypename == m_Types[i]->name())
                {
                    return Type::copy(m_Types[i]);
                }
            }
            return nullptr;
        }
        Type * Runtime::GetType(integer & aTypeID)
        {
            for (int i = 0; i < m_TypeCount; i++)
            {
                if (m_Types[i] != nullptr && aTypeID == m_Types[i]->typeID())
                {
                    return Type::copy(m_Types[i]);
                }
            }
            return nullptr;
        }

        
        Runtime::Runtime()
        {
            //Create types
            
            TypeList & list = GetTypeRegistry();
            
            //Create list of type infos to iterate through for type creation
            std::vector<TypeInfo*> typeInfoList;
            {
                TypeInfo * info = list.GetType(0);
                for (int i = 1; info != nullptr; i++)
                {
                    typeInfoList.push_back(info);
                    info = list.GetType(i);
                }
            }

            std::vector<Type*> typeList;

            //Create a type object for each typeinfo in the typeinfo list
            while (typeInfoList.size() >= 1)
            {
                TypeInfo * info = typeInfoList[0];
                if (info != nullptr)
                {
                    //Create a type
                    Type * type = Memory::Instantiate<Type>();
                    typeList.push_back(type);
                    //Create temp ptrs
                    TypeInfo * tempInfo = info;
                    Type * tempType = type;
                   
                    //Assign data for type and base types
                    while (tempType != nullptr)
                    {
                        //Set type from info
                        tempType->m_Alignment = info->alignment();
                        tempType->m_Size = info->size();
                        tempType->m_TypeID = info->typeID();
                        tempType->m_Name = info->className();
                        tempType->m_CreateFunc = info->createFunc();
                        tempType->m_DestroyFunc = info->DestroyFunc();

                        
                        tempInfo = list.GetType(info->baseClassName());
                        if (tempInfo != nullptr)
                        {
                            Type * base = Memory::Instantiate<Type>();
                            tempType->m_BaseType = base;
                            info = tempInfo;
                        }
                        tempType = tempType->m_BaseType;    
                    }
                }
                //erase front
                typeInfoList.erase(typeInfoList.begin() + 0);
            }

            //Create the array of types dynamically
            m_TypeCount = typeList.size();
            m_Types = Memory::InstantiateArray<Type*>(m_TypeCount);

            int currentIndex = 0;
            while (typeList.size() > 0)
            {
                for (int i = 0; i < typeList.size(); i++)
                {
                    //Get ID matching current index
                    //Assign that to type to the final list
                    //erase that type off the temp list
                    //increment current index
                    //break out of for loop
                    if (typeList[i] != nullptr && typeList[i]->typeID() == currentIndex)
                    {
                        m_Types[currentIndex] = typeList[i];
                        currentIndex++;
                        typeList.erase(typeList.begin() + i);
                        break;
                    }
                }
            }


        }

        Runtime::~Runtime()
        {
            for (int i = 0; i < m_TypeCount; i++)
            {
                m_Types[i] = Memory::Destroy<Type>(m_Types[i]);
            }
            m_Types = Memory::DestroyArray<Type*>(m_Types, m_TypeCount);
        }
    }
}