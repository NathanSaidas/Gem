#ifndef G_ICOLLECTION_H
#define G_ICOLLECTION_H

#include "../../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	namespace Collections
	{
		class ICollection : public Object
		{
			virtual Pointer<Reflection::Type> getType() override
			{
				return typeOf("ICollection");
			}
		};
	}
}
GEM_CLASS(Collections::ICollection,Object)

#endif