#ifndef GEM_EC_INSTRUCTION_DATA_H
#define GEM_EC_INSTRUCTION_DATA_H

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "InstructionTerm.h"
#include <string>

namespace Gem
{
	namespace EntityComponent
	{
		struct GEM_API InstructionData
		{
			/** The name of the token in a string format*/
			CString tokenIDName;
			/** The integer value of the token id*/
			SInt32 tokenID;
			/** A parameter reserved for special token operations.*/
			SInt32 reservedParam0;
			/** A parameter reserved for special token operations.*/
			SInt32 reservedParam1;
			/** Indicates how many parameters there are of an instruction set.*/
			SInt32 paramCount;

			//List of params...
			InstructionTerm ** terms;
		};
	}
	

}

#endif