#ifndef G_IO_H
#define G_IO_H

#include <boost\filesystem.hpp>
#include "../Primitives/G_PrimitiveObjects.h"
#include "FileIO\G_File.h"


namespace Gem
{
    namespace FileIO
    {
        
        //
        class Directory : public Object
        {
        public:
            Directory():m_Path(""){}
            Directory(string aPath):m_Path(aPath){}
            ~Directory(){}
    
            std::vector<string> getFiles();
            std::vector<string> getDirectories();
    
            inline string directoryName()
            {
                return m_Path;
            }
			inline string path()
			{
				return string(m_Path);
			}
			///Returns true if the directory is a directory
            bool verify();
            //static std::vector<Directory> toDirectories(std::vector<std::string> aDirectoryList);
            virtual Pointer<Reflection::Type> getType();
        private:
            string m_Path;
        };

        class IO : public Object
        {
        public:
            inline static Directory projectDirectory()
            {
                string directory = "..\\..\\";
                if(exists(directory))
                {
                    if(isDirectory(directory))
                    {
                        return Directory(directory);
                    }
                }
                return Directory();
            }
            static bool createFolder(string & aPath);
            inline static bool isDirectory(string aPath)
            {
                return boost::filesystem::is_directory(aPath.c_str());
            }
            inline static bool isFile(string aPath)
            {
                return boost::filesystem::is_regular_file(aPath.c_str());
            }
            inline static bool exists(string aPath)
            {
                return boost::filesystem::exists(aPath.c_str());
            }

			//Where all assets belong. (Texture,Audio,Etc)
            inline static Directory assetsDirectory()
            {
                std::string directory = "..\\..\\Assets";
                if(exists(directory))
                {
                    if(isDirectory(directory))
                    {
                        return Directory(directory);
                    }
                }
                return Directory();
            }
			inline static Directory logFileDirectory()
			{
				std::string directory = "..\\..\\Assets\\Logs";
				if (exists(directory))
				{
					if (isDirectory(directory))
					{
						return Directory(directory);
					}
				}
				return Directory();
			}
            inline static Directory projectSettings()
            {
                std::string directory = "..\\..\\ProjectSettings";
                if(exists(directory))
                {
                    if(isDirectory(directory))
                    {
                        return Directory(directory);
                    }
                }
                return Directory();
            }

            virtual Pointer<Reflection::Type> getType();
        };
    }
}

GEM_CLASS(FileIO::Directory,Object)
GEM_CLASS(FileIO::IO,Object)

#endif