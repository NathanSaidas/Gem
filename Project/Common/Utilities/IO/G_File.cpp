#include "G_File.h"
#include <cstdio>

#ifdef __APPLE__ && __MACH__

#elif _WIN32
#include <io.h>
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#define _access(const char * path,int mode) access(const char * path, int mode)
#endif
#include <sys\stat.h>

//#include "../G_Debug.h"
#include "../../Reflection/G_Reflection.h"
//#include <boost\filesystem.hpp>

#include "G_Directory.h"


/// <summary>
/// 
/// </summary>
namespace Gem
{
	namespace FileIO
	{
		G_CLASS_IMPLEMENTATION(File,object)
		File::File()
		{
		
		}
		File::File(std::string & aPath)
		{

		}
		File::File(const char * aPath)
		{

		}
		File::~File()
		{

		}

		bool File::Open(FileMode aFileMode)
		{
			FILE * file = nullptr;
			std::string fileMode = "";
			switch (aFileMode)
			{
			case FileMode::READ:
				fileMode = "r";
				break;
			case FileMode::WRITE:
				fileMode = "w";
				break;
			case FileMode::READ_WRITE:
				fileMode = "r+";
				break;
			case FileMode::APPEND:
				fileMode = "a+";
				break;
			case FileMode::OVERWRITE:
				fileMode = "w+";
				break;
			default:
				return false;
			}
			//Try and open the file
			file = fopen(m_Path.c_str(), fileMode.c_str());
			if (file == nullptr)
			{
				return false;
			}
			switch (aFileMode)
			{
			case FileMode::READ:
				Read(file);
				break;
			case FileMode::WRITE:
				Write(file);
				break;
			case FileMode::READ_WRITE:
				ReadWrite(file);
				break;
			case FileMode::OVERWRITE:
				ReadWrite(file);
				break;
			case FileMode::APPEND:
				ReadWrite(file);
				break;
			default:
				break;
			}
			//Close the file when done and check the status.
			int closeStatus = fclose(file);
			return closeStatus == 0;
		}
		bool File::Open(std::string & aPath, FileMode aFileMode)
		{
			m_Path = aPath;
			return Open(aFileMode);
		}
		bool File::Open(const char * aPath, FileMode aFileMode)
		{
			m_Path = aPath;
			return Open(aFileMode);
		}
		void File::Close()
		{
			m_ReadStream = "";
			m_WriteStream = "";
		}
		bool File::Create(std::string & aPath, std::string & aFilename, std::string & aExtension)
		{
			std::string path = aPath + aFilename + aExtension;
            struct stat status;
            stat(aPath.c_str(), &status);
			if (!Directory::exists(aPath) || Exists(path))
			{
				return false;
			}
			FILE * file = fopen(path.c_str(), "w");
			if (file != nullptr)
			{
				return fclose(file) == 0;
			}
			return false;
		}
        
		bool File::Exists(std::string & aFilename)
		{
#if _WIN32
            DWORD file = GetFileAttributes(aFilename.c_str());
            if(file == INVALID_FILE_ATTRIBUTES)
            {
                return false;
            }
            if(file != FILE_ATTRIBUTE_DIRECTORY)
            {
                return true;
            }
            return false;
#elif __linux__
            int result = _access(aFilename.c_str(), (int)FileAccess::EXISTENCE);
            if (result == ENOENT)
            {
                Debug::log(aFilename + " file was not found.");
            }
            return result == 0;
#endif 
		}
		bool File::Exists(std::string & aFilename, std::string & aExtension)
		{
			return Exists(std::string(aFilename + aExtension));
		}
		bool File::Exists(File * aFile)
		{
			if (aFile == nullptr)
			{
				return false;
			}
			return Exists(aFile->Path());
		}
		std::string File::Extension(std::string & aFilename)
		{
			return "";
		}
		std::string File::Extension(File * aFile)
		{
			return "";
		}

		std::string ReplaceExtension(std::string aFilename, std::string aNewExtension)
		{
			return "";
		}

		std::string ReplaceExtension(File * aFile, std::string aNewExtension)
		{
			if (aFile == nullptr)
			{
				return "";
			}
			return ReplaceExtension(aFile->Path(),aNewExtension);
		}

		std::string File::ReadStream()
		{
			return m_ReadStream.data();
		}
		std::string File::WriteStream()
		{
			return m_WriteStream.data();
		}
		void File::SetWriteStream(std::string & aData)
		{
			m_WriteStream = aData + "\n";
		}
		void File::AddWriteStream(std::string & aData)
		{
			m_WriteStream = aData + "\n";
		}
		void File::ClearWriteStream()
		{
			m_WriteStream = "";
		}
		std::string File::Path()
		{
			return m_Path;
		}
		bool File::Verify()
		{
			return false;
		}
		void File::Read(FILE * aFile)
		{
			if (aFile == nullptr)
			{
				return;
			}
			//Clear Read Stream
			m_ReadStream = "";
			char buffer[100];
			//Read into buffer then append read stream
			while (fgets(buffer, 100, aFile) != nullptr)
			{
				m_ReadStream.append(buffer);
			}
		}
		void File::Write(FILE * aFile)
		{
			if (aFile == nullptr || m_WriteStream.size() == 0)
			{
				return;
			}
			int status = fputs(m_WriteStream.c_str(), aFile);
			if (status == EOF)
			{
				//Error, End of File?
			}
		}
		void File::ReadWrite(FILE * aFile)
		{
			if (aFile == nullptr)
			{
				return;
			}
			Read(aFile);
			Write(aFile);
		}
	}
}