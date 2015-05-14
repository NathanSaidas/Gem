#include "IniFileStream.h"
#include "Utilities.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(IniFileStream,object)

	IniFileStream::IniFileStream()
	{

	}
	IniFileStream::~IniFileStream()
	{
        Clear();
	}

	/// <summary>
	/// Clears all variables. Opens up the file, reads in all variables. Closes the file.
	/// </summary>
	void IniFileStream::Read()
	{
		//Clear the Variables
        Clear();
		
		//Attempt to Open the File
        std::ifstream fileStream;
        fileStream.open(m_Path);
        if (!fileStream.is_open())
        {
            return;
        }

		//Parse each line in the line buffer
        std::string line; 
		//Parse each variable line into words to get the typename, variable name, and value
        std::vector<std::string> words; 

        while (std::getline(fileStream, line))
        {
            if (line.size() == 0)
            {
                continue;
            }

			//Section Defined
            if (line[0] == '[')
            {
                std::string sectionName = line.substr(1, line.size() - 2);
                
                AddSection(sectionName);
                BindSection(sectionName);
                continue;
            }
			else if (line[0] == '#')
			{
				continue;
			}
			//Variable Defined
            else
            {
                Utilities::GetWords(line, words);
                if (!(
					words.size() == 4 || 
					words.size() == 9 || 
					words.size() == 11))
                {
					Debug::Log("Gem", "Error reading line");
                }
                else
                {
                    std::string typeName = words[0];
                    std::string variableName = words[1];
                    std::string variableValue = words[3];

                    if (typeName == "int")
                    {
                        int value = atoi(variableValue.c_str());
                        AddInt(variableName, value);
                    }
                    else if (typeName == "float")
                    {
                        float value = (float)atof(variableValue.c_str());
                        AddFloat(variableName, value);
                    }
                    else if (typeName == "bool")
                    {
                        bool value = variableValue == "true";
                        AddBool(variableName, value);
                    }
                    else if (typeName == "string" || typeName == "std::string")
                    {
                        AddString(variableName, variableValue);
                    }
					else if (typeName == "Vector3")
					{
						Vector3 vec;
						vec.x = Utilities::S2F(words[4]);
						vec.y = Utilities::S2F(words[6]);
						vec.z = Utilities::S2F(words[8]);
						AddVector3(variableName, vec);
					}
					else if (typeName == "Vector4")
					{
						Vector4 vec;

						vec.x = Utilities::S2F(words[4]);
						vec.y = Utilities::S2F(words[6]);
						vec.z = Utilities::S2F(words[8]);
						vec.w = Utilities::S2F(words[10]);
						AddVector4(variableName, vec);
					}
                    else
                    {
						Debug::LogFormat("Gem", nullptr, "Unsupported type %s", typeName);
                    }
                }
				//Clear the words list after
                words.clear();
            }
        }

        fileStream.close();
	}
	/// <summary>
	/// Opens up the file, saves all variables to the file. Closes the file.
	/// </summary>
	void IniFileStream::Save()
	{
		//Attempt to open the file for writing.
		std::ofstream fileStream;
		fileStream.open(m_Path);
		if (!fileStream.is_open())
		{
			Debug::ErrorFormat("Gem", nullptr, "Error opening the file to the filepath %s", m_Path.c_str());
			return;
		}

		//Write out each section
		for (std::vector<IniSection*>::iterator it = m_Sections.begin(); it != m_Sections.end(); it++)
		{
			(*it)->Write(fileStream);
		}


		fileStream.close();

	}

	/// <summary>
	/// Logs out to the console as if it were writing to the file.
	/// This is used for debugging purposes only.
	/// </summary>
    void IniFileStream::LogConsole()
    {
        for (std::vector<IniSection*>::iterator it = m_Sections.begin(); it != m_Sections.end(); it++)
        {
            (*it)->LogConsole();
        }
    }

	/// <summary>
	/// Clears all of the sections and their variables from the filestream.
	/// </summary>
    void IniFileStream::Clear()
    {
        for (int i = m_Sections.size() - 1; i >= 0; i--)
        {
            delete m_Sections[i];
            m_Sections[i] = nullptr;
        }
        m_Sections.clear();
    }

	/// <summary>
	/// Adds a section to the filestream. Sections are used for holding variables. Variables names are unique to their own section.
	/// </summary>
	/// <param name="aSectionName">The name of the section.</param>
	/// <returns> Returns false if a section with the name already exists. </returns>
	bool IniFileStream::AddSection(const std::string & aSectionName)
	{
		for (std::vector<IniSection*>::iterator it = m_Sections.begin(); it != m_Sections.end(); it++)
		{
			if ((*it)->GetName() == aSectionName)
			{
				return false;
			}
		}
		IniSection * section = new IniSection();
		section->SetName(aSectionName);
		m_Sections.push_back(section);
		return true;
	}

	/// <summary>
	/// Removes a section by name.
	/// </summary>
	/// <param name="aSectionName">The name of the section.</param>
	/// <returns> Returns false if a section with the name does not exist. </returns>
	bool IniFileStream::RemoveSection(const std::string & aSectionName)
	{
		for (std::vector<IniSection*>::iterator it = m_Sections.begin(); it != m_Sections.end(); it++)
		{
			if ((*it)->GetName() == aSectionName)
			{
				m_Sections.erase(it);
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Binds a section using Add<Type>, Remove<Type>, and Get<Type> methods.
	/// With no section bound the Get<Type> methods will return INI_BAD_VARIABLE_NAME variables.
	/// If the section is not found the bound section becomes null.
	/// </summary>
	/// <param name="aSectionName">The name of the section.</param>
	/// <returns> Returns false if a section with the name does not exist. </returns>
	bool IniFileStream::BindSection(const std::string & aSectionName)
	{
		for (std::vector<IniSection*>::iterator it = m_Sections.begin(); it != m_Sections.end(); it++)
		{
			if ((*it)->GetName() == aSectionName)
			{
				m_BoundSection = *it;
				return true;
			}
		}
		m_BoundSection = nullptr;
		return false;
	}

	/// <summary>
	/// Retrieves the names of the sections.
	/// </summary>
	/// <param name="aSectionNames">The name of the section.</param>
	void IniFileStream::GetSectionNames(std::vector<std::string> & aSectionNames)
	{
		for (std::vector<IniSection*>::iterator it = m_Sections.begin(); it != m_Sections.end(); it++)
		{
			aSectionNames.push_back((*it)->GetName());
		}
	}

	// -- Creates a new variable with the name and value. 
	void IniFileStream::AddBool(const std::string & aVariableName, bool aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->AddBool(aVariableName, aValue);
		}
	}
	void IniFileStream::AddInt(const std::string & aVariableName, int aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->AddInt(aVariableName, aValue);
		}
	}
	void IniFileStream::AddFloat(const std::string & aVariableName, float aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->AddFloat(aVariableName, aValue);
		}
	}
	void IniFileStream::AddString(const std::string & aVariableName, std::string aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->AddString(aVariableName, aValue);
		}
	}
	void IniFileStream::AddVector3(const std::string & aVariableName, Vector3 aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->AddVector3(aVariableName, aValue);
		}
	}
	void IniFileStream::AddVector4(const std::string & aVariableName, Vector4 aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->AddVector4(aVariableName, aValue);
		}
	}

	// -- Searches for a variable by name in the currently bound section
	IniBool IniFileStream::GetBool(const std::string & aVariableName)
	{
		return m_BoundSection != nullptr ? m_BoundSection->GetBool(aVariableName) : IniBool(INI_BAD_VARIABLE_NAME);
	}
	IniInt IniFileStream::GetInt(const std::string & aVariableName)
	{
        return m_BoundSection != nullptr ? m_BoundSection->GetInt(aVariableName) : IniInt(INI_BAD_VARIABLE_NAME);
	}
	IniFloat IniFileStream::GetFloat(const std::string & aVariableName)
	{
        return m_BoundSection != nullptr ? m_BoundSection->GetFloat(aVariableName) : IniFloat(INI_BAD_VARIABLE_NAME);
	}
	IniString IniFileStream::GetString(const std::string & aVariableName)
	{
        return m_BoundSection != nullptr ? m_BoundSection->GetString(aVariableName) : IniString(INI_BAD_VARIABLE_NAME);
	}
	IniVector3 IniFileStream::GetVector3(const std::string & aVariableName)
	{
		return m_BoundSection != nullptr ? m_BoundSection->GetVector3(aVariableName) : IniVector3(INI_BAD_VARIABLE_NAME);
	}

	IniVector4 IniFileStream::GetVector4(const std::string & aVariableName)
	{
		return m_BoundSection != nullptr ? m_BoundSection->GetVector4(aVariableName) : IniVector4(INI_BAD_VARIABLE_NAME);
	}

	// -- Creates a new variable with the name and value. 
	void IniFileStream::SetBool(const std::string & aVariableName, bool aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->SetBool(aVariableName, aValue);
		}
	}
	void IniFileStream::SetInt(const std::string & aVariableName, int aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->SetInt(aVariableName, aValue);
		}
	}
	void IniFileStream::SetFloat(const std::string & aVariableName, float aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->SetFloat(aVariableName, aValue);
		}
	}
	void IniFileStream::SetString(const std::string & aVariableName, std::string aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->SetString(aVariableName, aValue);
		}
	}
	void IniFileStream::SetVector3(const std::string & aVariableName, Vector3 aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->SetVector3(aVariableName, aValue);
		}
	}
	void IniFileStream::SetVector4(const std::string & aVariableName, Vector4 aValue)
	{
		if (m_BoundSection != nullptr)
		{
			m_BoundSection->SetVector4(aVariableName, aValue);
		}
	}

	bool IniFileStream::RemoveVariable(const std::string & aVariableName)
	{
		return m_BoundSection != nullptr ? m_BoundSection->RemoveVariable(aVariableName) : false;
	}
	bool IniFileStream::VariableExists(const std::string & aVariableName)
	{
		return m_BoundSection != nullptr ? m_BoundSection->VariableExists(aVariableName) : false;
	}

	std::string IniFileStream::GetPath()
	{
		return m_Path;
	}
	void IniFileStream::SetPath(const std::string & aPath)
	{
		m_Path = aPath;
	}
	void IniFileStream::SetPath(const char * aPath)
	{
		m_Path = aPath;
	}
}