#include "ComponentZero.h"
#include "../EntityComponent/EntityComponentSerialization.h"

namespace Gem
{
	RDEFINE_CLASS(ComponentZero,Component)
	
		RDEFINE_PROTECTED_FUNCTION(ComponentZero, OnSerializeData, void, std::vector<InstructionTerm*>&)
	void ComponentZero::OnSerializeData(std::vector<InstructionTerm*>& aTerms)
	{
		Component::OnSerializeData(aTerms);
		aTerms.push_back(SERIALIZE_TERM(m_Health));
		aTerms.push_back(SERIALIZE_TERM(m_Damage));
		aTerms.push_back(SERIALIZE_TERM(m_Level));
		aTerms.push_back(SERIALIZE_TERM(m_SpawnPoint));
	}

	RDEFINE_PROTECTED_FUNCTION(ComponentZero, OnDeserializeData, void, std::vector<InstructionTerm*>&)
	void ComponentZero::OnDeserializeData(std::vector<InstructionTerm*>& aTerms)
	{
		Component::OnDeserializeData(aTerms);
		InstructionTerm* term = InstructionTerm::GetTerm(aTerms, "m_Health");
		if (term != nullptr)
		{
			m_Health = term->AsFloat();
		}
		term = InstructionTerm::GetTerm(aTerms, "m_Damage");
		if (term != nullptr)
		{
			m_Damage = term->AsFloat();
		}
		term = InstructionTerm::GetTerm(aTerms, "m_Level");
		if (term != nullptr)
		{
			m_Level = term->AsInt();
		}
		term = InstructionTerm::GetTerm(aTerms, "m_SpawnPoint");
		if (term != nullptr)
		{
			m_SpawnPoint = term->AsVector3();
		}
	}

}