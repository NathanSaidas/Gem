#ifndef GEM_GEOMETRY_H
#define GEM_GEOMETRY_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Geometry used for constructing geometry for Graphics.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
#pragma endregion

//API and basic types
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
//Memory 
#include "../Memory/Memory.h"

namespace Gem
{
	class Mesh;
	class Color;

	class Geometry;
	template class GEM_API Reflection::MetaObject<Geometry>;

	class GEM_API Geometry : public object
	{
		RDECLARE_CLASS(Geometry)
	public:
		Geometry();
		~Geometry();


		static Mesh * CreatePlane(Float32 aWidth, Float32 aHeight,const Color & aColor, const Memory::AllocatorType & AllocType);
		static Mesh * CreateCube(Float32 aWidth, Float32 aHeight, Float32 aDepth, const Color & aColor, const Memory::AllocatorType & aAllocatorType);

	private:

	};

	TYPE_DEFINE(Geometry)
}


#endif // GEM_GEOMETRY_H