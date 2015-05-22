#ifndef GEM_STRING_EXTENSIONS_H
#define GEM_STRING_EXTENSIONS_H

#include "../Core/BasicTypes.h"
#include <string>

namespace Gem
{
	namespace Utilities
	{
		void GetWords(std::string aString, std::vector<std::string> & aWords);
		std::string F2S(Float32 aValue);
		std::string I2S(SInt32 aValue);
		std::string UI2S(UInt32 aValue);
		Float32 S2F(const std::string & aString);
		SInt32 S2I(const std::string & aString);
		UInt32 S2UI(const std::string & aString);

		std::string ByteToString(UInt8 aValue);
		UInt8 StringToByte(const std::string & aString);
	}
}


#endif
