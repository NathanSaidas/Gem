#include "Geometry.h"
#include "../Utilities/Utilities.h"
#include "../Memory/Memory.h"
#include "Mesh.h"
#include "Color.h"


using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(Geometry, object)

	Geometry::Geometry()
	{

	}
	Geometry::~Geometry()
	{

	}

	//Create a plane with CW winding
	Mesh * Geometry::CreatePlane(Float32 aWidth, Float32 aHeight, const Color & aColor, const Memory::AllocatorType & aAllocatorType)
	{
		Array<Vector3> positions;
		Array<Vector3> normals;
		Array<Vector2> texCoords;
		Array<Color> colors;
		Array<UInt16> indicies;

		positions.Allocate(6);
		normals.Allocate(6);
		texCoords.Allocate(6);
		colors.Allocate(6);
		indicies.Allocate(6);

		positions[0] = Vector3(-aWidth, 0.0f,  aHeight); //topleft
		positions[1] = Vector3( aWidth, 0.0f, -aHeight); //bottom right
		positions[2] = Vector3(-aWidth, 0.0f, -aHeight); //bottom left

		positions[3] = Vector3(-aWidth, 0.0f,  aHeight); //top left
		positions[4] = Vector3( aWidth, 0.0f,  aHeight); //top right
		positions[5] = Vector3( aWidth, 0.0f, -aHeight); //bottom right

		normals[0] = normals[1] = normals[2] = normals[3] = normals[4] = normals[5] = Vector3::Up();
		
		texCoords[0] = Vector2(0.0f, 0.0f); //topleft
		texCoords[1] = Vector2(1.0f, 1.0f);	//bottom right
		texCoords[2] = Vector2(0.0f, 1.0f);	//bottom left

		texCoords[3] = Vector2(0.0f, 0.0f);	//top left
		texCoords[4] = Vector2(1.0f, 0.0f);	//top right
		texCoords[5] = Vector2(1.0f, 1.0f);	//bottom right

		colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = colors[5] = aColor;

		indicies[0] = 0;
		indicies[1] = 1;
		indicies[2] = 2;
		indicies[3] = 3;
		indicies[4] = 4;
		indicies[5] = 5;

		Mesh * mesh = nullptr;
		switch (aAllocatorType)
		{
		case Memory::AllocatorType::Pool:
			mesh = MEM_POOL_ALLOC_T(Mesh);
			break;
		case Memory::AllocatorType::Stack:
			mesh = MEM_STACK_ALLOC_T(Mesh);
			break;
		case Memory::AllocatorType::Frame:
			mesh = MEM_FRAME_ALLOC_T(Mesh);
			break;
		default:
			Debug::ErrorFormat("Graphics", nullptr, "Invalid allocator type (%u) specified for creating a plant.", (unsigned int)aAllocatorType);
			break;
		}
		if (mesh != nullptr)
		{
			mesh->SetPositions(positions);
			mesh->SetNormals(normals);
			mesh->SetTexCoords(texCoords);
			mesh->SetColors(colors);
			mesh->SetIndices(indicies);
			if (!mesh->Upload())
			{
				Debug::Error("Graphics", "Failed to upload mesh plane to GPU");
			}
		}
		return mesh;
	}
	Mesh * Geometry::CreateCube(Float32 aWidth, Float32 aHeight, Float32 aDepth, const Color & aColor, const Memory::AllocatorType & aAllocatorType)
	{
		Array<Vector3> positions;
		Array<Vector3> normals;
		Array<Vector2> texCoords;
		Array<Color> colors;
		Array<UInt16> indicies;

		positions.Allocate(36);
		normals.Allocate(36);
		texCoords.Allocate(36);
		colors.Allocate(36);
		indicies.Allocate(36);


		float halfWidth = aWidth * 0.5f;
		float halfHeight = aHeight * 0.5f;
		float halfDepth = aDepth * 0.5f;

		const int F_TOP_LEFT		= 0;
		const int F_TOP_RIGHT		= 1;
		const int F_BOTTOM_RIGHT	= 2;
		const int F_BOTTOM_LEFT		= 3;
		const int B_TOP_LEFT		= 4;
		const int B_TOP_RIGHT		= 5;
		const int B_BOTTOM_RIGHT	= 6;
		const int B_BOTTOM_LEFT		= 7;


		Vector3 vertexPositions[8] =
		{
			//Front Face
			Vector3(-halfWidth,  halfHeight, -halfDepth), //TopLeft
			Vector3( halfWidth,  halfHeight, -halfDepth), //TopRight
			Vector3( halfWidth, -halfHeight, -halfDepth), //Bottom Right
			Vector3(-halfWidth, -halfHeight, -halfDepth), //Bottom Left

			//Back Face
			Vector3( halfWidth,  halfHeight,  halfDepth), //TopLeft
			Vector3(-halfWidth,  halfHeight,  halfDepth), //TopRight
			Vector3(-halfWidth, -halfHeight,  halfDepth), //Bottom Right
			Vector3( halfWidth, -halfHeight,  halfDepth)  //Bottom Left
		};

		//Front
		positions[0] = vertexPositions[F_TOP_LEFT];
		positions[1] = vertexPositions[F_BOTTOM_RIGHT];
		positions[2] = vertexPositions[F_BOTTOM_LEFT];
		positions[3] = vertexPositions[F_TOP_LEFT];
		positions[4] = vertexPositions[F_TOP_RIGHT];
		positions[5] = vertexPositions[F_BOTTOM_RIGHT];
		//Back
		
		positions[6]  = vertexPositions[B_TOP_LEFT];
		positions[7]  = vertexPositions[B_BOTTOM_RIGHT];
		positions[8]  = vertexPositions[B_BOTTOM_LEFT];
		positions[9]  = vertexPositions[B_TOP_LEFT];
		positions[10] = vertexPositions[B_TOP_RIGHT];
		positions[11] = vertexPositions[B_BOTTOM_RIGHT];

		//Left
		positions[12] = vertexPositions[B_TOP_RIGHT];
		positions[13] = vertexPositions[F_BOTTOM_LEFT];
		positions[14] = vertexPositions[B_BOTTOM_RIGHT];
		positions[15] = vertexPositions[B_TOP_RIGHT];
		positions[16] = vertexPositions[F_TOP_LEFT];
		positions[17] = vertexPositions[F_BOTTOM_LEFT];

		//Right
		positions[18] = vertexPositions[F_TOP_RIGHT];
		positions[19] = vertexPositions[B_BOTTOM_LEFT];
		positions[20] = vertexPositions[F_BOTTOM_RIGHT];
		positions[21] = vertexPositions[F_TOP_RIGHT];
		positions[22] = vertexPositions[B_TOP_LEFT];
		positions[23] = vertexPositions[B_BOTTOM_LEFT];

		//Top
		positions[24] = vertexPositions[B_TOP_RIGHT];
		positions[25] = vertexPositions[F_TOP_RIGHT];
		positions[26] = vertexPositions[F_TOP_LEFT];
		positions[27] = vertexPositions[B_TOP_RIGHT];
		positions[28] = vertexPositions[B_TOP_LEFT];
		positions[29] = vertexPositions[F_TOP_RIGHT];
		//Bottom
		positions[30] = vertexPositions[B_BOTTOM_LEFT];
		positions[31] = vertexPositions[F_BOTTOM_LEFT];
		positions[32] = vertexPositions[F_BOTTOM_RIGHT];
		positions[33] = vertexPositions[B_BOTTOM_LEFT];
		positions[34] = vertexPositions[B_BOTTOM_RIGHT];
		positions[35] = vertexPositions[F_BOTTOM_LEFT];
		
		//Front
		normals[0] = normals[1] = normals[2] = normals[3] = normals[4] = normals[5] = Vector3::Back();
		//Back
		normals[6] = normals[7] = normals[8] = normals[9] = normals[10] = normals[11] = Vector3::Forward();
		//Left
		normals[12] = normals[13] = normals[14] = normals[15] = normals[16] = normals[17] = Vector3::Left();
		//Rigth
		normals[18] = normals[19] = normals[20] = normals[21] = normals[22] = normals[23] = Vector3::Right();
		//Top
		normals[24] = normals[25] = normals[26] = normals[27] = normals[28] = normals[29] = Vector3::Up();
		//Bottom
		normals[30] = normals[31] = normals[32] = normals[33] = normals[34] = normals[35] = Vector3::Down();


		texCoords[0] = texCoords[6]  = texCoords[12] = texCoords[18] = texCoords[24] = texCoords[30] = Vector2(0.0f, 0.0f); //topleft
		texCoords[1] = texCoords[7]  = texCoords[13] = texCoords[19] = texCoords[25] = texCoords[31] = Vector2(1.0f, 1.0f);	//bottom right
		texCoords[2] = texCoords[8]  = texCoords[14] = texCoords[20] = texCoords[26] = texCoords[32] = Vector2(0.0f, 1.0f);	//bottom left
		texCoords[3] = texCoords[9]  = texCoords[15] = texCoords[21] = texCoords[27] = texCoords[33] = Vector2(0.0f, 0.0f);	//top left
		texCoords[4] = texCoords[10] = texCoords[16] = texCoords[22] = texCoords[28] = texCoords[34] = Vector2(1.0f, 0.0f);	//top right
		texCoords[5] = texCoords[11] = texCoords[17] = texCoords[23] = texCoords[29] = texCoords[35] = Vector2(1.0f, 1.0f);	//bottom right

		for (int i = 0; i < 36; i++)
		{
			colors[i] = aColor;
			indicies[i] = i;
		}

		Mesh * mesh = nullptr;
		switch (aAllocatorType)
		{
		case Memory::AllocatorType::Pool:
			mesh = MEM_POOL_ALLOC_T(Mesh);
			break;
		case Memory::AllocatorType::Stack:
			mesh = MEM_STACK_ALLOC_T(Mesh);
			break;
		case Memory::AllocatorType::Frame:
			mesh = MEM_FRAME_ALLOC_T(Mesh);
			break;
		default:
			Debug::ErrorFormat("Graphics", nullptr, "Invalid allocator type(%u) specified for creating a plane.", (unsigned int)aAllocatorType);
			break;
		}
		if (mesh != nullptr)
		{
			mesh->SetPositions(positions);
			mesh->SetNormals(normals);
			mesh->SetTexCoords(texCoords);
			mesh->SetColors(colors);
			mesh->SetIndices(indicies);
			if (!mesh->Upload())
			{
				Debug::Error("Graphics", "Failed to upload mesh cube to GPU");
			}
		}
		return mesh;
	}
}