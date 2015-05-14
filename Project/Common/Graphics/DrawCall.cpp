#include "DrawCall.h"


namespace Gem
{
	DrawCall::DrawCall()
		:model(Matrix4x4::Identity())
	{
		mesh.Release();
		material.Release();
	}

	DrawCall::DrawCall(Matrix4x4 aModel, Pointer<Mesh> aMesh, Pointer<Material> aMaterial)
		: model(aModel), mesh(aMesh), material(aMaterial)
	{
	}

	DrawCall::~DrawCall()
	{

	}
}