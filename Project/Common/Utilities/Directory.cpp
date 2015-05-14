#include "Directory.h"
#ifdef _WIN32
#include <Windows.h>
#include <Shlwapi.h>
#else
#endif

namespace Gem
{
	RDEFINE_CLASS(Directory,object)

	Directory::Directory()
	{
		m_Path = "";
	}
	Directory::Directory(std::string aPath)
	{
		m_Path = aPath;
	}
	Directory::~Directory()
	{
	}



	void Directory::Back()
	{
#ifdef _WIN32
		for (int i = m_Path.length(); i >= 0; i--)
		{
			if (m_Path[i] == '\\' || m_Path[i] == '/')
			{
				m_Path = m_Path.substr(0, i + 1);
				return;
			}
		}
#else

#endif
	}

	void Directory::Back(int aCount)
	{
#ifdef _WIN32
		int hitCount = 0;
		for (int i = m_Path.length(); i >= 0; i--)
		{
			if (m_Path[i] == '\\' || m_Path[i] == '/')
			{
				hitCount++;
				if (hitCount > aCount)
				{
					m_Path = m_Path.substr(0, i + 1);
					return;
				}
			}
		}
		m_Path = "";
#else

#endif
	}

	bool Directory::Change(const std::string & aPath)
	{
		std::string path = m_Path;
		path.append(aPath);
		if (Exists(Directory(path)))
		{
			m_Path = path;
			return true;
		}
		return false;
	}

	std::string Directory::GetPath()
	{
		return m_Path;
	}
	std::string Directory::GetPath() const
	{
		return m_Path;
	}

	void Directory::SetPath(std::string aPath)
	{
		m_Path = aPath;
	}

	Directory Directory::GetCurrent()
	{
		std::string directoryPath = "";
#ifdef _WIN32
		TCHAR buffer[MAX_DIRECTORY_LENGTH];
		DWORD bufferLength = 0;

		bufferLength = GetCurrentDirectory(MAX_DIRECTORY_LENGTH, buffer);
		directoryPath = buffer;
		directoryPath.append("\\");
		if (bufferLength == 0)
		{
			//TODO(Nathan): Error Log.
		}
#else

#endif
		return Directory(directoryPath);
	}

	bool Directory::Exists(const Directory & aDirectory)
	{
#ifdef _WIN32
		DWORD fileAttribs = GetFileAttributes(aDirectory.GetPath().c_str());
		return (fileAttribs != INVALID_FILE_ATTRIBUTES && fileAttribs & FILE_ATTRIBUTE_DIRECTORY);
#else

#endif

	}

	bool Directory::FileExists(const std::string & aFilename)
	{
#ifdef _WIN32
		return PathFileExists(aFilename.c_str());
#else

#endif
	}


}