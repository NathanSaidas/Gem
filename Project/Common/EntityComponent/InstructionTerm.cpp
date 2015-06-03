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
		m_Value.append(",y=").append(Utilities::F2S(aValue.y));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const Vector3 & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append(",y=").append(Utilities::F2S(aValue.y));
		m_Value.append(",z=").append(Utilities::F2S(aValue.z));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const Vector4 & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append(",y=").append(Utilities::F2S(aValue.y));
		m_Value.append(",z=").append(Utilities::F2S(aValue.z));
		m_Value.append(",w=").append(Utilities::F2S(aValue.w));
		m_Value.append("]");
	}
	void InstructionTerm::SetComplexValue(const Quaternion & aValue)
	{
		m_Value = std::string("[x=").append(Utilities::F2S(aValue.x));
		m_Value.append(",y=").append(Utilities::F2S(aValue.y));
		m_Value.append(",z=").append(Utilities::F2S(aValue.z));
		m_Value.append(",w=").append(Utilities::F2S(aValue.w));
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
		return m_Name.append("=").append(m_Value);
	}
}