#ifndef GEM_IEC_TYPE_H
#define GEM_IEC_TYPE_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"

namespace Gem
{
	class IComplexTerm
	{
		RDECLARE_INTERFACE(IComplexTerm)
	public:
		virtual std::string OnSerialize() const;
		virtual bool OnDeserialize(const std::string & aData) const;
	};
	TYPE_DEFINE(IComplexTerm)
}

#endif