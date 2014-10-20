#include "G_File.h"
#include "../../Reflection/G_Reflection.h"
#include <boost\filesystem.hpp>
#include <cstdio>

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

		bool File::open(FileMode aFileMode)
		{
			FILE * file = nullptr;
			string fileMode = string::empty();
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
				read(file);
				break;
			case FileMode::WRITE:
				write(file);
				break;
			case FileMode::READ_WRITE:
				readWrite(file);
				break;
			case FileMode::OVERWRITE:
				readWrite(file);
				break;
			case FileMode::APPEND:
				readWrite(file);
				break;
			default:
				break;
			}
			//Close the file when done and check the status.
			int closeStatus = fclose(file);
			return closeStatus == 0;
		}
		bool File::open(string & aPath, FileMode aFileMode)
		{
			m_Path = aPath;
			return open(aFileMode);
		}
		bool File::open(const char * aPath, FileMode aFileMode)
		{
			m_Path = aPath;
			return open(aFileMode);
		}
		void File::close()
		{
			m_ReadStream.ref()->set("");
			m_WriteStream.ref()->set("");
		}
		bool File::createFile(string & aPath, string & aFilename, string & aExtension)
		{
			string path = aPath + aFilename + aExtension;
			if (!boost::filesystem::exists(aPath.c_str()) || boost::filesystem::exists(path.c_str()))
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
		bool File::exists(string & aFilename)
		{
			return boost::filesystem::exists(aFilename.c_str());
		}
		bool File::exists(string & aFilename, string & aExtension)
		{
			return boost::filesystem::exists(string(aFilename + aExtension).c_str());
		}
		bool File::exists(File * aFile)
		{
			if (aFile == nullptr)
			{
				return false;
			}
			return exists(aFile->path());
		}
		string File::extension(string & aFilename)
		{
			boost::filesystem::path path(aFilename.c_str());
			if (!path.has_extension())
			{
				return string::empty();
			}
			return string(path.extension().string().c_str());
		}
		string File::extension(File * aFile)
		{
			if (aFile == nullptr)
			{
				return string::empty();
			}
			return extension(aFile->path());
		}

		string replaceExtension(string aFilename, string aNewExtension)
		{
			boost::filesystem::path path(aFilename.c_str());
			return path.replace_extension(aNewExtension.c_str()).string().c_str();
		}

		string replaceExtension(File * aFile, string aNewExtension)
		{
			if (aFile == nullptr)
			{
				return string::empty();
			}
			return replaceExtension(aFile->path(),aNewExtension);
		}

		string File::readStream()
		{
			return m_ReadStream.data();
		}
		string File::writeStream()
		{
			return m_WriteStream.data();
		}
		void File::setWriteStream(string & aData)
		{
			m_WriteStream.ref()->set(aData);
			m_WriteStream.ref()->append("\n");
		}
		void File::addWriteStream(string & aData)
		{
			m_WriteStream.ref()->append(aData);
			m_WriteStream.ref()->append("\n");
		}
		void File::clearWriteStream()
		{
			m_WriteStream.ref()->set("");
		}
		string File::path()
		{
			return m_Path;
		}
		boolean File::verify()
		{
			return boost::filesystem::exists(m_Path.c_str()) && boost::filesystem::is_regular_file(m_Path.c_str());
		}
		Pointer<Reflection::Type> File::getType()
		{
			return typeOf("File");
		}

		void File::read(FILE * aFile)
		{
			if (aFile == nullptr)
			{
				return;
			}
			//Clear Read Stream
			m_ReadStream.ref()->set("");
			char buffer[100];
			//Read into buffer then append read stream
			while (fgets(buffer, 100, aFile) != nullptr)
			{
				m_ReadStream.ref()->append(buffer);
			}
		}
		void File::write(FILE * aFile)
		{
			if (aFile == nullptr || m_WriteStream.ref()->length() == 0)
			{
				return;
			}
			int status = fputs(m_WriteStream.ref()->c_str(), aFile);
			if (status == EOF)
			{
				//Error, End of File?
			}
		}
		void File::readWrite(FILE * aFile)
		{
			if (aFile == nullptr)
			{
				return;
			}
			read(aFile);
			write(aFile);
		}
	}
}