#ifndef G_FILE_MODE_H
#define G_FILE_MODE_H

namespace Gem
{
    namespace FileIO
    {
#pragma region Change Log
        /* October,20,2014 - Nathan Hanlan - Added enums for file read/write modes
        *  October,20,2014 - Nathan Hanlan - Created separate file for FileMode
        */
#pragma endregion Change Log
        enum class FileMode
        {
            READ,
            WRITE,
            READ_WRITE,
            OVERWRITE,
            APPEND,
        };
    }
}


#endif