#ifndef G_DIRECTORY_H
#define G_DIRECTORY_H

#include "../../Primitives/G_PrimitiveObjects.h" //For Object / Type
#include "G_FileMode.h" //For FileMode enum
#include "G_FileAccess.h" //Fore FileAccess enum

#include <vector>

namespace Gem
{
    namespace FileIO
    {
#pragma region Change Log
        /* October,20,2014 - Nathan Hanlan - Moved File to its own File.
        *  October,20,2014 - Nathan Hanlan - Removing support for std::string. Adding support for Gem::string and const char *
        *  October,20,2014 - Nathan Hanlan - Removing Boost Require Support. Using Windows / unix systems.
        */
#pragma endregion ChangeLog
        /// <summary>
        /// A data structure used to handle directory operations. Create, Exists etc.
        /// </summary>
        class Directory : public Object
        {
            public:
                static bool exists(string & aPath);
                static Directory create(string & aPath);
                Directory();
                Directory(string aPath);
                ~Directory();

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
                virtual Pointer<Reflection::Type> GetType();




            private:
                string m_Path;
        };
    }
}
GEM_CLASS(FileIO::Directory, Object)


#endif