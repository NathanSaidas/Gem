#include "Color.h"

namespace Gem
{
	RDEFINE_CLASS(Color, object)
	Color::Color()
		: r(0.0f), g(0.0f), b(0.0f), a(0.0f)
	{

	}

	Color::Color(Float32 r, Float32 g, Float32 b, Float32 a)
		: r(r), g(g), b(b), a(a)
	{

	}
	Color::~Color()
	{

	}
}