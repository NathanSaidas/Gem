#ifndef G_REFLECTION_H
#define G_REFLECTION_H

#include <string>

#include "G_AttributeDefinitions.h"
#include "G_Attribute.h"
#include "G_IntegerAttribute.h"
#include "G_StringAttribute.h"
#include "G_ObjectFuncAttribute.h"
#include "G_Function.h"
#include "G_Method.h"
#include "G_ReflectionDefinitions.h"
#include "G_Runtime.h"
#include "G_TypeRegistry.h"
#include "G_Type.h"





namespace Gem
{
	namespace Reflection
	{
		inline void Initialize()
		{
			Runtime::Instance();
		}
		inline void CleanUp()
		{
			Runtime::Destroy();
		}
		inline Type GetType(char * aType)
		{
			return Runtime::Instance()->GetType(aType);
		}
		inline Type GetType(const char * aType)
		{
			return Runtime::Instance()->GetType(const_cast<char*>(aType));
		}
		inline Type GetType(std::string & aType)
		{
			return Runtime::Instance()->GetType(const_cast<char*>(aType.c_str()));
		}

		inline bool IsBadType(Type & aType)
		{
			return Runtime::Instance()->IsBadType(aType);
		}

		inline bool IsBaseOf(Type & aBase, Type & aDerived)
		{
			return Runtime::Instance()->IsBaseOf(aBase, aDerived);
		}
	}
}

#endif