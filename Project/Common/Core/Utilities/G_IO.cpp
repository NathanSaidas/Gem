#include "G_IO.h"
#include "../Reflection/G_Reflection.h"



namespace Gem
{
    namespace FileIO
    {
        std::vector<string> Directory::getFiles()
        {
            std::vector<string> filenames;

            if(IO::exists(m_Path))
            {
                if(IO::isDirectory(m_Path))
                {
                    for(boost::filesystem::directory_iterator iter(m_Path.c_str()); iter != boost::filesystem::directory_iterator(); ++iter)
                    {
                        if(IO::isFile(iter->path().string().c_str()))
                        {
							
							filenames.push_back(iter->path().filename().string().c_str());
                        }
                    }
                }
            }

            return filenames;
        }
        std::vector<string> Directory::getDirectories()
        {
            std::vector<string> directoryNames;

            if(IO::exists(m_Path))
            {
                if(IO::isDirectory(m_Path))
                {
                    for(boost::filesystem::directory_iterator iter(m_Path.c_str()); iter != boost::filesystem::directory_iterator(); ++iter)
                    {
                        if(IO::isDirectory(iter->path().string().c_str()))
                        {
                            directoryNames.push_back(iter->path().string().c_str());
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
            
        

        bool IO::createFolder(string & aPath)
        {
            boost::filesystem::path path(aPath.c_str());
            if(path.has_extension())
            {
                path.replace_extension("");
				aPath = path.string().c_str();
            }
            return boost::filesystem::create_directory(aPath.c_str());
        }
        Pointer<Reflection::Type> IO::getType()
        {
            return typeOf("IO");
        }
    }
}