#include "IniSection.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(IniSection, Object);
	

	IniSection::IniSection() : Object()
	{
		SetName("");
	}

	IniSection::~IniSection()
	{
		for (int i = m_Variables.size() - 1; i >= 0; i--)
		{
			delete m_Variables[i];
			m_Variables[i] = nullptr;
		}
		m_Variables.clear();
	}

	void IniSection::Write(std::ofstream & aFileStream)
	{
		aFileStream << "[" << GetName() << "]\n";

		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			aFileStream << (*it)->ToString() << "\n";
		}
	}
	void IniSection::Read(std::fstream & aFileStream)
	{

	}

    void IniSection::LogConsole()
    {
        //DEBUG_LOG("[ %s ]", GetName().c_str());

		Debug::LogFormat("Gem", nullptr, "[ %s ]", GetName().c_str());
        for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
        {
            //DEBUG_LOG((*it)->ToString().c_str(),"");
			Debug::Log("Gem", (*it)->ToString().c_str());
        }
    }

	// -- Creates a new variable with the name and value. 
	void IniSection::AddBool(const std::string & aVariableName, bool aValue)
	{
		if (!VariableExists(aVariableName))
		{
			IniBool * variable = new IniBool();
			variable->SetVariableName(aVariableName);
			variable->SetValue(aValue);
			m_Variables.push_back(variable);
		}
	}
	void IniSection::AddInt(const std::string & aVariableName, int aValue)
	{
		if (!VariableExists(aVariableName))
		{
			IniInt * variable = new IniInt();
			variable->SetVariableName(aVariableName);
			variable->SetValue(aValue);
			m_Variables.push_back(variable);
		}
	}
	void IniSection::AddFloat(const std::string & aVariableName, float aValue)
	{
		if (!VariableExists(aVariableName))
		{
			IniFloat * variable = new IniFloat();
			variable->SetVariableName(aVariableName);
			variable->SetValue(aValue);
			m_Variables.push_back(variable);
		}
	}
	void IniSection::AddString(const std::string & aVariableName, std::string aValue)
	{
		if (!VariableExists(aVariableName))
		{
			IniString * variable = new IniString();
			variable->SetVariableName(aVariableName);
			variable->SetValue(aValue);
			m_Variables.push_back(variable);
		}
	}

	void IniSection::AddVector3(const std::string & aVariableName, Vector3 aValue)
	{
		if (!VariableExists(aVariableName))
		{
			IniVector3 * variable = new IniVector3();
			variable->SetVariableName(aVariableName);
			variable->SetValue(aValue);
			m_Variables.push_back(variable);
		}
	}

	void IniSection::AddVector4(const std::string & aVariableName, Vector4 aValue)
	{
		if (!VariableExists(aVariableName))
		{
			IniVector4 * variable = new IniVector4();
			variable->SetVariableName(aVariableName);
			variable->SetValue(aValue);
			m_Variables.push_back(variable);
		}
	}

	// -- Searches for a variable by name
	IniBool IniSection::GetBool(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_BOOL)
			{
				IniBool * boolVar = dynamic_cast<IniBool*>(*it);
				if (boolVar != nullptr && boolVar->GetVariableName() == aVariableName)
				{
					//IniBool variable;
					//variable.SetName(boolVar->GetName());
					//variable.SetVariableName(boolVar->GetVariableName());
					//variable.SetValue(boolVar->GetValue());
					//return variable;
                    return *boolVar;
				}
			}
		}
		return IniBool(INI_BAD_VARIABLE_NAME);
	}
	IniInt IniSection::GetInt(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_INT)
			{
				IniInt * intVar = dynamic_cast<IniInt*>(*it);
				if (intVar != nullptr && intVar->GetVariableName() == aVariableName)
				{
                    return *intVar;
				}
			}
		}
		return IniInt(INI_BAD_VARIABLE_NAME);
	}
	IniFloat IniSection::GetFloat(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_FLOAT)
			{
				IniFloat * floatVar = dynamic_cast<IniFloat*>(*it);
				if (floatVar != nullptr && floatVar->GetVariableName() == aVariableName)
				{
                    return *floatVar;
				}
			}
		}
		return IniFloat(INI_BAD_VARIABLE_NAME);
	}
	IniString IniSection::GetString(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_STRING)
			{
				IniString * stringVar = dynamic_cast<IniString*>(*it);
				if (stringVar != nullptr && stringVar->GetVariableName() == aVariableName)
				{
                    return *stringVar;
				}
			}
		}
		return IniString(INI_BAD_VARIABLE_NAME);
	}

	IniVector3 IniSection::GetVector3(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_VECTOR_3)
			{
				IniVector3 * vectorVar = dynamic_cast<IniVector3*>(*it);
				if (vectorVar != nullptr && vectorVar->GetVariableName() == aVariableName)
				{
					return *vectorVar;
				}
			}
		}
		return IniVector3(INI_BAD_VARIABLE_NAME);
	}

	IniVector4 IniSection::GetVector4(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_VECTOR_4)
			{
				IniVector4 * vectorVar = dynamic_cast<IniVector4*>(*it);
				if (vectorVar != nullptr && vectorVar->GetVariableName() == aVariableName)
				{
					return *vectorVar;
				}
			}
		}
		return IniVector4(INI_BAD_VARIABLE_NAME);
	}

	void IniSection::SetBool(const std::string & aVariableName, bool aValue)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_BOOL)
			{
				IniBool * variable = dynamic_cast<IniBool*>(*it);
				if (variable != nullptr && variable->GetVariableName() == aVariableName)
				{
					variable->SetValue(aValue);
				}
			}
		}
	}
	void IniSection::SetInt(const std::string & aVariableName, int aValue)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_INT)
			{
				IniInt * variable = dynamic_cast<IniInt*>(*it);
				if (variable != nullptr && variable->GetVariableName() == aVariableName)
				{
					variable->SetValue(aValue);
				}
			}
		}
	}
	void IniSection::SetFloat(const std::string & aVariableName, float aValue)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_FLOAT)
			{
				IniInt * variable = dynamic_cast<IniInt*>(*it);
				if (variable != nullptr && variable->GetVariableName() == aVariableName)
				{
					variable->SetValue(aValue);
				}
			}
		}
	}
	void IniSection::SetString(const std::string & aVariableName, std::string aValue)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_STRING)
			{
				IniString * variable = dynamic_cast<IniString*>(*it);
				if (variable != nullptr && variable->GetVariableName() == aVariableName)
				{
					variable->SetValue(aValue);
				}
			}
		}
	}
	void IniSection::SetVector3(const std::string & aVariableName, Vector3 aValue)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_VECTOR_3)
			{
				IniVector3 * variable = dynamic_cast<IniVector3*>(*it);
				if (variable != nullptr && variable->GetVariableName() == aVariableName)
				{
					variable->SetValue(aValue);
				}
			}
		}
	}
	void IniSection::SetVector4(const std::string & aVariableName, Vector4 aValue)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			if ((*it)->GetName() == INI_VECTOR_4)
			{
				IniVector4 * variable = dynamic_cast<IniVector4*>(*it);
				if (variable != nullptr && variable->GetVariableName() == aVariableName)
				{
					variable->SetValue(aValue);
				}
			}
		}
	}

	// -- Removes a variable by name
	bool IniSection::RemoveVariable(const std::string & aVariableName)
	{
		for (std::vector<Object*>::iterator it = m_Variables.begin(); it != m_Variables.end(); it++)
		{
			std::string typeName = (*it)->GetName();

			if (typeName == INI_BOOL)
			{
				IniBool * boolVar = dynamic_cast<IniBool*>(*it);
				if (boolVar != nullptr && boolVar->GetVariableName() == aVariableName)
				{
					m_Variables.erase(it);
					return true;
				}
			}
			else if (typeName == INI_INT)
			{
				IniInt * intVar = dynamic_cast<IniInt*>(*it);
				if (intVar != nullptr && intVar->GetVariableName() == aVariableName)
				{
					m_Variables.erase(it);
					return true;
				}
			}
			else if (typeName == INI_FLOAT)
			{
				IniFloat * floatVar = dynamic_cast<IniFloat*>(*it);
				if (floatVar != nullptr && floatVar->GetVariableName() == aVariableName)
				{
					m_Variables.erase(it);
					return true;
				}
			}
			else if (typeName == INI_STRING)
			{
				IniString * stringVar = dynamic_cast<IniString*>(*it);
				if (stringVar != nullptr && stringVar->GetVariableName() == aVariableName)
				{
					m_Variables.erase(it);
					return true;
				}
			}

		}
		return false;
	}

	bool IniSection::VariableExists(const std::string & aVariableName)
	{
		if (GetBool(aVariableName).GetVariableName() != INI_BAD_VARIABLE_NAME)
		{
			return true;
		}
		else if (GetInt(aVariableName).GetVariableName() != INI_BAD_VARIABLE_NAME)
		{
			return true;
		}
		else if (GetFloat(aVariableName).GetVariableName() != INI_BAD_VARIABLE_NAME)
		{
			return true;
		}
		else if (GetString(aVariableName).GetVariableName() != INI_BAD_VARIABLE_NAME)
		{
			return true;
		}
		return false;
	}
}