#include "StringExtensions.h"
#include <sstream>

namespace Gem
{
	namespace Utilities
	{
		void GetWords(std::string aString, std::vector<std::string> & aWords)
		{
			UInt32 end = 0;

			while (aString.size() > 0)
			{
				for (unsigned int i = 0; i < aString.size(); i++)
				{
					if (aString[i] == ' ')
					{
						end = i;
						string word = aString.substr(0, end);
						aString.erase(aString.begin(), aString.begin() + end + 1);
						aWords.push_back(word);
						break;
					}
					else if (i == aString.size() - 1)
					{
						end = i;
						string word = aString.substr(0, end + 1);
						aString.clear();
						aWords.push_back(word);
						break;
					}
				}

			}
		}

		std::string F2S(Float32 aValue)
		{
			std::stringstream stream;
			stream << aValue;

			std::string sValue = stream.str();
			aValue = S2F(sValue);
			if (aValue == (int)aValue)
			{
				sValue.append(".0");
			}
			return sValue;
		}
		std::string I2S(SInt32 aValue)
		{
			std::stringstream stream;
			stream << aValue;
			return stream.str();
		}

		Float32 S2F(const std::string & aString)
		{
			std::stringstream stream(aString);
			float value = 0.0f;
			stream >> value;
			return value;
		}
		SInt32 S2I(const std::string & aString)
		{
			std::stringstream stream(aString);
			SInt32 value = 0;
			stream >> value;
			return value;
		}
	}
}