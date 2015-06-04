#ifndef GEM_C_FILE_EXTENSIONS_H
#define GEM_C_FILE_EXTENSIONS_H

#include <string>
#include <cstdio>



namespace Gem
{
	namespace Utilities
	{
		/**
		* Reads a line from a file.
		* @param file The file to read from
		* @param outString An output string. This will contain the new line without the \n character at the end.
		* @return Returns true if the end of the file has been reached. False otherwise.
		*/
		bool ReadLine(FILE* file, std::string & outString);
	}
	
}

#endif