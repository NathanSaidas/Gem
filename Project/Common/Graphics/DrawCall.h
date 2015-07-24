#ifndef GEM_DRAW_CALL_H
#define GEM_DRAW_CALL_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class DrawCall used for rendering operations in Graphics
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor uses initializer list now.
#pragma endregion

//API
#include "../Core/Core.h"
//Memory
#include "../Memory/Memory.h"
//Math
#include "../Math/Matrix4x4.h"
#include "../Math/Vector3.h"
//Graphics
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

namespace Gem
{


	struct DrawCall
	{
		DrawCall();
		DrawCall(Matrix4x4 aModel, Pointer<Mesh> aMesh, Pointer<Material> aMaterial);
		~DrawCall();


		Matrix4x4 model;
		Pointer<Mesh> mesh;
		Pointer<Material> material;
	};
}

#endif // GEM_DRAW_CALL_H