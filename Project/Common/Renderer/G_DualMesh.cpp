#include "G_DualMesh.h"
#include "G_Graphics.h"
#include "G_VertexInfo.h"

#include <stdlib.h>
#include <vector>

#include <fstream>

namespace Gem
{
	DualMesh::DualMesh()
	{
		m_CustomVertices = MemoryHandle<DualVertexInfo>(Memory::HANDLE_INVALID_MEMORY, nullptr);
	}

	void DualMesh::Upload(bool aFree) 
	{
		///Free any allocated memory on the GPU
		FreeGPU();

		DualVertexInfo * vertices = m_CustomVertices;
		unsigned short * indices = m_Indices;

		///TODO: If the garbage collector is to run on a separate thread, check for GC Flag on object
		///(vertices->Flags() & Reflection::FLAG_GC_COLLECT) == Reflection::FLAG_GC_COLLECT
		if (vertices == nullptr || indices == nullptr)
		{
			return;
		}

		glGenBuffers(1, &m_VBOHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOHandle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DualVertexInfo)* m_VertexCount, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_IBOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)* m_IndexCount, m_Indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_UploadedIndexCount = m_IndexCount;
		///Free allocated memory on the CPU
		if (aFree == true)
		{
			FreeCPU();
		}
		m_IsStatic = true;
	}
	void DualMesh::FreeCPU()
	{
		Mesh::FreeCPU();
		if (m_CustomVertices.Memory() != nullptr)
		{
			m_CustomVertices = Memory::DestroyArrayHandle<DualVertexInfo>(m_CustomVertices, m_VertexCount);
		}
	}
	void DualMesh::FreeGPU()
	{
		Mesh::FreeGPU();
	}
	void DualMesh::Load(string & aPathA, string & aPathB)
	{
		FreeCPU();

		///Parse first models vertices,uvs and normals
		std::vector<unsigned int> vertexIndicies, uvIndicies, normalIndicies;
		std::vector<ClassUtils::Vector3> tempVerts;
		std::vector<ClassUtils::Vector2> tempUvs;
		std::vector<ClassUtils::Vector3> tempNormals;
		FILE * file;
		file = fopen(aPathA.C_Str(), "r");
		if (file == NULL)
		{
			printf("Failed to open file %s", aPathA.C_Str());
			return;
		}

		int line = 0;
		while (true)
		{
			char lineHeader[128];
			int result = fscanf(file, "%s", lineHeader);
			if (result == EOF)
			{
				break;
			}
			line++;;
			if (strcmp(lineHeader, "#") == 0)
			{
				fgets(lineHeader, 128, file);
			}
			else if (strcmp(lineHeader, "v") == 0)
			{
				ClassUtils::Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tempVerts.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				ClassUtils::Vector2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				tempUvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				ClassUtils::Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{

				int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9)
				{
					printf("Failed to read line %d in file %s", line, aPathA.C_Str());
					return;
				}
				vertexIndicies.push_back(vertexIndex[0]);
				vertexIndicies.push_back(vertexIndex[1]);
				vertexIndicies.push_back(vertexIndex[2]);
				uvIndicies.push_back(uvIndex[0]);
				uvIndicies.push_back(uvIndex[1]);
				uvIndicies.push_back(uvIndex[2]);
				normalIndicies.push_back(normalIndex[0]);
				normalIndicies.push_back(normalIndex[1]);
				normalIndicies.push_back(normalIndex[2]);
			}
		}
		fclose(file);

		//parse second models vertices
		std::vector<unsigned int> positionBuffer;
		std::vector<ClassUtils::Vector3> tempPositions;
		file = fopen(aPathB.C_Str(), "r");
		if (file == NULL)
		{
			printf("Failed to open file %s", aPathB.C_Str());
			return;
		}
		line = 0;
		while (true)
		{
			char lineHeader[128];
			int result = fscanf(file, "%s", lineHeader);
			if (result == EOF)
			{
				break;
			}
			line++;;
			if (strcmp(lineHeader, "#") == 0)
			{
				fgets(lineHeader, 128, file);
			}
			else if (strcmp(lineHeader, "v") == 0)
			{
				ClassUtils::Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tempPositions.push_back(vertex);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{

				int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9)
				{
					printf("Failed to read line %d in file %s", line, aPathB.C_Str());
					return;
				}
				positionBuffer.push_back(vertexIndex[0]);
				positionBuffer.push_back(vertexIndex[1]);
				positionBuffer.push_back(vertexIndex[2]);
			}
		}
		int sizeA = vertexIndicies.size();
		int sizeB = positionBuffer.size();
		if (sizeA != sizeB)
		{
			printf("This mesh does not have the same number of vertices as the other. Cannot dual load meshes");
			return;
		}
		m_VertexCount = sizeA;
		m_IndexCount = m_VertexCount;
		if (m_VertexCount <= 2)
		{
			printf("This file does not contain any faces");
			return;
		}

		m_CustomVertices = Memory::InstantiateArray<DualVertexInfo>(m_VertexCount);
		m_Indices = Memory::InstantiateArray<unsigned short>(m_VertexCount);
		DualVertexInfo * vertices = m_CustomVertices;
		unsigned short * indicies = m_Indices;
		for (unsigned int i = 0; i < vertexIndicies.size(); i++)
		{
			unsigned int vertexIndex = vertexIndicies[i];
			unsigned int uvIndex = uvIndicies[i];
			unsigned int normalIndex = normalIndicies[i];
			unsigned int positionB = positionBuffer[i];

			DualVertexInfo info;
			info.position = tempVerts[vertexIndex - 1];
			info.texCoord = tempUvs[uvIndex - 1];
			info.positionB = tempPositions[positionB - 1];
			//info.normal = tempNormals[normalIndex - 1];
			info.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			vertices[i] = info;
			indicies[i] = i;
		}

	}
}