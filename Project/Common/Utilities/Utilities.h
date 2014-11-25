#ifndef G_UTILITIES_H
#define G_UTILITIES_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	class Utilities : public Object
	{
	public:
		///Converts a int to a string
		static string I2S(int & aInt);
		///Converts a string to an int
		static int S2I(string & aString);
		///Converts float to string
		static string F2S(float & aFloat);
		///Converts a string to a float
		static float S2F(string & aString);
	};
}

GEM_CLASS(Utilities,Object)

#endif