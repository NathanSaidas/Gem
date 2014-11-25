#include "G_Mesh.h"
#include "G_Graphics.h"
#include "G_VertexInfo.h"

#include <stdlib.h>
#include <vector>

#include <fstream>


namespace Gem
{
	Mesh::Mesh()
	{
		m_VBOHandle = 0;
		m_IBOHandle = 0;
		m_IsStatic = false;
		m_VertexCount = 0;
		m_Vertices = MemoryHandle<VertexInfo>(Memory::HANDLE_INVALID_MEMORY, nullptr);
		m_IndexCount = 0;
		m_Indices = MemoryHandle<unsigned short>(Memory::HANDLE_INVALID_MEMORY, nullptr);
	}
	Mesh::~Mesh()
	{
		Free();
	}

	unsigned int Mesh::VBOHandle()
	{
		return m_VBOHandle;
	}
	unsigned int Mesh::IBOHandle()
	{
		return m_IBOHandle;
	}
	bool Mesh::IsStatic()
	{
		return m_IsStatic;
	}
	int Mesh::VertexCount()
	{
		return m_VertexCount;
	}
	MemoryHandle<VertexInfo> Mesh::Vertices()
	{
		return m_Vertices;
	}
	MemoryHandle<VertexInfo> Mesh::Vertices(int aVertexCount,MemoryHandle<VertexInfo> & aInfo)
	{
		m_VertexCount = aVertexCount;
		return m_Vertices = aInfo;
	}
	int Mesh::IndexCount()
	{
		return m_IndexCount;
	}
	MemoryHandle<unsigned short> Mesh::Indices()
	{
		return m_Indices;
	}
	MemoryHandle<unsigned short> Mesh::Indices(int aIndexCount, MemoryHandle<unsigned short> & aIndices)
	{
		m_IndexCount = aIndexCount;
		return m_Indices = aIndices;
	}
	void Mesh::Upload(bool aFree)
	{
		///Free any allocated memory on the GPU
		FreeGPU();

		VertexInfo * vertices = m_Vertices;
		unsigned short * indices = m_Indices;

		///TODO: If the garbage collector is to run on a separate thread, check for GC Flag on object
		///(vertices->Flags() & Reflection::FLAG_GC_COLLECT) == Reflection::FLAG_GC_COLLECT
		if (vertices == nullptr || indices == nullptr)
		{
			return;
		}
		
		glGenBuffers(1, &m_VBOHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOHandle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo)* m_VertexCount, vertices, GL_STATIC_DRAW);
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
	void Mesh::FreeCPU()
	{
		///Clean up the vertices / indicies handles
		if (m_Vertices.Memory() != nullptr)
		{
			m_Vertices = Memory::DestroyArrayHandle<VertexInfo>(m_Vertices,m_VertexCount);
		}
		if (m_Indices.Memory() != nullptr)
		{
			m_Indices = Memory::DestroyArrayHandle<unsigned short>(m_Indices, m_IndexCount);
		}

		m_VertexCount = 0;
		m_IndexCount = 0;

	}
	void Mesh::FreeGPU()
	{
		if (m_IsStatic == true)
		{
			if (m_VBOHandle != 0)
			{
				glDeleteBuffers(1, &m_VBOHandle);
				m_VBOHandle = 0;
			}
			if (m_IBOHandle != 0)
			{
				glDeleteBuffers(1, &m_IBOHandle);
				m_IBOHandle = 0;
			}
#ifdef _DEBUG
			Graphics::CheckForErrors();
#endif
			m_IsStatic = false;
		}
	}
	void Mesh::Free()
	{
		FreeGPU();
		FreeCPU();
	}
	void Mesh::Load(string & aPath)
	{
		FreeCPU();

		std::vector<unsigned int> vertexIndicies, uvIndicies, normalIndicies;
		std::vector<ClassUtils::Vector3> tempVerts;
		std::vector<ClassUtils::Vector2> tempUvs;
		std::vector<ClassUtils::Vector3> tempNormals;
		FILE * file;
		file = fopen(aPath.C_Str(), "r");
		if (file == NULL)
		{
			printf("Failed to open file %s", aPath.C_Str());
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
					printf("Failed to read line %d in file %s", line, aPath.C_Str());
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

		m_VertexCount = vertexIndicies.size();
		m_IndexCount = m_VertexCount;
		if (m_VertexCount <= 2)
		{
			printf("This file does not contain any faces");
			return;
		}

		m_Vertices = Memory::InstantiateArray<VertexInfo>(m_VertexCount);
		m_Indices = Memory::InstantiateArray<unsigned short>(m_VertexCount);
		VertexInfo * vertices = m_Vertices;
		unsigned short * indicies = m_Indices;
		for (unsigned int i = 0; i < vertexIndicies.size(); i++)
		{
			unsigned int vertexIndex = vertexIndicies[i];
			unsigned int uvIndex = uvIndicies[i];
			unsigned int normalIndex = normalIndicies[i];

			VertexInfo info;
			info.position = tempVerts[vertexIndex - 1];
			info.texCoord = tempUvs[uvIndex - 1];
			info.normal = tempNormals[normalIndex - 1];
			info.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			vertices[i] = info;
			indicies[i] = i;
		}

	}
	

	unsigned short Mesh::UploadedIndexCount()
	{
		return m_UploadedIndexCount;
	}
}