#include "InstructionTerm.h"
#include "../Utilities/Utilities.h"

namespace Gem
{
	RDEFINE_CLASS(InstructionTerm, object)

	InstructionTerm::InstructionTerm() : object()
	{

	}
	InstructionTerm::InstructionTerm(const std::string & aName, std::string aValue) : object(),
		m_Name(aName),
		m_Value(aValue)
	{

	}
	InstructionTerm::InstructionTerm(const std::string & aName, SInt32 aValue) : object(),
		m_Name(aName)
	{
		SetValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, UInt32 aValue) : object(),
		m_Name(aName)
	{
		SetValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, bool aValue) : object(),
		m_Name(aName)
	{
		SetValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, Float32 aValue) : object(),
		m_Name(aName)
	{
		SetValue(aValue);
	}

	InstructionTerm::InstructionTerm(const std::string & aName, const Vector2 & aValue) : object(),
		m_Name(aName)
	{
		SetComplexValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, const Vector3 & aValue) : object(),
		m_Name(aName)
	{
		SetComplexValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, const Vector4 & aValue) : object(),
		m_Name(aName)
	{
		SetComplexValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, const Quaternion & aValue) : object(),
		m_Name(aName)
	{
		SetComplexValue(aValue);
	}
	InstructionTerm::InstructionTerm(const std::string & aName, const IComplexTerm * aValue) : object(),
		m_Name(aName)
	{
		SetComplexValue(aValue);
	}

	InstructionTerm::~InstructionTerm()
	{

	}

	std::string InstructionTerm::GetName() const
	{
		return m_Name;
	}
	std::string InstructionTerm::GetValue() const
	{
		return m_Value;
	}

	SInt32 InstructionTerm::AsInt() const
	{
		return Utilities::S2I(m_Value);
	}
	UInt32 InstructionTerm::AsUInt() const
	{
		return Utilities::S2UI(m_Value);
	}
	bool InstructionTerm::AsBool() const
	{
		return Utilities::S2I(m_Value) != 1;
	}
	Float32 InstructionTerm::AsFloat() const
	{
		return Utilities::S2F(m_Value);
	}
	Vector2 InstructionTerm::AsVector2() const
	{
		Vector2 vector;
		int vecIndex = -1;
		int startIndex = 0;
		int size = 0;

		for (int i = 0; i < m_Value.size(); i++)
		{
			char currentChar = m_Value[i];

			//Check if the currentChar is a vector element or end of complex term.
			//Must also be of size to carry an equals sign after and contain a number or something.
			if (((currentChar == 'x' || currentChar == 'y') || currentChar == ']' )
				&& i + 2 < m_Value.size()
				&& m_Value[i+1] == '=')
			{
				if (vecIndex == 0)
				{
					size = i - startIndex;
					vector.x = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 1)
				{
					size = i - startIndex;
					vector.y = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				startIndex = i + 2;
				vecIndex++;
				if (vecIndex > 2)
				{
					break;
				}
			}
		}

		return vector;
	}
	Vector3 InstructionTerm::AsVector3() const
	{
		Vector3 vector;
		int vecIndex = -1;
		int startIndex = 0;
		int size = 0;

		for (int i = 0; i < m_Value.size(); i++)
		{
			char currentChar = m_Value[i];

			//Check if the currentChar is a vector element or end of complex term.
			//Must also be of size to carry an equals sign after and contain a number or something.
			if (((currentChar == 'x' || currentChar == 'y' || currentChar == 'z') || currentChar == ']')
				&& i + 2 < m_Value.size()
				&& m_Value[i + 1] == '=')
			{
				if (vecIndex == 0)
				{
					size = i - startIndex;
					vector.x = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 1)
				{
					size = i - startIndex;
					vector.y = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 2)
				{
					size = i - startIndex;
					vector.z = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				startIndex = i + 2;
				vecIndex++;
				if (vecIndex > 2)
				{
					break;
				}
			}
		}

		return vector;
	}
	Vector4 InstructionTerm::AsVector4() const
	{
		Vector4 vector;
		int vecIndex = -1;
		int startIndex = 0;
		int size = 0;

		for (int i = 0; i < m_Value.size(); i++)
		{
			char currentChar = m_Value[i];

			//Check if the currentChar is a vector element or end of complex term.
			//Must also be of size to carry an equals sign after and contain a number or something.
			if (((currentChar == 'x' || currentChar == 'y' || currentChar == 'z' || currentChar == 'w') || currentChar == ']')
				&& i + 2 < m_Value.size()
				&& m_Value[i + 1] == '=')
			{
				if (vecIndex == 0)
				{
					size = i - startIndex;
					vector.x = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 1)
				{
					size = i - startIndex;
					vector.y = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 2)
				{
					size = i - startIndex;
					vector.z = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 3)
				{
					size = i - startIndex;
					vector.w = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				startIndex = i + 2;
				vecIndex++;
				if (vecIndex > 3)
				{
					break;
				}
			}
		}

		return vector;
	}
	Quaternion InstructionTerm::AsQuaternion() const
	{
		Quaternion quaternion;
		int vecIndex = -1;
		int startIndex = 0;
		int size = 0;

		for (int i = 0; i < m_Value.size(); i++)
		{
			char currentChar = m_Value[i];

			//Check if the currentChar is a vector element or end of complex term.
			//Must also be of size to carry an equals sign after and contain a number or something.
			if (((currentChar == 'x' || currentChar == 'y' || currentChar == 'z' || currentChar == 'w') || currentChar == ']')
				&& i + 2 < m_Value.size()
				&& m_Value[i + 1] == '=')
			{
				if (vecIndex == 0)
				{
					size = i - startIndex;
					quaternion.x = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 1)
				{
					size = i - startIndex;
					quaternion.y = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 2)
				{
					size = i - startIndex;
					quaternion.z = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				else if (vecIndex == 3)
				{
					size = i - startIndex;
					quaternion.w = Utilities::S2F(m_Value.substr(startIndex, size));
				}
				startIndex = i + 2;
				vecIndex++;
				if (vecIndex > 3)
				{
					break;
				}
			}
		}

		return quaternion;
	}

	void InstructionTerm::AsComplexTerm(IComplexTerm* aValue)
	{
		if (aValue != nullptr)
		{
			aValue->OnDeserialize(m_Value);
		}
	}

	void InstructionTerm::SetName(std::string aName)
	{
		m_Name = aName;
	}
	void InstructionTerm::SetValue(std::string aValue)
	{
		m_Value = aValue;
	}
	void InstructionTerm::SetValue(SInt32 aValue)
	{
		m_Value = Utilities::I2S(aValue);
	}
	void InstructionTerm::SetValue(UInt32 aValue)
	{
		m_Value = Utilities::UI2S(aValue);
	}
	void InstructionTerm::SetValue(bool aValue)
	{
		m_Value = Utilities::I2S(aValue == true ? 1 : 0);
	}
	void InstructionTerm::SetValue(Float32 aValue)
	{
		m_Value = Utilities::F2S(aValue);
	}

	InstructionTerm InstructionTerm::ComplexTerm(std::string aName, const Vector2 & aValue)
	{
		InstructionTerm term = InstructionTerm(aName, "");
		term.SetComplexValue(aValue);
		return term;
	}
	InstructionTerm InstructionTerm::ComplexTerm(std::string aName, const Vector3 & aValue)
	{
		InstructionTerm term = InstructionTerm(aName, "");
		term.SetComplexValue(aValue);
		return term;
	}
	InstructionTerm InstructionTerm::ComplexTerm(std::string aName, const Vector4 & aValue)
	{
		InstructionTerm term = InstructionTerm(aName, "");
		term.SetComplexValue(aValue);
		return term;
	}
	InstructionTerm InstructionTerm::ComplexTerm(std::string aName, const Quaternion & aValue)
	{
		InstructionTerm term = InstructionTerm(aName, "");
		term.SetComplexValue(aValue);
		return term;
	}
	InstructionTerm InstructionTerm::ComplexTerm(std::string aName, const IComplexTerm * aValue)
	{
		if (aValue == nullptr)
		{
			return InstructionTerm();
		}
		InstructionTerm term = InstructionTerm(aName, "");
		term.SetComplexValue(aValue);
		return term;
	}

	void InstructionTerm::SetComplexValue(const Vector2 & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append("y=").append(Utilities::F2S(aValue.y));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const Vector3 & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append("y=").append(Utilities::F2S(aValue.y));
		m_Value.append("z=").append(Utilities::F2S(aValue.z));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const Vector4 & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append("y=").append(Utilities::F2S(aValue.y));
		m_Value.append("z=").append(Utilities::F2S(aValue.z));
		m_Value.append("w=").append(Utilities::F2S(aValue.w));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const Quaternion & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append("y=").append(Utilities::F2S(aValue.y));
		m_Value.append("z=").append(Utilities::F2S(aValue.z));
		m_Value.append("w=").append(Utilities::F2S(aValue.w));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const IComplexTerm * aValue)
	{
		if (aValue != nullptr)
		{
			m_Value = std::string("[").append(aValue->OnSerialize()).append("]");
		}
	}

	std::string InstructionTerm::ToString()
	{
		return m_Name.append(" = ").append(m_Value);
	}

	InstructionTerm* InstructionTerm::GetTerm(std::vector<InstructionTerm*> & aTerms, const std::string & aName)
	{
		for (std::vector<InstructionTerm*>::iterator it = aTerms.begin(); it != aTerms.end(); it++)
		{
			if ((*it)->GetName() == aName)
			{
				return *it;
			}
		}
		return nullptr;
	}
}