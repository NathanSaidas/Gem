#ifndef GEM_COLOR_H
#define GEM_COLOR_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Color,  used for color operations in Graphics.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor uses initializer list now.
#pragma endregion

// Basic includes for types and API spec.
#include "../Core/Core.h"

namespace Gem
{

	//Explicit export of color type.
    //FORCE_EXPORT_META(Color);
	

    //TODO: Implement functions such as mixing / lerping and static constant color values
    class Color : public object
    {
		RDECLARE_CLASS(Color)
    public:
		Color();
		Color(Float32 r, Float32 g, Float32 b, Float32 a);
		~Color();
		
        Float32 r;
        Float32 g;
        Float32 b;
        Float32 a;

		inline static const Color White() 
		{
			return Color(1.0f, 1.0f, 1.0f, 1.0f);
		}
		inline static const Color Black() 		
		{
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		}

		inline static const Color Crimson()
		{
			return ByteToFloat(220.0f, 20.0f, 60.0f);
		}
		inline static const Color Indigo()
		{
			return ByteToFloat(75.0f, 0.0f, 130.0f);
		}
		inline static const Color CornflowerBlue()
		{
			return ByteToFloat(100.f, 149.0f, 237.0f);
		}
		inline static const Color MidnightBlue()
		{
			return ByteToFloat(25.0f, 25.0f, 112.0f);
		}
		inline static const Color Turquoise()
		{
			return ByteToFloat(64.0f, 224.0f, 208.0f);
		}

		inline static const Color ByteToFloat(Float32 r, Float32 g, Float32 b, Float32 aAlpha = 1.0f)
		{
			return Color(r / 255.0f, g / 255.0f, b / 250.0f, aAlpha);
		}
    };

    TYPE_DEFINE(Color)
}

#endif // GEM_COLOR_H