#ifndef G_COLLECTION_H
#define G_COLLECTION_H

#include "../../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
    namespace Collections
    {
        class Collection : public Object
        {
        public:
            virtual Pointer<Reflection::Type> GetType() override;
        };
    }
}

GEM_CLASS(Collections::Collection, Object)
#endif