#include "Utilities.h"
#include <sstream>
#include <string>

namespace Gem
{
	string Utilities::I2S(int & aInt)
	{
		string result;
		std::stringstream stream;
		stream << aInt;
		if (stream.fail())
		{
			return string::Empty();
		}
		return stream.str().c_str();
	}
	int Utilities::S2I(string & aString)
	{
		int result;
		std::stringstream stream(aString.C_Str());
		stream >> result;
		if (stream.fail())
		{
			return -1;
		}
		return result;
	}
	string Utilities::F2S(float & aFloat)
	{
		string result;
		std::stringstream stream;
		stream << aFloat;
		if (stream.fail())
		{
			return string::Empty();
		}
		return stream.str().c_str();
	}
	float Utilities::S2F(string & aString)
	{
		float result;
		std::stringstream stream(aString.C_Str());
		stream >> result;
		if (stream.fail())
		{
			return -1;
		}
		return result;
	}
}