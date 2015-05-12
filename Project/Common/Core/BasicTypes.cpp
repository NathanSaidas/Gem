#include "BasicTypes.h"

namespace Gem
{
	CLASS_CPP(object, "");
	CLASS_CPP(Object, object);

	object::object()
	{

	}
	object::~object()
	{

	}
	string object::ToString()
	{
        return GetType().GetName();
	}

	Object::Object() : object()
	{

	}
	Object::~Object()
	{

	}

	string Object::ToString()
	{
        return GetType().GetName();
	}

	string Object::GetName()
	{
		return m_Name;
	}
	void Object::SetName(const string & aName)
	{
		m_Name = aName;
	}

	void Object::SetName(const char * aName)
	{
		m_Name = aName;
	}


}