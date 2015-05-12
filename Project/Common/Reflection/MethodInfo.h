#ifndef GAME_ENGINE_METHOD_INFO_H
#define GAME_ENGINE_METHOD_INFO_H

#pragma region CHANGE LOG
/// -- January, 30, 2015 - Nathan Hanlan - Added MethodInfo file and MethodInfo / MethodActionInfo classes 
/// -- May,		12, 2015 - Nathan Hanlan - Refactored MethodInfo to use the new Method class with variadic templates. Works as before.
#pragma region

#include "../Core/GemAPI.h"
#include "../Core/Method.h"
#include "MemberInfo.h"

namespace Gem
{
    namespace Reflection
    {

		template<typename CLASS, typename RETURN, typename ... ARGS>
		class GEM_API MethodInfo : public MemberInfo
		{
		public:
			MethodInfo()
			{
				m_Method = nullptr;
			}
			MethodInfo(Method<CLASS,RETURN,ARGS...> aFunction, const char * aClassTypename, const char * aMemberName, const char * aMemberTypename, size_t aOffset, bool aIsPublic)
				: MemberInfo(aClassTypename,aMemberName,aMemberTypename,aOffset,aIsPublic)
			{
				m_Method = aFunction;
			}
			Method<CLASS, RETURN, ARGS...> GetMethod()
			{
				return m_Method;
			}
		private:
			Method<CLASS, RETURN, ARGS...> m_Method;
		};
    }
    
}

#endif