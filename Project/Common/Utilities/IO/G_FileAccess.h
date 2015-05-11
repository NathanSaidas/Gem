#ifndef G_FILE_ACCESS_H
#define G_FILE_ACCESS_H

namespace Gem
{
    namespace FileIO
    {
#pragma region Change Log
        /* 
        * 
        */
#pragma endregion Change Log
        enum class FileAccess
        {
            EXISTENCE = 0,
            WRITE_ONLY = 2,
            READ_ONLY = 4,
            READ_WRITE_ONLY = 6
        };
    }
}

#endif