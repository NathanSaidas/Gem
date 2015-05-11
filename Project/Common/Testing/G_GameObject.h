#ifndef G_GAME_OBJECT_H
#define G_GAME_OBJECT_H

#define TRICK(X) (X)

#include "../G_Object.h"

namespace Gem
{
	class GameObject : public Object
	{
		G_CLASS_DEF(GameObject)
	public:
		GameObject();
		~GameObject();
		virtual void Update();
	
	};

	

	G_REFLECT(int)
	G_REFLECT(float)

	template<class T>
	class TestTemplate : public object
	{
		static const Gem::Reflection::TemplateTypeEntry<TestTemplate<T>,T> GEM_CLASS_DEFINITION;
	public:
		virtual Gem::Type GetType()
		{
			static Gem::Type classType = Gem::Reflection::Runtime::Instance()->GetType("TestTemplate");
			return classType;
		}
	private:
		//G_TEMPLATE_CLASS(TestTemplate<T>)
	public:
		TestTemplate() 
		{
			TRICK(GEM_CLASS_DEFINITION);
		}
		virtual ~TestTemplate()
		{

		}
		T data;
	};

	//G_TEMPLATE_CLASS_IMPLEMENTATION(TestTemplate<T>,object)
	template<class T>
	const Gem::Reflection::TemplateTypeEntry<TestTemplate<T>,T> TestTemplate<T>::GEM_CLASS_DEFINITION = Gem::Reflection::TemplateTypeEntry<TestTemplate<T>,T>::TemplateDefinition("TestTemplate", "object");

	
}

#endif