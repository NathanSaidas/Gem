#ifndef GEM_EC_INSTRUCTION_TERM_H
#define GEM_EC_INSTRUCTION_TERM_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Math/MathTypes.h"
#include "IComplexTerm.h"

namespace Gem
{
	FORCE_EXPORT_META(InstructionTerm);

	class GEM_API InstructionTerm : public object
	{
		RDECLARE_CLASS(InstructionTerm)
	public:
		InstructionTerm();
		InstructionTerm(const std::string & aName, std::string aValue);
		InstructionTerm(const std::string & aName, SInt32 aValue);
		InstructionTerm(const std::string & aName, UInt32 aValue);
		InstructionTerm(const std::string & aName, bool aValue);
		InstructionTerm(const std::string & aName, Float32 aValue);
		InstructionTerm(const std::string & aName, const Vector2 & aValue);
		InstructionTerm(const std::string & aName, const Vector3 & aValue);
		InstructionTerm(const std::string & aName, const Vector4 & aValue);
		InstructionTerm(const std::string & aName, const Quaternion & aValue);
		InstructionTerm(const std::string & aName, const IComplexTerm * aValue);
		~InstructionTerm();

		std::string GetName() const;
		std::string GetValue() const;
		SInt32 AsInt() const;
		UInt32 AsUInt() const;
		bool AsBool() const;
		Float32 AsFloat() const;
		Vector2 AsVector2() const;
		Vector3 AsVector3() const;
		Vector4 AsVector4() const;
		Quaternion AsQuaternion() const;
		void AsComplexTerm(IComplexTerm* aValue);

		void SetName(std::string aName);
		void SetValue(std::string aValue);
		void SetValue(SInt32 aValue);
		void SetValue(UInt32 aValue);
		void SetValue(bool aValue);
		void SetValue(Float32 aValue);

		//Complex Constructors
		static InstructionTerm ComplexTerm(std::string aName, const Vector2 & aValue);
		static InstructionTerm ComplexTerm(std::string aName, const Vector3 & aValue);
		static InstructionTerm ComplexTerm(std::string aName, const Vector4 & aValue);
		static InstructionTerm ComplexTerm(std::string aName, const Quaternion & aValue);
		static InstructionTerm ComplexTerm(std::string aName, const IComplexTerm * aValue);
		//Complex Value Setters.

		void SetComplexValue(const Vector2 & aValue);
		void SetComplexValue(const Vector3 & aValue);
		void SetComplexValue(const Vector4 & aValue);
		void SetComplexValue(const Quaternion & aValue);
		void SetComplexValue(const IComplexTerm * aValue);

		std::string ToString() override;

		static InstructionTerm* GetTerm(std::vector<InstructionTerm*> & aTerms, const std::string & aName);

	private:
		std::string m_Name;
		std::string m_Value;
	};

	TYPE_DEFINE(InstructionTerm)

}


#endif