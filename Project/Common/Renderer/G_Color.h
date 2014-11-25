#ifndef G_COLOR_H
#define G_COLOR_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	//
	class Color // : public Object
	{
	public:
		Color() : r(0.0f),g(0.0f), b(0.0f), a(0.0f) {}
		Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}
		~Color(){}
		float r;
		float g;
		float b;
		float a;
	};
}
//GEM_CLASS(Color,Object)
#endif