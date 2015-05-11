#ifndef GAME_ENGINE_REFLECTION_H
#define GAME_ENGINE_REFLECTION_H

#pragma region CHANGE LOG
/// -- January, 29, 2015 - Nathan Hanlan - Added file Reflection. Added TYPE_DEFINE, TYPE_DEFINE_PTR, TYPE_NAME macros
/// -- January, 30, 2015 - Nathan Hanlan - Added MetaObject / Runtime includes as well as macros for defining Types.
/// -- March,   15, 2015 - Nathan Hanlan - Added type traits for std::string
/// -- April,   1, 2015 - Nathan Hanlan - Added in MemberAttribute and MemberInfo to replace ClassMember class. 
/// -- April    3, 2015 - Nathan Hanlan - Cleaned up the comments for the available macros. 
/// -- -----    -, ---- - Nathan Hanlan - Deprecated some macros. Replaced with macros following a naming convention.
#pragma endregion

#include "Attribute.h"
#include "BoolAttribute.h"
#include "FloatAttribute.h"
#include "IntAttribute.h"
#include "StringAttribute.h"
#include "FunctionAttribute.h"
#include "TypeTrait.h"
#include "MetaObject.h"
#include "MetaObjectLinker.h"
#include "Runtime.h"
#include "AttributeBinder.h"
#include "MethodInfo.h"
#include "MemberAttribute.h"
#include "MemberInfo.h"
#include "../Type.h"

// -- Dependencies
// -- <cstring>
// -- <string>
// -- <limits>
// -- <map>
// -- <vector>
// --
// -- Type.h
// -- Func.h
// -- Method.h

//////////////////////
///AVAILABLE MACROS///
//////////////////////

// -- Naming
//	• R = Reflection
//	• DECLARE = Header (.h)
//	• DEFINE = Source (.cpp)

//	Note(Nathan): No macros require any type to have their data held within parentheses. Any string conversion is handled automatically.
//	Note(Nathan): All RDECLARE macros must have a RDEFINE accompanying them.

// -- INSIDE CLASS MACROS (Quick Reference)
//	These macros are intended to be used within the class definition. (Note(Nathan): This applies to headers.)
//	See the actual declaration for information

// -- Class Reflection
//	RDECLARE_CLASS(TYPE)
//	RDEFINE_CLASS(TYPE,BASECLASS)

// -- Abstract Class Reflection
//	RDECLARE_ABSTRACT_CLASS(TYPE)
//  RDEFINE_ABSTRACT_CLASS(TYPE,BASECLASS)

// -- Interface Reflection
//	RDECLARE_INTERFACE(TYPE)
//	RDEFINE_INTERFACE(TYPE)

// -- Class Attributes 
//	RDECLARE_ATTRIBUTE_INTERFACE(TYPE,INTERFACE)
//	RDEFINE_ATTRIBUTE_INTERFACE(TYPE,INTERFACE)

// -- Member Reflection.
//	RDECLARE_PUBLIC_MEMBER(TYPE,MEMBER)
//	RDEFINE_PUBLIC_MEMBER(TYPE,MEMBER,MEMBER_TYPE)
//	RDECLARE_PROTECTED_MEMBER(TYPE,MEMBER)
//	RDEFINE_PROTECTED_MEMBER(TYPE,MEMBER,MEMBER_TYPE)
//	RDECLARE_PRIVATE_MEMBER(TYPE,MEMBER)
//	RDEFINE_PRIVATE_MEMBER(TYPE,MEMBER,MEMBER_TYPE)



// -- Deprecated Macros.
//CLASS_ (HEADER/CPP) - Replaced with RDECLARE_CLASS / RDEFINE_CLASS rev 16
//ABSTRACT_CLASS_ (HEADER/CPP) - Replaced with RDECLARE_CLASS / RDEFINE_CLASS rev 16

//METHOD_INFO_HEADER(TYPE,METHOD_NAME) - Removed rev 16.
//METHOD_INFO_CPP(TYPE,METHOD_NAME, METHOD_INFO) - Removed rev 16.

//CLASS_ATTRIBUTE_INTERFACE_ (HEADER/CPP) - Replaced with RDECLARE_ATTRIBUTE_INTERFACE / RDEFINE_ATTRIBUTE_INTERFACE

namespace Engine
{
    namespace Reflection
    {

		/**
		* Declares the meta data object for class information. Uses the name HIDDEN_CLASS
		* @param TYPE The name of the class being reflected.
		*/
#define RDECLARE_CLASS(TYPE)														\
	private:																		\
		static const Engine::Reflection::MetaObject<TYPE> HIDDEN_CLASS;             \
	public:                                                                         \
		virtual Engine::Type GetType();                                             \
	private:                                                                        \

		/**
		* Defines the meta data object info for the class.
		* @param TYPE The name of the class being reflected.
		* @param BASECLASS The name of the baseclass. If there is no baseclass a empty string can be used in substitution.
		*/
#define RDEFINE_CLASS(TYPE,BASECLASS)																										\
	const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_CLASS = Engine::Reflection::MetaObject<TYPE>::DefineClass(#TYPE, #BASECLASS);   \
	Engine::Type TYPE::GetType() { static Engine::Type type = Engine::Reflection::Runtime::TypeOf(#TYPE); return type; }					\

		/**
		* Declares the meta data object for an abstract class. Uses the name HIDDEN_ABSTRACT_CLASS
		* @param TYPE The name of the class being reflected.
		*/
#define RDECLARE_ABSTRACT_CLASS(TYPE) private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_ABSTRACT_CLASS;
		
		/**
		* Defines the meta data object info for an abstract class.
		* @param TYPE The name of the class being reflected.
		* @param BASECLASS The name of the baseclass. If there is no baseclass a empty string can be used in substitution.
		*/
#define RDEFINE_ABSTRACT_CLASS(TYPE,BASECLASS) const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_ABSTRACT_CLASS = Engine::Reflection::MetaObject<TYPE>::DefineAbstractClass(#TYPE, #BASECLASS);

		/**
		* Declares the meta object for an interface.
		* @param TYPE The name of the interface being reflected.
		*/
#define RDECLARE_INTERFACE(TYPE) private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_INTERFACE; public: virtual Engine::Type GetType(); private:

		/**
		* Defines the meta data object info for an interface.
		* @param TYPE The name of the class being reflected.
		*/
#define RDEFINE_INTERFACE(TYPE)																												\
		const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_INTERFACE = Engine::Reflection::MetaObject<TYPE>::DefineInterface(#TYPE);	\
		Engine::Type TYPE::GetType() { static Engine::Type type = Engine::Reflection::Runtime::TypeOf(#TYPE); return type; }                \

		/**
		* Declares the meta data object info for an inherited interface.
		* @param TYPE The name of the class being reflected.
		* @param INTERFACE The an interface of the class.
		*/
#define RDECLARE_ATTRIBUTE_INTERFACE(TYPE,INTERFACE) private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_INTERFACE_ ## INTERFACE;

		/**
		* Define the meta data object info for an inherited interface.
		* @param TYPE The name of the class being reflected.
		* @param INTERFACE The an interface of the class.
		*/
#define RDEFINE_ATTRIBUTE_INTERFACE(TYPE,INTERFACE) const Engine::Reflection::MetaObject<TYPE> TYPE:: ## HIDDEN_INTERFACE_ ## INTERFACE = Engine::Reflection::MetaObject<TYPE>::DefineClassInterface(#TYPE,#INTERFACE);

		/**
		* Declare a meta object for a public reflected member.
		* @param TYPE The name of the class being reflected
		* @param MEMBER The name of the member being reflected.
		*/
#define DECLARE_PUBLIC_MEMBER(TYPE,MEMBER)														\
    private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_ ## MEMBER; public:       \

		/**
		* Define a meta object for a public reflected member.
		* @param TYPE The name of the class being reflected
		* @param MEMBER The name of the member being reflected.
		* @param MEMBER_TYPE The type name of the member being reflected.
		*/
#define RDEFINE_PUBLIC_MEMBER(TYPE,MEMBER,MEMBER_TYPE)																																				\
        const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_ ## MEMBER = Engine::Reflection::MetaObject<TYPE>::DeclareMemberType(#TYPE,#MEMBER, offsetof(TYPE,MEMBER), #MEMBER_TYPE, true);     \


		/**
		* Declare a meta object for a protected reflected member.
		* @param TYPE The name of the class being reflected
		* @param MEMBER The name of the member being reflected.
		*/
#define RDECLARE_PROTECTED_MEMBER(TYPE,MEMBER)													\
    private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_ ## MEMBER; protected:    \

		/**
		* Define a meta object for a protected reflected member.
		* @param TYPE The name of the class being reflected
		* @param MEMBER The name of the member being reflected.
		* @param MEMBER_TYPE The type name of the member being reflected.
		*/
#define RDEFINE_PROTECTED_MEMBER(TYPE,MEMBER,MEMBER_TYPE)																																			\
        const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_ ## MEMBER = Engine::Reflection::MetaObject<TYPE>::DeclareMemberType(#TYPE,#MEMBER, offsetof(TYPE,MEMBER), #MEMBER_TYPE, false);    \

		/**
		* Declare a meta object for a private reflected member.
		* @param TYPE The name of the class being reflected
		* @param MEMBER The name of the member being reflected.
		*/
#define RDECLARE_PRIVATE_MEMBER(TYPE,MEMBER)													\
    private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_ ## MEMBER; private:      \

		/**
		* Define a meta object for a private reflected member.
		* @param TYPE The name of the class being reflected
		* @param MEMBER The name of the member being reflected.
		* @param MEMBER_TYPE The type name of the member being reflected.
		*/
#define RDEFINE_PRIVATE_MEMBER(TYPE,MEMBER,MEMBER_TYPE)                                                                                                                                             \
        const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_ ## MEMBER = Engine::Reflection::MetaObject<TYPE>::DeclareMemberType(#TYPE,#MEMBER, offsetof(TYPE,MEMBER), #MEMBER_TYPE, false);    \

		/**
		* Declare a meta object for a reflected enum
		* @param TYPE The name of the enum being reflected.
		*/
#define RDECLARE_ENUM(TYPE) private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_ENUM;

		/**
		* Define the meta object into for the reflected enum.
		* @param TYPE The name of the enum being reflected.
		*/
#define RDEFINE_ENUM(TYPE) const Engine::Reflection::MetaObject<TYPE> TYPE::HIDDEN_ENUM = Engine::Reflection::MetaObject<TYPE>::DefineEnum(#TYPE);

        ///Define Primitive Types to allow for limited reflection info.

#define TYPE_NAME(TYPE) Engine::Reflection::TypeTrait<TYPE>::Name()

#define CLASS_HEADER(TYPE) RDECLARE_CLASS(TYPE) //private: static const Engine::Reflection::MetaObject<TYPE> HIDDEN_INTERFACE; public: virtual Engine::Type GetType(); private:
#define CLASS_CPP(TYPE,BASECLASS) RDEFINE_CLASS(TYPE,BASECLASS)

#define ABSTRACT_CLASS_HEADER(TYPE) RDECLARE_ABSTRACT_CLASS
#define ABSTRACT_CLASS_CPP(TYPE,BASECLASS) RDEFINE_ABSTRACT_CLASS(TYPE,BASECLASS)

#define INTERFACE_HEADER(TYPE) RDECLARE_INTERFACE(TYPE)
#define INTERFACE_CPP(TYPE) RDEFINE_INTERFACE(TYPE)

#define CLASS_ATTRIBUTE_INTERFACE_HEADER(TYPE,INTERFACE) RDECLARE_ATTRIBUTE_INTERFACE(TYPE,INTERFACE)
#define CLASS_ATTRIBUTE_INTERFACE_CPP(TYPE,INTERFACE) RDEFINE_ATTRIBUTE_INTERFACE(TYPE,INTERFACE)

		TYPE_DEFINE(char)
		TYPE_DEFINE(bool)
		TYPE_DEFINE(short)
		TYPE_DEFINE(int)
		TYPE_DEFINE(float)
		TYPE_DEFINE(double)
		TYPE_DEFINE(void)
		TYPE_DEFINE(long)
		TYPE_DEFINE(unsigned char)
		TYPE_DEFINE(unsigned short)
		TYPE_DEFINE(unsigned int)
		TYPE_DEFINE(unsigned long)
		TYPE_DEFINE(std::string)
		


		template<typename A,typename B>
		class TypeEqual
		{
		public:
			static bool IsEqual()
			{
				const char * a = TypeTrait<A>::Name();
				const char * b = TypeTrait<B>::Name();
				return strcmp(a, b) == 0;
			}
			
		};

		
    }

}

#endif