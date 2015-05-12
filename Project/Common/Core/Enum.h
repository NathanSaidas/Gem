#ifndef GAME_ENGINE_ENUM_H
#define GAME_ENGINE_ENUM_H

#pragma region CHANGE LOG
/// -- April	3, 2015 - Nathan Hanlan - Added class Enum. This will be the base class of all future enums.
#pragma endregion

//TODO(Nathan): Create proper name for enum DECLARE

#include "BasicTypes.h"
#include "..\Utilities\Array.h"

namespace Gem
{
	/**
	* Base class for enum types.
	*/
	class Enum : public object
	{
		RDECLARE_ENUM(Enum)
	protected:
		/**
		* Override and implement to return a string version of the value.
		* @return Returns a string version of the value.
		*/
		virtual std::string GetName();
		/**
		* Override and implement to return an array of strings corresponding with the available values.
		* @return Returns an array of strings corresponding with the available values.
		*/
		virtual Array<std::string> GetNames();
		/**
		* Override and implement to return the current value in an integer format.
		* @return Returns the value in an integer format.
		*/
		virtual SInt32 GetValue();
		/**
		* Override and implement to return the available values
		* @return Returns the available values.
		*/
		virtual Array<SInt32> GetValues();
	public:
		/**
		* Returns the string version of the enum.
		* @param aEnum The enum to get the name from.
		* @return Returns a string version of the value.
		*/
		static std::string GetName(Enum * aEnum);
		/**
		* Returns the array of strings corresponding with the available values.
		* @param aEnum The enum to get the names from.
		* @return Returns an array of strings corresponding with the available values.
		*/
		static Array<std::string> GetNames(Enum * aEnum);
		/**
		* Returns the value of the enum in integer format.
		* @param aEnum The enum to get the value from.
		* @return Returns the value in an integer format.
		*/
		static SInt32 GetValue(Enum * aEnum);
		/**
		* Returns the available values of the enum in integer format.
		* @param enum The enum to get the values from.
		* @return Returns the available values.
		*/
		static Array<SInt32> GetValues(Enum * aEnum);

		/**
		* Returns the name of the enum. 
		* @return Returns the name of the enum.
		*/
		virtual std::string ToString();

	};
}

#define RENUM_H(TYPE,VALUES) private: VALUES m_Value;  \
	protected:																\
		std::string GetName();												\
		Engine::Array<std::string> GetNames();								\
		Engine::SInt32 GetValue();											\
		Engine::Array<Engine::SInt32> GetValues();							\
	public:																	\
		TYPE();																\
		TYPE(const TYPE::VALUES & aValue);									\
		TYPE(Engine::SInt32 aValue);										\
																			\
		inline TYPE operator=(const TYPE::VALUES & aValue)					\
		{																	\
			m_Value = aValue;												\
			return *this;													\
		}																	\
		inline TYPE operator=(int aValue)									\
		{																	\
			m_Value = (TYPE::VALUES)aValue;									\
			return *this;													\
		}																	\
		inline bool operator==(const TYPE::VALUES & aValue)					\
		{																	\
			return m_Value == aValue;										\
		}																	\
		inline bool operator==(const TYPE & aEnum)							\
		{																	\
			return m_Value == aEnum.m_Value;								\
		}																	\
		inline bool operator!=(const TYPE::VALUES & aValue)					\
		{																	\
			return m_Value != aValue;										\
		}																	\
		inline bool operator!=(const TYPE & aEnum)							\
		{																	\
			return m_Value != aEnum.m_Value;								\
		}																	\


#define RENUM_CPP(TYPE,VALUES,COUNT,VALUE_NAMES)	TYPE::TYPE()	\
	{																				\
		m_Value = (VALUES)0;														\
	}																				\
	TYPE::TYPE(const TYPE::VALUES & aValue)											\
	{																				\
		m_Value = aValue;															\
	}																				\
	TYPE::TYPE(Engine::SInt32 aValue)												\
	{																				\
		m_Value = (VALUES)aValue;													\
	}																				\
	std::string TYPE::GetName()														\
	{																				\
		return VALUE_NAMES[(Engine::SInt32)m_Value];								\
	}																				\
	Engine::Array<std::string> TYPE::GetNames()										\
	{																				\
		Engine::Array<std::string> names(COUNT);									\
		for (int i = 0; i < COUNT; i++)												\
		{																			\
			names[i] = VALUE_NAMES[i];												\
		}																			\
		return names;																\
	}																				\
	Engine::SInt32 TYPE::GetValue()													\
	{																				\
		return (Engine::SInt32)m_Value;												\
	}																				\
	Engine::Array<Engine::SInt32> TYPE::GetValues()									\
	{																				\
		Engine::Array<Engine::SInt32> values(COUNT);								\
		for (int i = 0; i < COUNT; i++)												\
		{																			\
			values[i] = i;															\
		}																			\
		return values;																\
	}																				\

#endif