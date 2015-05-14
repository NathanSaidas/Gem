#ifndef GEM_I_SERIALIZEABLE_H
#define GEM_I_SERIALIZEABLE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "Stream.h"
#include "IFormatter.h"

namespace Gem
{
	//class ISerializeable;
	//template class GEM_API Reflection::MetaObject<ISerializeable>;

    class ISerializeable
    {
        RDECLARE_INTERFACE(ISerializeable)
    public:
        virtual void OnSerialize(IFormatter * aFormatter, Stream & aStream) = 0;
        virtual void OnDeserialize(IFormatter * aFormatter, Stream & aStream) = 0;
    };

    TYPE_DEFINE(ISerializeable)
}

#endif //GEM_I_SERIALIZEABLE_H