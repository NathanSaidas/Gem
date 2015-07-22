#include "Mesh.h"
#include "Graphics.h"
#include <stdio.h>

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(Mesh, Resource)


    Mesh::Mesh() : Resource(),
		m_VBO(0),
		m_IBO(0),
		m_Uploaded(false),
		m_AttributeCount(0),
		m_IndexCount(0)
    {

    }
    Mesh::~Mesh()
    {
        Release(true);
    }
    bool Mesh::LoadFile(const std::string &aFileName)
    {
        FILE * file;
        file = fopen(aFileName.c_str(), "r");
        if (file == NULL)
        {
			Debug::ErrorFormat("Graphics",nullptr, "Failed to open file %s", aFileName.c_str());
            return false;
        }
        std::vector<UInt16> vertexIndices, texCoordIndices, normalIndices;
        std::vector<Vector3> tempPositions;
        std::vector<Vector3> tempNormals;
        std::vector<Vector2> tempTexCoords;
        std::vector<Color> tempColors;

        int line = 0;

        while (true)
        {
            char lineHeader[128];
            int result = fscanf(file, "%s", lineHeader);
            if (result == EOF)
            {
                break;
            }
            line++;
            if (strcmp(lineHeader, "#") == 0)
            {
                fgets(lineHeader, 128, file);
            }
            else if (strcmp(lineHeader, "v") == 0)
            {
                Vector3 position;
                fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
                tempPositions.push_back(position);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                Vector2 texCoords;
                fscanf(file, "%f %f\n", &texCoords.x, &texCoords.y);
                tempTexCoords.push_back(texCoords);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                Vector3 normal;
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
					Debug::ErrorFormat("Graphics", nullptr, "Failed to read line %d in file %s", line, aFileName.c_str());
                    fclose(file);
                    return false;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                texCoordIndices.push_back(uvIndex[0]);
				texCoordIndices.push_back(uvIndex[1]);
				texCoordIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
        fclose(file);

        m_Positions.Release();
        m_Colors.Release();
        m_TexCoords.Release();
        m_Normals.Release();
        m_Indices.Release();

        m_Positions.Allocate(vertexIndices.size());
		m_Colors.Allocate(vertexIndices.size());
		m_TexCoords.Allocate(vertexIndices.size());
		m_Normals.Allocate(vertexIndices.size());
		m_Indices.Allocate(vertexIndices.size());

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
        {
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = vertexIndices[i];
            unsigned int normalIndex = normalIndices[i];

            m_Positions[i].Set(tempPositions[vertexIndex - 1]);
            m_TexCoords[i].Set(tempTexCoords[uvIndex - 1]);
            m_Normals[i].Set(tempNormals[normalIndex - 1]);
            m_Colors[i].r = m_Colors[i].g = m_Colors[i].b = m_Colors[i].a = 1.0f;
            m_Indices[i] = i;
        }


        return true;
    }

    bool Mesh::Upload()
    {
        if (!Graphics::LoadMesh(this, m_VBO, m_IBO))
        {
            return false;
        }
        m_AttributeCount = m_Positions.GetCount();
        m_IndexCount = m_Indices.GetCount();
        m_Uploaded = true;
        return true;
    }

    bool Mesh::Release(bool aReleaseCPU)
    {
        if (aReleaseCPU)
        {
            m_Positions.Release();
            m_Colors.Release();
            m_TexCoords.Release();
            m_Normals.Release();
            m_Indices.Release();
        }
        if (!m_Uploaded)
        {
            return false;
        }
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_IBO);
        m_Uploaded = false;
        m_VBO = 0;
        m_IBO = 0;
        m_AttributeCount = 0;
        m_IndexCount = 0;
        return true;
    }

    void Mesh::SetPositions(Array<Vector3> & aPositions)
    {
        Array<Vector3>::Copy(aPositions, m_Positions);
    }
    void Mesh::SetNormals(Array<Vector3> & aNormals)
    {
        Array<Vector3>::Copy(aNormals, m_Normals);
    }
    void Mesh::SetTexCoords(Array<Vector2> & aTexCoords)
    {
        Array<Vector2>::Copy(aTexCoords, m_TexCoords);
    }
    void Mesh::SetColors(Array<Color> & aColors)
    {
        Array<Color>::Copy(aColors, m_Colors);
    }
    void Mesh::SetIndices(Array<UInt16> & aIndicies)
    {
        Array<UInt16>::Copy(aIndicies, m_Indices);
    }

    Array<Vector3> Mesh::GetPositions()
    {
        return m_Positions;
    }
    Array<Vector3> Mesh::GetNormals()
    {
        return m_Normals;
    }
    Array<Vector2> Mesh::GetTexCoords()
    {
        return m_TexCoords;
    }
    Array<Color> Mesh::GetColors()
    {
        return m_Colors;
    }
    Array<UInt16> Mesh::GetIndicies()
    {
        return m_Indices;
    }

    bool Mesh::IsUploaded()
    {
        return m_Uploaded;
    }
    GLuint Mesh::GetVBO()
    {
        return m_VBO;
    }
    GLuint Mesh::GetIBO()
    {
        return m_IBO;
    }
    UInt32 Mesh::GetAttributeCount()
    {
        return m_AttributeCount;
    }
    UInt32 Mesh::GetIndexCount()
    {
        return m_IndexCount;
    }

    
}