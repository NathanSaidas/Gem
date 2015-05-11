#ifndef G_TYPE_REGISTRY_H
#define G_TYPE_REGISTRY_H

#include "G_StringAttribute.h"
#include "G_IntegerAttribute.h"
#include "G_ObjectFuncAttribute.h"

#include <map>

namespace Gem
{
	namespace Reflection
	{
		typedef std::multimap<char *, StringAttribute> StringMMap;
		typedef std::multimap<char *, IntegerAttribute> IntegerMMap;
		typedef std::multimap<char *, ObjectFuncAttribute> ObjectFuncMMap;

		typedef std::pair<char *, StringAttribute> StringPair;
		typedef std::pair<char *, IntegerAttribute> IntegerPair;
		typedef std::pair<char *, ObjectFuncAttribute> ObjectFuncPair;

		typedef std::pair<StringMMap::iterator, StringMMap::iterator> StringMapKeyIterator;
		typedef std::pair<IntegerMMap::iterator, IntegerMMap::iterator> IntegerMapKeyIterator;
		typedef std::pair<ObjectFuncMMap::iterator, ObjectFuncMMap::iterator> ObjectFuncMapKeyIterator;


		namespace Hidden
		{
			///Holds all the meta data info at CRT init
			class TypeRegistry
			{
			public:
				inline static std::vector<char *> & GetTypes()
				{
					static std::vector<char *> types;
					return types;
				}

				inline static StringMMap & GetStringAttributes()
				{
					static StringMMap stringAttributes;
					return stringAttributes;
				}
				inline static IntegerMMap & GetIntegerAttributes()
				{
					static IntegerMMap integerAttributes;
					return integerAttributes;
				}

				inline static ObjectFuncMMap & GetObjectFuncAttributes()
				{
					static ObjectFuncMMap objectFuncAttributes;
					return objectFuncAttributes;
				}

				inline static int & NextID()
				{
					static int id = 1;
					id++;
					return id;
				}
			};
		}
	}
}

#endif