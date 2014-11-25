#ifndef G_DUAL_MESH
#define G_DUAL_MESH
#include "G_Mesh.h"
namespace Gem
{
	struct DualVertexInfo;
	class DualMesh : public Mesh
	{
	public:
		DualMesh();

		void Upload(bool aFree = true) override;
		void FreeCPU() override;
		void FreeGPU() override;
		void Load(string & aPathA, string & aPathB);
	private:
		MemoryHandle<DualVertexInfo> m_CustomVertices;
	};
}
GEM_CLASS(DualMesh,Mesh)

#endif