#ifndef G_ATTRIBUTE_BINDERS_H
#define G_ATTRIBUTE_BINDERS_H



namespace Gem
{
	class Type;

	namespace Reflection
	{
		class StringAttribute;
		class IntegerAttribute;
		class ObjectFuncAttribute;

		class IStringAttributeBinder
		{
		public:
			virtual void BindAttribute(StringAttribute & aAttribute, Type & aType) = 0;
		};
		class IIntegerAttributeBinder
		{
		public:
			virtual void BindAttribute(IntegerAttribute & aAttribute, Type & aType) = 0;
		};
		class IObjectFuncAttributeBinder
		{
		public:
			virtual void BindAttribute(ObjectFuncAttribute & aAttribute, Type & aType) = 0;
		};

	}
}

#endif