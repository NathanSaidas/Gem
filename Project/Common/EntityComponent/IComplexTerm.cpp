#include "IComplexTerm.h"

namespace Gem
{
	RDEFINE_INTERFACE(IComplexTerm)

		std::string IComplexTerm::OnSerialize() const
	{
		return "";
	}
	bool IComplexTerm::OnDeserialize(const std::string & aData) const
	{
		return true;
	}
}