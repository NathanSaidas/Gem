#ifndef GEM_INI_VECTOR3_H
#define GEM_INI_VECTOR3_H

#include "../Core/GemAPI.h"
#include "../Math/Vector3.h"
#include "StringExtensions.h"
#include "IniVariables.h"



namespace Gem
{
	

	template<>
	class GEM_API IniVariable<Vector3> : public Object
	{
	public:
		IniVariable() : Object()
		{
			m_VariableName = "";
			SetName(std::string("Ini").append(Reflection::TypeTrait<Vector3>::Name()));
		}
		IniVariable(const std::string & aVariableName) : Object()
		{
			m_VariableName = aVariableName;
			SetName(std::string("Ini").append(Reflection::TypeTrait<Vector3>::Name()));
		}
		IniVariable(const char * aVariableName) : Object()
		{
			m_VariableName = aVariableName;
			SetName(std::string("Ini").append(Reflection::TypeTrait<Vector3>::Name()));
		}
		~IniVariable()
		{
		}

		/// <summary>
		/// Gets the variable name.
		/// </summary>
		/// <returns> Returns the variable name </returns>
		std::string GetVariableName()
		{
			return m_VariableName;
		}
		/// <summary>
		/// Sets the variable name
		/// </summary>
		/// <param name="aVariableName">The name of the variable.</param>
		void SetVariableName(const std::string & aVariableName)
		{
			m_VariableName = aVariableName;
		}
		/// <summary>
		/// Sets the variable name
		/// </summary>
		/// <param name="aVariableName">The name of the variable.</param>
		void SetVariableName(CString aVariableName)
		{
			m_VariableName = aVariableName;
		}

		/// <summary>
		/// Gets the variable value.
		/// </summary>
		/// <returns> Returns the variable value </returns>
		Vector3 GetValue()
		{
			return m_Value;
		}

		/// <summary>
		/// Sets the variable value
		/// </summary>
		/// <param name="aValue">The value of the variable.</param>
		void SetValue(const Vector3 & aValue)
		{
			m_Value = aValue;
		}

		/// <summary>
		/// Determines if a variable is a bad variable or not.
		/// </summary>
		/// <returns> Returns true if the variable is a bad variable. </returns>
		static bool IsBadValue(IniVariable<Vector3> & aType)
		{
			return aType.GetName() == INI_BAD_VARIABLE_NAME;
		}


		/// <summary>
		/// Override the object.ToString() method to a string representation of the type name variable name and value
		/// </summary>
		/// <returns> Returns the variable value </returns>
		std::string ToString()
		{
			//Format ex. type_name variable_name = value
			//Example: int points = 64

			CString typeName = Reflection::TypeTrait<Vector3>::Name();
			std::string value = "";

			if (strcmp(typeName, Reflection::TypeTrait<Vector3>::Name()) == 0)
			{
				value = "x: ";
				std::string sValue = Utilities::F2S(m_Value.x);
				value.append(sValue).append(" y: ");
				sValue = Utilities::F2S(m_Value.y);
				value.append(sValue).append(" z: ");
				sValue = Utilities::F2S(m_Value.z);
				value.append(sValue);
			}
			else
			{
				//DEBUG_LOG("Invalid variable type. Cannot convert value.");
				Debugging::Debug::Error("Gem","Invalid variable type, Cannot convert value.");
				return "";
			}
			//Return a copy of the string with the proper format.
			return std::string(typeName).append(" ").append(m_VariableName).append(" = ").append(value);
		}

	private:
		/// <summary>
		/// The name of the variable
		/// </summary>
		std::string m_VariableName;
		/// <summary>
		/// The value of the variable.
		/// </summary>
		Vector3 m_Value;

	};

	typedef IniVariable<Vector3> IniVector3;

	template class GEM_API Reflection::MetaObject<IniVector3>;

	TYPE_DEFINE(IniVector3);
}

#endif // GEM_INI_VECTOR_3_H