#ifndef G_INTEGER_ATTRIBUTE_H
#define G_INTEGER_ATTRIBUTE_H

#include "G_Attribute.h"

namespace Gem
{
	namespace Reflection
	{
		class IntegerAttribute : public Attribute
		{
		public:
			IntegerAttribute(char * aClassName, const char * aAttributeName, int aValue) : Attribute()
			{
				m_ClassName = aClassName;
				m_AttributeName = const_cast<char*>(aAttributeName);
				m_Value = aValue;
			}


			IntegerAttribute(char * aClassName, char * aAttributeName, int aValue) : Attribute()
			{
				m_ClassName = aClassName;
				m_AttributeName = aAttributeName;
				m_Value = aValue;
			}
			~IntegerAttribute()
			{

			}
			inline int Value()
			{
				return m_Value;
			}
		private:
			IntegerAttribute();
			int m_Value;
		};
	}
}

#endif