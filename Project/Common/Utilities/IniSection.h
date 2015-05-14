#ifndef GEM_INI_SECTION_H
#define GEM_INI_SECTION_H


#include <vector>
#include "IniVariables.h"
#include "IniVector3.h"
#include "IniVector4.h"
#include "../Core/BasicTypes.h"


namespace Gem
{
	class IniSection;
	template class GEM_API Reflection::MetaObject<IniSection>;

	template class GEM_API std::vector<Object*>;

	class GEM_API IniSection : public Object
	{
		RDECLARE_CLASS(IniSection);
	public:
		IniSection();
		~IniSection();


		void Write(std::ofstream & aFileStream);
		void Read(std::fstream & aFileStream);

        void LogConsole();

		// -- Creates a new variable with the name and value. 
		void AddBool(const std::string & aVariableName, bool aValue);
		void AddInt(const std::string & aVariableName, int aValue);
		void AddFloat(const std::string & aVariableName, float aValue);
		void AddString(const std::string & aVariableName, std::string aValue);
		void AddVector3(const std::string & aVariableName, Vector3 aValue);
		void AddVector4(const std::string & aVariableName, Vector4 aValue);

		// -- Searches for a variable by name
		IniBool GetBool(const std::string & aVariableName);
		IniInt GetInt(const std::string & aVariableName);
		IniFloat GetFloat(const std::string & aVariableName);
		IniString GetString(const std::string & aVariableName);
		IniVector3 GetVector3(const std::string & aVariableName);
		IniVector4 GetVector4(const std::string & aVariableName);

		// -- Sets values
		void SetBool(const std::string & aVariableName, bool aValue);
		void SetInt(const std::string & aVariableName, int aValue);
		void SetFloat(const std::string & aVariableName, float aValue);
		void SetString(const std::string & aVariableName, std::string aValue);
		void SetVector3(const std::string & aVariableName, Vector3 aValue);
		void SetVector4(const std::string & aVariableName, Vector4 aValue);

		// -- Removes a variable by name
		bool RemoveVariable(const std::string & aVariableName);

		bool VariableExists(const std::string & aVariableName);

	private:
		

		std::vector<Object*> m_Variables;
	};
}

#endif // GEM_INI_SECTION_H