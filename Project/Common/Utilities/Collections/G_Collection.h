#ifndef G_COLLECTION_H
#define G_COLLECTION_H

#include "../../G_Object.h"

namespace Gem
{
    namespace Collections
    {
        class Collection : public object
        {
			G_CLASS_DEF(Collection)
        public:
			Collection();
			virtual ~Collection();
        };
    }
}
#endif