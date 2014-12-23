#ifndef G_OBJECT_H
#define G_OBJECT_H

#include "Reflection\G_Reflection.h"
#include <string>

namespace Gem
{
	class object
	{
		G_CLASS_DEF(object)
	public:
		object();
		virtual ~object();
	};

	class Object : object
	{
		G_CLASS_DEF(Object)
	public:
		Object();
		virtual ~Object();
	protected:
		std::string m_Name;
	};
}

#endif