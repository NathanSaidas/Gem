#ifndef G_RUNTIME_H
#define G_RUNTIME_H


#include "G_IntegerAttribute.h"
#include "G_StringAttribute.h"
#include "G_ObjectFuncAttribute.h"
#include "G_Type.h"
#include <map>

namespace Gem
{
	class Allocator;
	class Type;

	namespace Reflection
	{
		class IStringAttributeBinder;
		class IIntegerAttributeBinder;
		class IObjectFuncAttributeBinder;

		///This is the class that handles all the types and their attributes.
		class Runtime
		{
		public:
			static Runtime * Instance(IStringAttributeBinder * aStringAttributeBinder = nullptr, IIntegerAttributeBinder * aIntegerAttributeBinder = nullptr, IObjectFuncAttributeBinder * aObjectFuncAttributeBinder = nullptr);
			static void Destroy();


			Type GetType(char * aName);
			template<class ARG1 = void, class ARG2 = void, class ARG3 = void, class ARG4 = void, class ARG5>
			Type GetTemplateType(char * aName)
			{
				return Type();
			}
			bool IsBadType(Type & aType);
			bool IsBaseOf(Type & aBase, Type & aDerived);
			

		private:
			static IStringAttributeBinder * s_StringAttributeBinder;
			static IIntegerAttributeBinder * s_IntegerAttributeBinder;
			static IObjectFuncAttributeBinder * s_ObjectFuncAttributeBinder;
			static Runtime * s_Instance;
			Runtime();
			~Runtime();

			///Main binding function that binds the attributes to their types.
			void BindAttributes();
			///Binding Helper Functions
			void BindStringAttribute(StringAttribute & aAttribute, Type & aType);
			void BindIntegerAttribute(IntegerAttribute & aAttribute, Type & aType);
			void BindObjectFuncAttribute(ObjectFuncAttribute & aAttribute, Type & aType);


			
			///A map of all types and their attributes
			std::map<char*, Type> m_Types;

			friend Allocator;
		};
	}
}

#endif