#include "G_Debug.h"
#include "../Reflection/G_Reflection.h"
#include "FileIO\G_File.h"
#include "G_IO.h"
#include "G_Utilities.h"

namespace Gem
{
	using namespace FileIO;

	Debug * Debug::s_Instance = nullptr;
	Debug * Debug::instance()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = (Debug*)Memory::instantiate("Debug");
		}
		return s_Instance;
	}
	void Debug::destroy()
	{
		if (s_Instance != nullptr)
		{
			s_Instance = (Debug*)Memory::instantiate("Debug");
		}
	}

	Debug::Debug()
	{
		m_File = Memory::instantiate<File>();
	}
	Debug::~Debug()
	{
		m_File = Memory::destroy<File>(m_File);
	}


	void Debug::log(string aMessage, bool aWriteFile)
	{
		instance()->debugLog(aMessage, aWriteFile);
	}
	void Debug::log(Object aMessage, bool aWriteFile)
	{
		instance()->debugLog(aMessage.toString(), aWriteFile);
	}
	void Debug::log(Object * aMessage, bool aWriteFile)
	{
		if (aMessage != nullptr)
		{
			instance()->debugLog(aMessage->toString(), aWriteFile);
		}
	}
	void Debug::warning(string aMessage, bool aWriteFile)
	{
		instance()->debugWarning(aMessage, aWriteFile);
	}
	void Debug::warning(Object aMessage, bool aWriteFile)
	{
		instance()->debugWarning(aMessage.toString(), aWriteFile);
	}
	void Debug::warning(Object * aMessage, bool aWriteFile)
	{
		if (aMessage != nullptr)
		{
			instance()->debugWarning(aMessage->toString(), aWriteFile);
		}
	}
	void Debug::error(string aMessage, bool aWriteFile)
	{
		instance()->debugError(aMessage, aWriteFile);
	}
	void Debug::error(Object aMessage, bool aWriteFile)
	{
		instance()->debugError(aMessage.toString(), aWriteFile);
	}
	void Debug::error(Object * aMessage, bool aWriteFile)
	{
		if (aMessage != nullptr)
		{
			instance()->debugError(aMessage->toString(), aWriteFile);
		}
	}
	void Debug::saveLogFile()
	{
		instance()->debugSaveLogFile();
	}
	void Debug::clearLogFile()
	{
		instance()->debugClearLogFile();
	}

	void Debug::debugLog(string aMessage, bool aWriteFile)
	{
		string message = string("[Log]:") + aMessage;
		printf("%s \n",message.c_str());
		if (aWriteFile == true && m_File != nullptr)
		{
			m_File->addWriteStream(message);
		}
	}
	void Debug::debugWarning(string aMessage, bool aWriteFile)
	{
		string message = string("[Warning]:") + aMessage;
		printf("%s \n", message.c_str());
		if (aWriteFile == true && m_File != nullptr)
		{
			m_File->addWriteStream(message);
		}
	}
	void Debug::debugError(string aMessage, bool aWriteFile)
	{
		string message = string("[Error]:") + aMessage;
		printf("%s \n", message.c_str());
		if (aWriteFile == true && m_File != nullptr)
		{
			m_File->addWriteStream(message);
		}
	}
	void Debug::debugSaveLogFile()
	{
		if (m_File != nullptr)
		{
			Directory logsDirectory = IO::logFileDirectory();
			if (logsDirectory.verify() == false)
			{
				return;
			}
			string path = logsDirectory.path();
			std::vector<string> filenames = logsDirectory.getFiles();
			std::vector<int> logFiles;
			string currentFile = string::empty();

			//Search
			for (int i = 0; i < filenames.size(); i++)
			{
				filenames[i].subString(currentFile, 4);

				if (currentFile == string("Log_"))
				{
					string indexString;
					int index;
					filenames[i].subString(indexString, 4, filenames[i].length() - 4);
					index = S2I(indexString);
					logFiles.push_back(index);
				}
			}
			if (logFiles.size() == 0)
			{
				string filename = "\\Log_01";
				path.append(filename);
				path.append(".txt");
				m_File->open(path, FileMode::WRITE);
			}
			else
			{
				string filename = "\\Log_";
				int index = logFiles.at(logFiles.size() - 1) + 1;
				if (index < 10)
				{
					filename.append("0");
				}
				filename.append(I2S(index).c_str());
				path.append(filename);
				path.append(".txt");
				m_File->open(path, FileMode::WRITE);
			}

			m_File->clearWriteStream();
		}
	}
	void Debug::debugClearLogFile()
	{
		if (m_File != nullptr)
		{
			m_File->clearWriteStream();
		}
	}


	Pointer<Reflection::Type> Debug::getType()
	{
		return typeOf("Debug");
	}
}