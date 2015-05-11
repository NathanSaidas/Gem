#include "G_Runtime.h"
#include "G_ReflectionDefinitions.h"
#include "G_Type.h"
#include "G_AttributeBinders.h"
#include "../G_Config.h"


#ifdef GEM_MEMORY
#include "../Memory/G_Memory.h"
#endif

namespace Gem
{
	namespace Reflection
	{
		IStringAttributeBinder * Runtime::s_StringAttributeBinder = nullptr;
		IIntegerAttributeBinder * Runtime::s_IntegerAttributeBinder = nullptr;
		IObjectFuncAttributeBinder * Runtime::s_ObjectFuncAttributeBinder = nullptr;
		Runtime * Runtime::s_Instance = nullptr;
		Runtime * Runtime::Instance(IStringAttributeBinder * aStringAttributeBinder, IIntegerAttributeBinder * aIntegerAttributeBinder, IObjectFuncAttributeBinder * aObjectFuncAttributeBinder)
		{
			if (s_Instance == nullptr)
			{
#ifdef GEM_MEMORY
				s_Instance = Memory::Instantiate<Runtime>();
#else
				s_Instance = new Runtime();
#endif
				s_StringAttributeBinder = aStringAttributeBinder;
				s_IntegerAttributeBinder = aIntegerAttributeBinder;
				s_ObjectFuncAttributeBinder = aObjectFuncAttributeBinder;
			}
			return s_Instance;
		}
		void Runtime::Destroy()
		{
			if (s_Instance != nullptr)
			{
#ifdef GEM_MEMORY
				s_Instance = Memory::Destroy<Runtime>(s_Instance);
#else
				delete s_Instance;
				s_Instance = nullptr;
#endif
			}
		}
		Runtime::Runtime()
		{
			BindAttributes();
		}
		Runtime::~Runtime()
		{
			m_Types.clear();
		}
		void Runtime::BindAttributes()
		{
			///Get all the registered types
			std::vector<char *> & types = Hidden::TypeRegistry::GetTypes();
			///Retrieve the attribute maps from the Type Registry
			StringMMap & stringAttributes = Hidden::TypeRegistry::GetStringAttributes();
			IntegerMMap & integerAttributes = Hidden::TypeRegistry::GetIntegerAttributes();
			ObjectFuncMMap & objectFuncAttributes = Hidden::TypeRegistry::GetObjectFuncAttributes();

			///For each type, Grab the attributes from the registry and bind them to a type and then store the type
			for (std::vector<char*>::iterator typeIter = types.begin(); typeIter != types.end(); ++typeIter)
			//for (int i = types.size(); i >= 0; i--)
			{
				///Retrieve Attributes for Type
				StringMapKeyIterator sKeyIter = stringAttributes.equal_range((*typeIter));
				IntegerMapKeyIterator iKeyIter = integerAttributes.equal_range((*typeIter));
				ObjectFuncMapKeyIterator ofKeyIter = objectFuncAttributes.equal_range((*typeIter));

				///Create Type
				Type type;
				///Bind String Attributes
				for (StringMMap::iterator it = sKeyIter.first; it != sKeyIter.second; ++it)
				{
					BindStringAttribute((*it).second, type);
				}
				///Bind Integer Attributes
				for (IntegerMMap::iterator it = iKeyIter.first; it != iKeyIter.second; ++it)
				{
					BindIntegerAttribute((*it).second, type);
				}
				///Bind Object Function Attributes
				for (ObjectFuncMMap::iterator it = ofKeyIter.first; it != ofKeyIter.second; ++it)
				{
					BindObjectFuncAttribute((*it).second, type);
				}

				///Add Type to Runtime.
				m_Types.insert(std::pair<char*, Type>(type.m_Name, type));
			}

			///Clear out all of the attributes.
			types.clear();
			stringAttributes.clear();
			integerAttributes.clear();
			objectFuncAttributes.clear();
		}

		void Runtime::BindStringAttribute(StringAttribute & aAttribute, Type & aType)
		{
			if (aAttribute.Is(Attributes::CLASS_NAME))
			{
				aType.m_Name = aAttribute.Value();
			}
			else if (aAttribute.Is(Attributes::BASE_CLASS_NAME))
			{
				aType.m_BaseClass = aAttribute.Value();
			}
			else if (aAttribute.Is(Attributes::INTERFACE_ATTRIBUTE))
			{
				aType.m_Interfaces.push_back(aAttribute.Value());
			}
			else if (aAttribute.Is(Attributes::INTERFACE_NAME))
			{
				aType.m_Name = aAttribute.Value();
			}
			if (s_StringAttributeBinder != nullptr)
			{
				s_StringAttributeBinder->BindAttribute(aAttribute, aType);
			}
		}
		void Runtime::BindIntegerAttribute(IntegerAttribute & aAttribute, Type & aType)
		{
			if (aAttribute.Is(Attributes::CLASS_SIZE))
			{
				aType.m_Size = (unsigned int)aAttribute.Value();
			}
			else if (aAttribute.Is(Attributes::CLASS_ALIGNMENT))
			{
				aType.m_Alignment = (unsigned int)aAttribute.Value();
			}
			else if (aAttribute.Is(Attributes::CLASS_TYPE_ID))
			{
				aType.m_TypeID = (unsigned int)aAttribute.Value();
			}
			else if (aAttribute.Is(Attributes::IS_CLASS))
			{
				aType.m_IsClass = aAttribute.Value() == 1;
			}
			else if (aAttribute.Is(Attributes::IS_INTERFACE))
			{
				aType.m_IsInterface = aAttribute.Value() == 1;
			}
			if (s_IntegerAttributeBinder != nullptr)
			{
				s_IntegerAttributeBinder->BindAttribute(aAttribute, aType);
			}
		}
		void Runtime::BindObjectFuncAttribute(ObjectFuncAttribute & aAttribute, Type & aType)
		{
			if (aAttribute.Is(Attributes::CREATE_FUNC))
			{
				aType.m_Create = aAttribute.Value();
			}
			else if (aAttribute.Is(Attributes::DESTROY_FUNC))
			{
				aType.m_Destroy = aAttribute.Value();
			}
			if (s_ObjectFuncAttributeBinder != nullptr)
			{
				s_ObjectFuncAttributeBinder->BindAttribute(aAttribute, aType);
			}
		}

		Type Runtime::GetType(char * aName)
		{
			std::map<char*, Type>::iterator iterType = m_Types.find(aName);
			if (iterType == m_Types.end())
			{
				return Type();
			}
			return (*iterType).second;
		}
		bool Runtime::IsBadType(Type & aType)
		{
			return strcmp(aType.Name(), Type::BAD_TYPE) == 0;
		}
		bool Runtime::IsBaseOf(Type & aBase, Type & aDerived)
		{
			if (strcmp(aBase.Name(), Type::BAD_TYPE) == 0 || aDerived.BaseClass() == nullptr)
			{
				return false;
			}
			if (strcmp(aBase.Name(), aDerived.BaseClass()) == 0)
			{
				return true;
			}
			else
			{
				std::map<char*, Type>::iterator next = m_Types.find(aDerived.BaseClass());

				while (next != m_Types.end())
				{
					if (strcmp((*next).second.BaseClass(), aBase.Name()) == 0)
					{
						return true;
					}
					next = m_Types.find((*next).second.BaseClass());
				}

			}
			return false;
		}
	}
	
}