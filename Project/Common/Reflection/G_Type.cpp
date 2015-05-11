#include "G_Type.h"

namespace Gem
{
	const char * Type::BAD_TYPE = "BadType";
	Type::Type()
	{
		m_Size = 0;
		m_Alignment = 0;
		m_BaseClass = 0;
		m_Name = const_cast<char*>(BAD_TYPE);
		m_TypeID = 0 - 1;

	}
	Type::~Type()
	{

	}
}