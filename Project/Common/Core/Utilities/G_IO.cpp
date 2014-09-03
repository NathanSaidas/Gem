#include "G_IO.h"
#include "../Reflection/G_Reflection.h"



namespace Gem
{
    namespace FileIO
    {
        std::vector<std::string> Directory::getFiles()
        {
            std::vector<std::string> filenames;

            if(IO::exists(m_Path))
            {
                if(IO::isDirectory(m_Path))
                {
                    for(boost::filesystem::directory_iterator iter(m_Path); iter != boost::filesystem::directory_iterator(); ++iter)
                    {
                        if(IO::isFile(iter->path().string()))
                        {
                            filenames.push_back(iter->path().string());
                        }
                    }
                }
            }

            return filenames;
        }
        std::vector<std::string> Directory::getDirectories()
        {
            std::vector<std::string> directoryNames;

            if(IO::exists(m_Path))
            {
                if(IO::isDirectory(m_Path))
                {
                    for(boost::filesystem::directory_iterator iter(m_Path); iter != boost::filesystem::directory_iterator(); ++iter)
                    {
                        if(IO::isDirectory(iter->path().string()))
                        {
                            directoryNames.push_back(iter->path().string());
                        }
                    }
                }
            }
            return directoryNames;
        }
        bool Directory::verify()
        {
            return IO::isDirectory(m_Path);
        }
        Pointer<Reflection::Type> Directory::getType()
        {
            return typeOf("Directory");
        }
            
        bool File::verify()
        {
            return IO::isFile(m_Path);
        }
        Pointer<Reflection::Type> File::getType()
        {
            return typeOf("File");
        }

        bool IO::createFolder(std::string & aPath)
        {
            boost::filesystem::path path(aPath);
            if(path.has_extension())
            {
                path.replace_extension("");
                aPath = path.string();
            }
            return boost::filesystem::create_directory(aPath);
        }
        Pointer<Reflection::Type> Directory::getType()
        {
            return typeOf("IO");
        }
    }
}