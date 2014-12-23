#ifndef G_ATTRIBUTE_H
#define G_ATTRIBUTE_H
#include <cstring> //strcmp

namespace Gem
{
	namespace Reflection
	{
		///The base class of attributes.
		///A minimal attributes require a name for the class they belong to and a attribute name to identify the name of attribute.
		///Attribute names should be unique by type.
		class Attribute
		{
		public:
			///The name of the class  this attribute is binded to
			inline char * ClassName()
			{
				return m_ClassName;
			}
			///A unique name of the attribute.
			inline char * AttributeName()
			{
				return m_AttributeName;
			}
			///Returns true if the given attribute name is equal
			inline bool Is(const char * aAttributeName)
			{
				return strcmp(m_AttributeName, aAttributeName) == 0;
			}
		protected:
			char * m_ClassName;
			char * m_AttributeName;
		};

		
	}
}

#endif