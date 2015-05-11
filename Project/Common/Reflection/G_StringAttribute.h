#ifndef G_STRING_ATTRIBUTE_H
#define G_STRING_ATTRIBUTE_H

#include "G_Attribute.h"

namespace Gem
{
	namespace Reflection
	{
		class StringAttribute : public Attribute
		{
		public:
			StringAttribute(char * aClassName, const char * aAttributeName, char * aValue) : Attribute()
			{
				m_ClassName = aClassName;
				m_AttributeName = const_cast<char*>(aAttributeName);
				m_Value = aValue;
			}
			StringAttribute(char * aClassName, char * aAttributeName, char * aValue) : Attribute()
			{
				m_ClassName = aClassName;
				m_AttributeName = aAttributeName;
				m_Value = aValue;
			}
			~StringAttribute()
			{

			}
			inline char * Value()
			{
				return m_Value;
			}
		private:
			StringAttribute();
			char * m_Value;
		};

		
	}
}

#endif