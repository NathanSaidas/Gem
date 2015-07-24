#ifndef GEM_INI_VARIABLES_H
#define GEM_INI_VARIABLES_H


#include <sstream>
#include <fstream>
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Debug.h"

#pragma warning (push) 
#pragma warning (disable : 4244)




namespace Gem
{
	const static std::string INI_BOOL = "Inibool";
	const static std::string INI_INT = "Iniint";
	const static std::string INI_FLOAT = "Inifloat";
	const static std::string INI_STRING = "Inistring";
	const static std::string INI_VECTOR_3 = "IniVector3";
	const static std::string INI_VECTOR_4 = "IniVector4";
	const static std::string INI_BAD_VARIABLE_NAME = "BAD_NAME";


	/// <summary>
	/// A class template for all variables types accepted within the IniFile module.
	/// A variable stores the value of the given TYPE and a name to identify it with.
	/// The name of the IniVariable object uses "Ini" as the prefix and postfixes it with the TYPE name retrieved from Reflection::TypeTrait
	///
	/// Currently Supported Types: bool, int, float, std::string.
	/// An unsupported type recieves a variable name of INI_BAD_VARIABLE_NAME (refer to constants).
	/// </summary>
	template<typename TYPE>
	class GEM_API IniVariable : public Object
	{
	public:
		IniVariable() : Object()
		{
			m_VariableName = "";
			memset(&m_Value, 0, sizeof(TYPE));
            SetName(std::string("Ini").append(Reflection::TypeTrait<TYPE>::Name()));
		}
        IniVariable(const std::string & aVariableName) : Object()
		{
			m_VariableName = aVariableName;
			memset(&m_Value, 0, sizeof(TYPE));
            SetName(std::string("Ini").append(Reflection::TypeTrait<TYPE>::Name()));
		}
		IniVariable(CString aVariableName) : Object()
		{
			m_VariableName = aVariableName;
			memset(&m_Value, 0, sizeof(TYPE));
            SetName(std::string("Ini").append(Reflection::TypeTrait<TYPE>::Name()));
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
		TYPE GetValue()
		{
			return m_Value;
		}

		/// <summary>
		/// Sets the variable value
		/// </summary>
		/// <param name="aValue">The value of the variable.</param>
		void SetValue(const TYPE & aValue)
		{
			m_Value = aValue;
		}

		/// <summary>
		/// Determines if a variable is a bad variable or not.
		/// </summary>
		/// <returns> Returns true if the variable is a bad variable. </returns>
        static bool IsBadValue(IniVariable<TYPE> & aType)
        {
            return aType.GetVariableName() == INI_BAD_VARIABLE_NAME;
        }
		

		/// <summary>
		/// Override the object.ToString() method to a string representation of the type name variable name and value
		/// </summary>
		/// <returns> Returns the variable value </returns>
        std::string ToString()
		{
			//Format ex. type_name variable_name = value
			//Example: int points = 64

			const char * typeName = Reflection::TypeTrait<TYPE>::Name();
            std::string value = "";

			//Is int type
			if (strcmp(typeName, Reflection::TypeTrait<int>::Name()) == 0 ||
				strcmp(typeName, Reflection::TypeTrait<unsigned int>::Name()) == 0
				)
			{
				//Convert to string
				try
				{
					std::stringstream converterStream;
					converterStream << m_Value;
					value = converterStream.str();
				}
				catch (std::exception aException)
				{
					//DEBUG_LOG("Error parsing string of IniVariable.\nError Info: %s", aException.what());
					Debugging::Debug::ErrorFormat("Gem", nullptr, "%s", aException.what());
				}
				
			}
			//As float value
			else if (strcmp(typeName, Reflection::TypeTrait<Float32>::Name()) == 0)
			{
				try
				{
					//Convert to string
					std::stringstream converterStream;
					converterStream << m_Value;
					value = converterStream.str();
					
					//Convert back to float for type
					float floatValue = 0.0f;
					std::stringstream floatConverterStream(value);
					if (!(floatConverterStream >> floatValue))
					{
						floatValue = 0.0f;
					}
					
					//Append .0 to non-fractional float values
					if (floatValue == (int)floatValue)
					{
						value.append(".0");
					}
				}
				catch (std::exception aException)
				{
					Debugging::Debug::ErrorFormat("Gem", nullptr, "%s", aException.what());
					//DEBUG_LOG("Error parsing string of IniVariable.\nError Info: %s", aException.what());
				}
			}
			//As bool value
			else if (strcmp(typeName, Reflection::TypeTrait<bool>::Name()) == 0)
			{
				try
				{
					//Convert to string
					std::stringstream converterStream;
					converterStream << m_Value;
					if (converterStream.str() == "0")
					{
						value = "false";
					}
					else
					{
						value = "true";
					}
				}
				catch (std::exception aException)
				{
					//DEBUG_LOG("Error parsing string of IniVariable.\nError Info: %s", aException.what());
					Debugging::Debug::ErrorFormat("Gem", nullptr, "%s", aException.what());
				}
			}
			//As string values
            else if (strcmp(typeName, Reflection::TypeTrait<std::string>::Name()) == 0)
			{
				value = m_Value;
			}
			else
			{
				//DEBUG_LOG("Invalid variable type. Cannot convert value.");
				Debugging::Debug::ErrorFormat("Gem", nullptr, "Invalid type, cannot convert value");
				return "";
			}
			//Return a copy of the string with the proper format.
            return std::string(typeName).append(" ").append(m_VariableName).append(" = ").append(value);;
		}

	private:
		/// <summary>
		/// The name of the variable
		/// </summary>
        std::string m_VariableName;
		/// <summary>
		/// The value of the variable.
		/// </summary>
		TYPE m_Value;

	};

	// The supported variable types.

	typedef IniVariable<bool> IniBool;
	typedef IniVariable<SInt32> IniInt;
	typedef IniVariable<Float32> IniFloat;
	typedef IniVariable<std::string> IniString;

	template class GEM_API Reflection::MetaObject<IniBool>;
	template class GEM_API Reflection::MetaObject<IniInt>;
	template class GEM_API Reflection::MetaObject<IniFloat>;
	template class GEM_API Reflection::MetaObject<IniString>;

	TYPE_DEFINE(IniBool);
	TYPE_DEFINE(IniInt);
	TYPE_DEFINE(IniFloat);
	TYPE_DEFINE(IniString);
	
}

#pragma warning(pop)

#endif //GEM_INI_VARIABLES_H