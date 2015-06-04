#ifndef GEM_COMPONENT_ZERO_H
#define GEM_COMPONENT_ZERO_H

#include "../Engine.h"

namespace Gem
{
	class ComponentZero : public Component
	{
		RDECLARE_CLASS(ComponentZero)
	public:
		inline void SetDamage(float aDamage)
		{
			m_Damage = aDamage;
		}
		inline void SetHealth(float aHealth)
		{
			m_Health = aHealth;
		}
		inline void SetLevel(int aLevel)
		{
			m_Level = aLevel;
		}

		inline void SetSpawnPoint(Vector3 aSpawnPoint)
		{
			m_SpawnPoint = aSpawnPoint;
		}

		inline float GetDamage() const
		{
			return m_Damage;
		}

		inline float GetHealth() const
		{
			return m_Health;
		}

		inline int GetLevel() const
		{
			return m_Level;
		}

		inline Vector3 GetSpawnPoint() const
		{
			return m_SpawnPoint;
		}

	protected:

		
		void OnSerializeData(std::vector<InstructionTerm*>& aTerms) override;
		RDECLARE_PROTECTED_FUNCTION(ComponentZero, OnSerializeData)
		void OnDeserializeData(std::vector<InstructionTerm*>& aTerms) override;
		RDECLARE_PROTECTED_FUNCTION(ComponentZero, OnDeserializeData);
		

	private:

		float m_Damage;
		float m_Health;
		int m_Level;
		Vector3 m_SpawnPoint;

	};
	
	TYPE_DEFINE(ComponentZero)
}

#endif // GEM_COMPONENT_ZERO_H