#ifndef G_ATTRIBUTE_DEFINITIONS_H
#define G_ATTRIBUTE_DEFINITIONS_H

namespace Gem
{
	namespace Reflection
	{
		///All of the names of attributes.
		namespace Attributes
		{
			static const char * CLASS_NAME = "Class Name";
			static const char * BASE_CLASS_NAME = "Base Class Name";
			static const char * CLASS_SIZE = "Class Size";
			static const char * CLASS_ALIGNMENT = "Class Alignment";
			static const char * CLASS_TYPE_ID = "Class Type ID";
			static const char * CREATE_FUNC = "Create Func";
			static const char * DESTROY_FUNC = "Destroy Func";
			static const char * INTERFACE_ATTRIBUTE = "Interface Attribute";
			static const char * INTERFACE_NAME = "Interface Name";
			static const char * IS_CLASS = "Is Class";
			static const char * IS_INTERFACE = "Is Interface";
		}
	}
}

#endif