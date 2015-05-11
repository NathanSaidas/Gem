#include "G_Directory.h"
#include "../../Reflection/G_Reflection.h"
//Include Debug
#ifdef GEM_DEBUG
#include "../G_Debug.h"
#endif

#ifdef __APPLE__ && __MACH__

#elif _WIN32
#include <io.h> //for _access
#include <Windows.h> 
#elif __linux__
#include <unistd.h> //for access
#define _access(const char * path,int mode) access(const char * path, int mode)
#endif
//#include <sys\types.h> //for stat
#include <sys\stat.h> //for stat

namespace Gem
{
    namespace FileIO
    {
		G_CLASS_IMPLEMENTATION(Directory,object)
			bool Directory::exists(std::string & aPath)
        {
            if (aPath.size() == 0)
            {
                return false;
            }
#if _WIN32
            DWORD directory = GetFileAttributes(aPath.c_str());
            if (directory == INVALID_FILE_ATTRIBUTES)
            {
                return false;
            }
            if (directory == FILE_ATTRIBUTE_DIRECTORY)
            {
                return true;
            }
            return false;
#elif __linux__
            if (_access(aPath.c_str(), (int)FileAccess::EXISTENCE))
            {
                struct stat status;
                stat(aPath.c_str(), &status);
                return S_ISDIR(status.st_mode);
            }
            return false;
#endif
        }
		Directory Directory::create(std::string & aPath)
        {
#if _WIN32
			return Directory();
#elif
			
#endif
        }

        Directory::Directory() 
            :m_Path("")
        {

        }
		Directory::Directory(std::string aPath)
            :m_Path(aPath)
        {

        }
        Directory::~Directory()
        {

        }

		std::vector<std::string> Directory::getFiles()
        {
#ifdef _WIN32
            HANDLE fileHandle = INVALID_HANDLE_VALUE;
            WIN32_FIND_DATA fileData;
			std::vector<std::string> files;

            if (m_Path.size() == 0)
            {
                return files;
            }

            fileHandle = FindFirstFile(m_Path.c_str(), &fileData);
            if (fileHandle == INVALID_HANDLE_VALUE)
            {
                return files;
            }

            do
            {
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    //Directory
                }
                else
                {
                    files.push_back(fileData.cFileName);
                }
            } while ((FindNextFile(fileHandle, &fileData)) != 0);
            FindClose(fileHandle);
            return files;
#elif __linux__

#endif
        }
		std::vector<std::string> Directory::getDirectories()
        {
#ifdef _WIN32
            HANDLE fileHandle = INVALID_HANDLE_VALUE;
            WIN32_FIND_DATA fileData;
			std::vector<std::string> directories;

            if (m_Path.size() == 0)
            {
                return directories;
            }

            fileHandle = FindFirstFile(m_Path.c_str(), &fileData);
            if (fileHandle == INVALID_HANDLE_VALUE)
            {
                return directories;
            }

            do
            {
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    //Directory
                    directories.push_back(fileData.cFileName);
                }
                else
                {

                }
            } while ((FindNextFile(fileHandle, &fileData)) != 0);
            FindClose(fileHandle);
            return directories;
#elif __linux__

#endif
        }
        bool Directory::verify()
        {
            return exists(m_Path);
        }
    }
}