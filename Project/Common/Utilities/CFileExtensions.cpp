#include "CFileExtensions.h"

namespace Gem
{
	namespace Utilities
	{
		bool ReadLine(FILE* file, std::string & outString)
		{
			if (file == nullptr)
			{
				return false;
			}

			int c = 0;
			bool endOfFile = false;

			while (!endOfFile)
			{
				c = fgetc(file);
				if (c == EOF)
				{
					return true;
				}
				else if (c == '\n')
				{
					return false;
				}
				else
				{
					outString.push_back((char)c);
				}
			}
			return endOfFile;
		}
	}
	
}