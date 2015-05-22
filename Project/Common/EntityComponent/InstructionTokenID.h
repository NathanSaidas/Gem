#ifndef GEM_EC_INSTRUCTION_TOKEN_ID_H
#define GEM_EC_INSTRUCTION_TOKEN_ID_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Enum.h"

namespace Gem
{
	namespace EntityComponent
	{
		/**
		* Export the type.
		*/
		FORCE_EXPORT_META(InstructionTokenID);

		/**
		* Specify's the type of instruction tokens there are.
		*/
		class GEM_API InstructionTokenID : public Enum
		{
		public:
			enum GEM_API Values
			{
				AddGameObject,
				LinkGameObject,
				AddComponent,
				LinkComponent,
				Invalid
			};
			RDECLARE_ENUM(InstructionTokenID)
			RENUM_H(InstructionTokenID, Values, Values::Invalid +1)
		//private:
		//	static const std::string NAMES[Values::Invalid + 1];
		};

		

	}
	TYPE_DEFINE_NAMED(EntityComponent::InstructionTokenID,"InstructionTokenID")
}

#endif