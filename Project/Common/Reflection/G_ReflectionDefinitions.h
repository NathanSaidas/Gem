#ifndef G_REFLECTION_DEFINITIONS_H
#define G_REFLECTION_DEFINITIONS_H

#include <map>
#include <vector>
#include <array>
#include "G_IntegerAttribute.h"
#include "G_StringAttribute.h"
#include "G_ObjectFuncAttribute.h"
#include "G_AttributeDefinitions.h"
#include "G_Type.h"
#include "G_TypeRegistry.h"
#include "G_Runtime.h"


///To get an understanding of how im manipulating the CRT to add meta data to classes take a read at this. ~Nathan
///http://msdn.microsoft.com/en-us/library/bb918180.aspx

namespace Gem
{
	

	namespace Reflection
	{
		namespace Hidden
		{
			template<typename T>
			class TypeName { public: static const char* name; };

			template<typename T>
			const char * TypeName<T>::name = "unknown";
			///Static methods for different types.
			template<class T>
			class ObjectFactory
			{
			public:
				///Invokes constructor
				static void * CreateObject(void * aAddress)
				{
					return new(aAddress)T();
				}
				///Invokes destructor
				static void * DestroyObject(void * aAddress)
				{
					T * obj = static_cast<T*>(aAddress);
					if (obj == nullptr)
					{
						return aAddress;
					}
					obj->T::~T();
					return nullptr;
				}
			};


		}

		///Template TypeEntry Serves as a hack at constexpr because of msvc lack of constexpr features.
		///Each class can define a static const variable and invoke the methods below to do some operations.
		template<class T>
		class TypeEntry
		{
		public:
			///Defines a class in the reflection system
			static TypeEntry & ClassDefinition(char * aName, char * aBaseClass)
			{
				///Retrieve the string and integer maps from the Type Registry
				std::vector<char *> & types = Hidden::TypeRegistry::GetTypes();
				StringMMap & stringAttributes = Hidden::TypeRegistry::GetStringAttributes();
				IntegerMMap & integerAttributes = Hidden::TypeRegistry::GetIntegerAttributes();
				ObjectFuncMMap & objectFuncAttributes = Hidden::TypeRegistry::GetObjectFuncAttributes();

				///Insert class name and base class attributes into the registry
				types.push_back(aName);
				stringAttributes.insert(StringPair(aName, StringAttribute(aName, Attributes::CLASS_NAME, aName)));
				stringAttributes.insert(StringPair(aName, StringAttribute(aName, Attributes::BASE_CLASS_NAME, aBaseClass)));
				
				///Insert compile time class information such as size and alignment into the registry
				integerAttributes.insert(IntegerPair(aName, IntegerAttribute(aName, Attributes::CLASS_SIZE, sizeof(T))));
				integerAttributes.insert(IntegerPair(aName, IntegerAttribute(aName, Attributes::CLASS_ALIGNMENT, __alignof(T))));
				integerAttributes.insert(IntegerPair(aName, IntegerAttribute(aName, Attributes::IS_CLASS, 1)));
				integerAttributes.insert(IntegerPair(aName, IntegerAttribute(aName, Attributes::IS_INTERFACE, 0)));

				///Create an ID and assign it into the registry as well.
				integerAttributes.insert(IntegerPair(aName, IntegerAttribute(aName, Attributes::CLASS_TYPE_ID, Hidden::TypeRegistry::NextID())));

				objectFuncAttributes.insert(ObjectFuncPair(aName, ObjectFuncAttribute(aName, Attributes::CREATE_FUNC, Hidden::ObjectFactory<T>::CreateObject)));
				objectFuncAttributes.insert(ObjectFuncPair(aName, ObjectFuncAttribute(aName, Attributes::DESTROY_FUNC, Hidden::ObjectFactory<T>::DestroyObject)));
				return GetInstance();
			}

			

			///Defines a classes interface in the reflection system.
			static TypeEntry & InterfaceAttribute(char * aClassName, char * aInterfaceName)
			{
				StringMMap & stringAttributes = Hidden::TypeRegistry::GetStringAttributes();
				stringAttributes.insert(StringPair(aClassName, StringAttribute(aClassName, Attributes::INTERFACE_ATTRIBUTE, aInterfaceName)));
				return GetInstance();
			}

			static TypeEntry & InterfaceDefinition(char * aInterfaceName)
			{
				std::vector<char*> & types = Hidden::TypeRegistry::GetTypes();
				StringMMap & stringAttributes = Hidden::TypeRegistry::GetStringAttributes();
				IntegerMMap & integerAttributes = Hidden::TypeRegistry::GetIntegerAttributes();
				types.push_back(aInterfaceName);

				stringAttributes.insert(StringPair(aInterfaceName, StringAttribute(aInterfaceName, Attributes::INTERFACE_NAME, aInterfaceName)));

				integerAttributes.insert(IntegerPair(aInterfaceName, IntegerAttribute(aInterfaceName, Attributes::IS_CLASS, 0)));
				integerAttributes.insert(IntegerPair(aInterfaceName, IntegerAttribute(aInterfaceName, Attributes::IS_INTERFACE, 1)));

				return GetInstance();
			}

		private:
			///This creates a single instance of the TypeEntry.
			static TypeEntry & GetInstance()
			{
				static TypeEntry<T> classDefinition;
				return classDefinition;
			}
			///Default constructor of the type entry
			TypeEntry()
			{
				
			}
		};

		template<class T, class ARG1 = void, class ARG2 = void, class ARG3 = void, class ARG4 = void, class ARG5 = void>
		class TemplateTypeEntry
		{
		public:
			static TemplateTypeEntry & TemplateDefinition(char * aName, char * aBaseClass)
			{
				std::string name = aName;
				name.append("<");
				char * arg1 = const_cast<char*>(Hidden::TypeName<ARG1>::name);
				char * arg2 = const_cast<char*>(Hidden::TypeName<ARG2>::name);
				char * arg3 = const_cast<char*>(Hidden::TypeName<ARG3>::name);
				char * arg4 = const_cast<char*>(Hidden::TypeName<ARG1>::name);
				char * arg5 = const_cast<char*>(Hidden::TypeName<ARG1>::name);
				if (!(strcmp(arg1, "void") == 0 || strcmp(arg1, "unknown") == 0))
				{
					name.append(arg1);
				}
				name.append(">");

				std::vector<char *> & types = Hidden::TypeRegistry::GetTypes();
				StringMMap & stringAttributes = Hidden::TypeRegistry::GetStringAttributes();
				IntegerMMap & integerAttributes = Hidden::TypeRegistry::GetIntegerAttributes();
				ObjectFuncMMap & objectFuncAttributes = Hidden::TypeRegistry::GetObjectFuncAttributes();

				return GetInstance();
			}
		private:
			///This creates a single instance of the TypeEntry.
			static TemplateTypeEntry & GetInstance()
			{
				static TemplateTypeEntry classDefinition;
				return classDefinition;
			}
			///Default constructor of the type entry
			TemplateTypeEntry()
			{

			}
		};

		

	}
}

#define G_REFLECT(TYPE) template<> const char * Gem::Reflection::Hidden::TypeName<TYPE>::name = #TYPE;

///Defines the class attribute
#define G_CLASS_ATTRIBUTE_DEF(CLASS_TYPE);										\
	static const Gem::Reflection::TypeEntry<CLASS_TYPE> GEM_CLASS_DEFINITION;	\
public:																			\
	virtual Gem::Type GetType();												\
private:																		\

///Implements the class attribute
#define G_CLASS_ATTRIBUTE_IMPLEMENTATION(CLASS_TYPE,BASE_CLASS)																											\
	const Gem::Reflection::TypeEntry<CLASS_TYPE> CLASS_TYPE::GEM_CLASS_DEFINITION = Gem::Reflection::TypeEntry<CLASS_TYPE>::ClassDefinition(#CLASS_TYPE, #BASE_CLASS);	\
	Gem::Type CLASS_TYPE::GetType()																																		\
	{																																									\
	static Gem::Type classType = Gem::Reflection::Runtime::Instance()->GetType(#CLASS_TYPE);																		\
	return classType;																																				\
}																																									\


#define G_TEMPLATE_CLASS(CLASS_TYPE)																\
	static const Gem::Reflection::TypeEntry<CLASS_TYPE> GEM_CLASS_DEFINITION;						\
public:																								\
	virtual Gem::Type GetType()																		\
	{																								\
		static Gem::Type classType = Gem::Reflection::Runtime::Instance()->GetType(#CLASS_TYPE);	\
		return classType;																			\
	}																								\
private:																							\


#define G_TEMPLATE_CLASS_IMPLEMENTATION(CLASS_TYPE,BASE_CLASS)	const Gem::Reflection::TypeEntry<CLASS_TYPE> CLASS_TYPE::GEM_CLASS_DEFINITION = Gem::Reflection::TypeEntry<CLASS_TYPE>::ClassDefinition(#CLASS_TYPE, #BASE_CLASS);																																																							  \
	


#define GT_SRING(CLASS_TYPE) #CLASS_TYPE
///Short Hand Methods
#define G_CLASS_DEF(CLASS_TYPE) G_CLASS_ATTRIBUTE_DEF(CLASS_TYPE)
#define G_CLASS_IMPLEMENTATION(CLASS_TYPE,BASE_CLASS) G_CLASS_ATTRIBUTE_IMPLEMENTATION(CLASS_TYPE,BASE_CLASS)

///Defines an interface attribute
#define G_INTERFACE_ATTRIBUTE_DEF(CLASS_TYPE,INTERFACE)										\
static const Gem::Reflection::TypeEntry<CLASS_TYPE> INTERFACE ## _INTERFACE_DEFINITION;		\

///Implements an interface attribute
#define G_INTERFACE_ATTRIBUTE_IMPLEMENTATION(CLASS_TYPE,INTERFACE)																													\
const Gem::Reflection::TypeEntry<CLASS_TYPE> CLASS_TYPE::INTERFACE ## _INTERFACE_DEFINITION = Gem::Reflection::TypeEntry<CLASS_TYPE>::InterfaceAttribute(#CLASS_TYPE, #INTERFACE);	\

#define G_INTERFACE_DEF(INTERFACE_TYPE) 											\
static const Gem::Reflection::TypeEntry<INTERFACE_TYPE> GEM_INTERFACE_DEFINITION;	\

#define G_INTERFACE_IMPLEMENTATION(INTERFACE_TYPE)																																\
const Gem::Reflection::TypeEntry<INTERFACE_TYPE> INTERFACE_TYPE::GEM_INTERFACE_DEFINITION = Gem::Reflection::TypeEntry<INTERFACE_TYPE>::InterfaceDefinition(#INTERFACE_TYPE);	\



#endif