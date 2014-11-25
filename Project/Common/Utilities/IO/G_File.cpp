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
		File::File()
		{
		
		}
		File::File(string & aPath)
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
			string fileMode = string::Empty();
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
			file = fopen(m_Path.C_Str(), fileMode.C_Str());
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
		bool File::Open(string & aPath, FileMode aFileMode)
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
			m_ReadStream.ref()->Set("");
			m_WriteStream.ref()->Set("");
		}
		bool File::Create(string & aPath, string & aFilename, string & aExtension)
		{
			string path = aPath + aFilename + aExtension;
            struct stat status;
            stat(aPath.C_Str(), &status);
			if (!Directory::exists(aPath) || Exists(path))
			{
				return false;
			}
			FILE * file = fopen(path.C_Str(), "w");
			if (file != nullptr)
			{
				return fclose(file) == 0;
			}
			return false;
		}
        
		bool File::Exists(string & aFilename)
		{
#if _WIN32
            DWORD file = GetFileAttributes(aFilename.C_Str());
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
		bool File::Exists(string & aFilename, string & aExtension)
		{
            return Exists(string(aFilename + aExtension));
		}
		bool File::Exists(File * aFile)
		{
			if (aFile == nullptr)
			{
				return false;
			}
			return Exists(aFile->Path());
		}
		string File::Extension(string & aFilename)
		{
			return string::Empty();
		}
		string File::Extension(File * aFile)
		{
			return string::Empty();
		}

		string ReplaceExtension(string aFilename, string aNewExtension)
		{
			return string::Empty();
		}

		string ReplaceExtension(File * aFile, string aNewExtension)
		{
			if (aFile == nullptr)
			{
				return string::Empty();
			}
			return ReplaceExtension(aFile->Path(),aNewExtension);
		}

		string File::ReadStream()
		{
			return m_ReadStream.data();
		}
		string File::WriteStream()
		{
			return m_WriteStream.data();
		}
		void File::SetWriteStream(string & aData)
		{
			m_WriteStream.ref()->Set(aData);
			m_WriteStream.ref()->Append("\n");
		}
		void File::AddWriteStream(string & aData)
		{
			m_WriteStream.ref()->Append(aData);
			m_WriteStream.ref()->Append("\n");
		}
		void File::ClearWriteStream()
		{
			m_WriteStream.ref()->Set("");
		}
		string File::Path()
		{
			return m_Path;
		}
		boolean File::Verify()
		{
			return false;
		}
		Pointer<Reflection::Type> File::GetType()
		{
			return typeOf("File");
		}

		void File::Read(FILE * aFile)
		{
			if (aFile == nullptr)
			{
				return;
			}
			//Clear Read Stream
			m_ReadStream.ref()->Set("");
			char buffer[100];
			//Read into buffer then append read stream
			while (fgets(buffer, 100, aFile) != nullptr)
			{
				m_ReadStream.ref()->Append(buffer);
			}
		}
		void File::Write(FILE * aFile)
		{
			if (aFile == nullptr || m_WriteStream.ref()->Length() == 0)
			{
				return;
			}
			int status = fputs(m_WriteStream.ref()->C_Str(), aFile);
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