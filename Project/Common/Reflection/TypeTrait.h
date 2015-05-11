#ifndef GAME_ENGINE_TYPE_TRAIT_H
#define GAME_ENGINE_TYPE_TRAIT_H

#pragma region CHANGE LOG
/// -- January, 29, 2015 - Nathan Hanlan - Added TypeTrait class/file
#pragma endregion

namespace Engine
{
	namespace Reflection
	{
		template<typename T>
		struct TypeTrait
		{
		public:
			static inline const char * Name()
			{
				return "unknown";
			}
			static const bool IS_POINTER = false;
		};


	#define TYPE_DEFINE(TYPE)										\
	template<>														\
		struct Engine::Reflection::TypeTrait < TYPE >				\
		{															\
		public:														\
		static inline const char * Name()							\
		{															\
		return #TYPE;												\
		}															\
		static const bool IS_POINTER = false;						\
		};															\
		template<>													\
		struct Engine::Reflection::TypeTrait< TYPE ## *>			\
		{															\
		public:														\
		static inline const char * Name()							\
		{															\
		return #TYPE;												\
		}															\
		static const bool IS_POINTER = true;						\
		};															\
		template<>													\
		struct Engine::Reflection::TypeTrait< TYPE ## **>			\
		{															\
		public:														\
		static inline const char * Name()							\
		{															\
		return #TYPE;												\
		}															\
		static const bool IS_POINTER = true;						\
		};															\

	}
}


#endif