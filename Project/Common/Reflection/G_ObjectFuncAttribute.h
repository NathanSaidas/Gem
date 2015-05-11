#ifndef G_OBJECT_FUNC_ATTRIBUTE_H
#define G_OBJECT_FUNC_ATTRIBUTE_H

#include "G_Attribute.h"

namespace Gem
{
	namespace Reflection
	{
		///The function prototype of an ObjectFunc (Take a void*, returns a void*)
		typedef void * (*ObjectFunc)(void*);

		///Object Function Attribute
		class ObjectFuncAttribute : public Attribute
		{
		public:
			ObjectFuncAttribute(char * aClassName, const char * aAttributeName, ObjectFunc aValue) : Attribute()
			{
				m_ClassName = aClassName;
				m_AttributeName = const_cast<char*>(aAttributeName);
				m_Value = aValue;
			}
			ObjectFuncAttribute(char * aClassName, char * aAttributeName, ObjectFunc aValue) : Attribute()
			{
				m_ClassName = aClassName;
				m_AttributeName = aAttributeName;
				m_Value = aValue;
			}
			~ObjectFuncAttribute()
			{

			}
			inline ObjectFunc Value()
			{
				return m_Value;
			}
		private:
			ObjectFuncAttribute();
			ObjectFunc m_Value;
		};
	}
}

#endif