#ifndef G_DIRECTORY_H
#define G_DIRECTORY_H

#include "G_FileMode.h" //For FileMode enum
#include "G_FileAccess.h" //Fore FileAccess enum
#include "../../G_Object.h"

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
        class Directory : public object
        {
			G_CLASS_DEF(Directory)
            public:
                static bool exists(std::string & aPath);
				static Directory create(std::string & aPath);
                Directory();
				Directory(std::string aPath);
                ~Directory();

				std::vector<std::string> getFiles();
				std::vector<std::string> getDirectories();

				inline std::string directoryName()
                {
                    return m_Path;
                }
				inline std::string path()
                {
					return std::string(m_Path);
                }
                ///Returns true if the directory is a directory
                bool verify();
                //static std::vector<Directory> toDirectories(std::vector<std::string> aDirectoryList);
            private:
				std::string m_Path;
        };
    }
}


#endif