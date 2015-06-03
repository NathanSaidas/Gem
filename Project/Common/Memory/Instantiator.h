#ifndef GEM_INSTANTIATOR_H
#define GEM_INSTANTIATOR_H

//============================================================
// Date:			May,	10,		2015
// Description:		Contains an extended error class, InvalidArgument. 
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Implemented Class.
#pragma endregion

#include "../Core/GemAPI.h"

namespace Gem
{
	namespace Memory
	{
		template<typename T>
		class Instantiator
		{
		public:
			template<typename ... Types>
			static T* Construct(void* aAddress, Types ... args)
			{
				return new(aAddress)T(args...);
			}
			static void Destruct(T* aAddress)
			{
				aAddress->~T();
			}
		};
	}
}

#endif // GEM_INSTATIATOR_H