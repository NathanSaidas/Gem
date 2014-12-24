#ifndef G_TYPE_H
#define G_TYPE_H

//#include "G_ReflectionDefinitions.h"
#include "G_Function.h"
#include <vector>

namespace Gem
{
	namespace Reflection
	{
		class Runtime;
	}

	class Type
	{

	public:
		Type();
		~Type();

		inline char * Name()
		{
			return m_Name;
		}
		inline char * BaseClass()
		{
			return m_BaseClass;
		}
		inline int Size()
		{
			return m_Size;
		}
		inline int Alignment()
		{
			return m_Alignment;
		}
		inline int TypeID()
		{
			return m_TypeID;
		}
		inline bool IsClass()
		{
			return m_IsClass;
		}
		inline bool IsInterface()
		{
			return m_IsInterface;
		}
		inline std::vector<char *> Interfaces()
		{
			return m_Interfaces;
		}

		inline void * InvokeConstructor(void * aMemory)
		{
			return m_Create.Invoke(aMemory);
		}
		inline void * InvokeDestructor(void * aMemory)
		{
			return m_Destroy.Invoke(aMemory);
		}
	private:
		char * m_Name;
		char * m_BaseClass;
		
		unsigned int m_Size;
		unsigned int m_Alignment;

		unsigned int m_TypeID;

		bool m_IsClass;
		bool m_IsInterface;

		Reflection::Func<void*, void*> m_Create;
		Reflection::Func<void*, void*> m_Destroy;
		std::vector<char *> m_Interfaces;


		static const char * BAD_TYPE;
		friend Reflection::Runtime;
	};

	
}

#endif
