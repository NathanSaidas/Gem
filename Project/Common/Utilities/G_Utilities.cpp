#include "G_Utilities.h"
#include <sstream>
#include <string>

namespace Gem
{
	G_CLASS_IMPLEMENTATION(Utilities,object)
	std::string Utilities::I2S(int & aInt)
	{
		std::string result;
		std::stringstream stream;
		stream << aInt;
		if (stream.fail())
		{
			return "";
		}
		return stream.str().c_str();
	}
	int Utilities::S2I(std::string & aString)
	{
		int result;
		std::stringstream stream(aString.c_str());
		stream >> result;
		if (stream.fail())
		{
			return -1;
		}
		return result;
	}
	std::string Utilities::F2S(float & aFloat)
	{
		std::string result;
		std::stringstream stream;
		stream << aFloat;
		if (stream.fail())
		{
			return "";
		}
		return stream.str().c_str();
	}
	float Utilities::S2F(std::string & aString)
	{
		float result;
		std::stringstream stream(aString.c_str());
		stream >> result;
		if (stream.fail())
		{
			return -1;
		}
		return result;
	}
}