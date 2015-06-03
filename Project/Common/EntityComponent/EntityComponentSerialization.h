#ifndef GEM_ENTITY_COMPONENT_H
#define GEM_ENTITY_COMPONENT_H

#include "../Memory/Memory.h"
#include "InstructionTerm.h"


#define SERIALIZE_TERM(NAME) MEM_FRAME_ALLOC_T(InstructionTerm,#NAME, NAME)

#endif