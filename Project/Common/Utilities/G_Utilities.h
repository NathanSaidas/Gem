#ifndef G_UTILITIES_H
#define G_UTILITIES_H

#include "../G_Object.h"

namespace Gem
{
	class Utilities : public object
	{
		G_CLASS_DEF(Utilities)
	public:
		///Converts a int to a string
		static std::string I2S(int & aInt);
		///Converts a string to an int
		static int S2I(std::string & aString);
		///Converts float to string
		static std::string F2S(float & aFloat);
		///Converts a string to a float
		static float S2F(std::string & aString);


	};
}


#endif