#ifndef GEM_EC_INSTRUCTION_TERM_H
#define GEM_EC_INSTRUCTION_TERM_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Math/MathTypes.h"
#include "IComplexTerm.h"

namespace Gem
{
	namespace EntityComponent
	{
		FORCE_EXPORT_META(InstructionTerm);

		class GEM_API InstructionTerm : public object
		{
			RDECLARE_CLASS(InstructionTerm)
		public:
			InstructionTerm();
			InstructionTerm(std::string aName, std::string aValue);
			InstructionTerm(std::string aName, SInt32 aValue);
			InstructionTerm(std::string aName, UInt32 aValue);
			InstructionTerm(std::string aName, bool aValue);
			InstructionTerm(std::string aName, Float32 aValue);
			~InstructionTerm();

			std::string GetName() const;
			std::string GetValue() const;

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

		private:
			std::string m_Name;
			std::string m_Value;
		};
	}

	TYPE_DEFINE_NAMED(EntityComponent::InstructionTerm, "Term")

}


#endif