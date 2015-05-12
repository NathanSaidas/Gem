#ifndef GEM_SINGLETON_H
#define GEM_SINGLETON_H

//============================================================
// Date:			May,	11,		2015
// Description:		This is the root file to include which includes the whole Gem Framework/Engine.
//============================================================

#pragma region CHANGE LOG

#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"

namespace Gem
{
	template<typename T>
	class Singleton
	{
	public:
		virtual ~Singleton()
		{

		}
		static T * GetInstance()
		{
			return s_Instance;
		}
		static void Initialize()
		{
			if (s_Instance == nullptr)
			{
				s_Instance = new T();
			}
		}
		static void Terminate()
		{
			if (s_Instance != nullptr)
			{
				delete s_Instance;
				s_Instance = nullptr;
			}
		}


	protected:
		static T * s_Instance;
		Singleton();
	};

	//template<typename T>
	//T * Singleton<T>::s_Instance = nullptr;

}

#endif