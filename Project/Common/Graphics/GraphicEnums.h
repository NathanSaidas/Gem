#ifndef GEM_GRAPHICS_ENUMS_H
#define GEM_GRAPHICS_ENUMS_H

namespace Gem
{
    enum class PrimitiveMode
    {
#ifdef GRAPHICS_OPEN_GL
        Points = 0x0000, //GL_POINTS,

        Lines = 0x0001, //GL_LINES,
        LineLoops = 0x0002, //GL_LINE_LOOP,
        LineStrips = 0x0003, //GL_LINE_STRIP,

        Triangles = 0x0004, //GL_TRIANGLES,
        TriangleStrips = 0x0005, //GL_TRIANGLE_STRIP,
        TriangleFans = 0x0006, //GL_TRIANGLE_FAN
#elif GRAPHICS_DIRECT_3D

#endif
    };

    enum class BufferTarget
    {
#ifdef GRAPHICS_OPEN_GL
        Array = 0x8892,   //ARRAY_BUFFER
        ElementArray = 0x8893    // GL_ELEMENT_ARRAY_BUFFER
#elif GRAPHICS_DIRECT_3D

#endif
    };

    enum class BufferMode
    {
#ifdef GRAPHICS_OPEN_GL
        StaticRead = 0x88E5,  //GL_STATIC_READ
        StaticCopy = 0x88E6,  //GL_STATIC_COPY
        StaticDraw = 0x88E4,  //GL_STATIC_DRAW
        DynamicRead = 0x88E9,  //GL_DYNAMIC_READ
        DynamicCopy = 0x88EA,  //GL_DYNAMIC_COPY
        DynamicDraw = 0x88E8,  //GL_DYNAMIC_DRAW
        StreamRead = 0x88E1,  //GL_STREAM_READ
        StreamCopy = 0x88E2,  //GL_STREAM_COPY
        StreamDraw = 0x88E0   //GL_STREAM_DRAW
#elif GRAPHICS_DIRECT_3D

#endif
    };


    enum class PrimitiveShape
    {
        Point,
        Line,
        Triangle,
        Circle,
        Rectangle,
        Plane,
        Cube,
        Sphere
    };

    enum class ShaderType
    {
#ifdef GRAPHICS_OPEN_GL
        Vertex = 0x8B31, //GL_VERTEX_SHADER
        Fragment = 0x8B30, //GL_FRAGMENT_SHADER,
        Geometry = 0x8DD9, //GL_GEOMETRY_SHADER,
        TessellationHull,
        TessellationDomain
#elif GRAPHICS_DIRECT_3D

#endif
    };

	enum class FilterMode
	{
		Nearest = 0x2600, // GL_NEAREST
		Linear = 0x2601, // GL_LINEAR
		MipmapNearest = 0x2700, //GL_NEAREST_MIPMAP_NEAREST
		MipmapLinear = 0x2703, //GL_LINEAR_MIPMAP_LINEAR
		MipmapLinearNearest = 0x2702 // GL_NEAREST_MIPMAP_LINEAR
	};

	enum class WrapMode
	{
		Clamp = 0x812F,			//GL_CLAMP_TO_EDGE,
		Repeat = 0x2901,		// GL_REPEAT,
		MirroredRepeat = 0x8370 // GL_MIRRORED_REPEAT
	};

	enum class ImageFormat
	{
		RGB = 0x1907,		//GL_RGB
		RGBA = 0x1908,		//GL_RGBA
		Greyscale = 0x2002,	//GL_R	
		GreyAlpha = 0x8227	//GL_RG
	};

	enum class CullMode
	{
		Front = 0x0404,			//GL_FRONT
		Back = 0x0405,			//GL_BACK
		FrontAndBack = 0x0408	//GL_FRONT_AND_BACK
	};

	enum class CullFace
	{
		ClockWise = 0x0900,			//GL_CW
		CounterClockWise = 0x0901	//GL_CCW
	};

	enum class DepthFunc
	{
		Never =			0x0200, //GL_NEVER
		Less =			0x0201, //GL_LESS
		Equal =			0x0202, //GL_EQUAL
		LessEqual =		0x0203, //GL_LEQUAL
		Greater =		0x0204, //GL_GREATER
		NotEqual =		0x0205, //GL_NOTEQUAL
		GreaterEqual =	0x0206, //GL_GEQUAL
		Always =		0x0207, //GL_ALWAYS
	};

	enum class BlendFunc
	{
		Zero				= 0, //GL_ZERO
		One					= 1, //GL_ONE
		SrcColor			= 0x0300, //GL_SRC_COLOR
		OneMinusSrcColor	= 0x0301, //GL_ONE_MINUS_SRC_COLOR
		SrcAlpha			= 0x0302, //GL_SRC_ALPHA
		OneMinusSrcAlpha    = 0x0303, //GL_ONE_MINUS_SRC_ALPHA
		DstAlpha			= 0x0304, //GL_DST_ALPHA
		OneMinusDstAlpha    = 0x0305, //GL_ONE_MINUS_DST_ALPHA
		DstColor			= 0x0306, //GL_DST_COLOR
		OneMinusDstColor	= 0x0307, //GL_ONE_MINUS_DST_COLOR
	};

	enum class GraphicsState
	{
		CullFace,
		Blending,
		DepthTesting
	};
}

#endif //GEM_GRAPHICS_ENUMS_H