#ifndef G_IO_H
#define G_IO_H

#include <boost\filesystem.hpp>


namespace Gem
{
    namespace FileIO
    {
        class File
        {
        public:
            File():m_Path(""){}
            File(std::string aPath):m_Path(aPath){}
            ~File(){}
            //static std::vector<File> toFiles(std::vector<std::string> aFileList);


            inline std::string filename()
            {
                return m_Path;
            }
            bool verify();
        private:
            std::string m_Path;
        };
        //
        class Directory
        {
        public:
            Directory():m_Path(""){}
            Directory(std::string aPath):m_Path(aPath){}
            ~Directory(){}
    
            std::vector<std::string> getFiles();
            std::vector<std::string> getDirectories();
    
            inline std::string directoryName()
            {
                return m_Path;
            }
            bool verify();
            //static std::vector<Directory> toDirectories(std::vector<std::string> aDirectoryList);
    
        private:
            std::string m_Path;
        };

        class IO
        {
        public:
            inline static Directory projectDirectory()
            {
                std::string directory = "..\\..\\";
                if(exists(directory))
                {
                    if(isDirectory(directory))
                    {
                        return Directory(directory);
                    }
                }
                return Directory();
            }
            static bool createFolder(std::string & aPath);
            inline static bool isDirectory(std::string aPath)
            {
                return boost::filesystem::is_directory(aPath);
            }
            inline static bool isFile(std::string aPath)
            {
                return boost::filesystem::is_regular_file(aPath);
            }
            inline static bool exists(std::string aPath)
            {
                return boost::filesystem::exists(aPath);
            }

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
            inline static File sceneMasterList(bool aCreate = false)
            {
                std::string file = "..\\..\\ProjectSettings\\SceneMasterList.xml";
                if(exists(file))
                {
                    if(isFile(file))
                    {
                        return File(file);
                    }
                }
                else
                {
                    if(aCreate == true && exists("..\\..\\ProjectSettings\\"))
                    {
                        std::ofstream file(file);
                        file.close();
                        return sceneMasterList(false);
                    }
                }
                return File();
            }
        };
    }
}

#endif