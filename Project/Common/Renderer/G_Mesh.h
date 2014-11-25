#ifndef G_MESH_H
#define G_MESH_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
	struct VertexInfo;
	class Mesh : public Object
	{
	public:
		Mesh();
		virtual ~Mesh();
		///Returns the VBOHandle
		unsigned int VBOHandle();
		///Returns the IBOHandle
		unsigned int IBOHandle();
		///Returns whether or not the mesh is static. (Uploaded to the GPU or not).
		bool IsStatic();
		///Returns the amount of vertices contained within the mesh
		int VertexCount();
		///Returns the vertices of the mesh
		MemoryHandle<VertexInfo> Vertices();
		///Returns / Sets the vertices of the mesh
		MemoryHandle<VertexInfo> Vertices(int aVertexCount, MemoryHandle<VertexInfo> & aInfo);
		///Returns the number of indices of the mesh
		int IndexCount();
		///Returns the indices of the mesh
		MemoryHandle<unsigned short> Indices();
		///Returns / Sets the indices of the mesh
		MemoryHandle<unsigned short> Indices(int aIndexCount, MemoryHandle<unsigned short> & aIndices);
		///Uploads vertices to the GPU creating an VBO / IBO.
		///@aFree - If free is true the vertices/indices are destroyed in memory
		virtual void Upload(bool aFree = true);
		///Frees the vertices / indices of the mesh on the CPU
		virtual void FreeCPU();
		///Frees the vertices / indices of the mesh on the GPU
		virtual void FreeGPU();
		///Calls FreeCPU / FreeGPU
		void Free();
		///Loads a mesh from a path. (.Obj)
		void Load(string & aPath);
		void Load(string & aPathA, string & aPathB);
		///The index count of the uploaded mesh
		unsigned short UploadedIndexCount();
	protected:
		///The handle of the VBO for OpenGL
		unsigned int m_VBOHandle;
		///The handle of the IBO for OpenGL
		unsigned int m_IBOHandle;
		///Determines if the mesh is on the GPU or not
		bool m_IsStatic;
		///Vertices of the mesh
		int m_VertexCount;
		MemoryHandle<VertexInfo> m_Vertices;
		///Indices of the mesh
		unsigned short m_IndexCount;
		MemoryHandle<unsigned short> m_Indices;

		unsigned short m_UploadedIndexCount;
	};
}
GEM_CLASS(Mesh,Object)

#endif