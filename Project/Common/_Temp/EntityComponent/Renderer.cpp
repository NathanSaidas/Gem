#include "Renderer.h"
#include "GameObject.h"

namespace Gem
{
	RDEFINE_CLASS(Renderer, Component)
    RDEFINE_PRIVATE_MEMBER(Renderer,m_Mesh,Pointer<Mesh>)
	RDEFINE_PRIVATE_MEMBER(Renderer, m_Material, Pointer<Material>)

	Renderer::Renderer() : Component()
	{

	}
	Renderer::~Renderer()
	{

	}

	void Renderer::SetMesh(Pointer<Mesh> aMesh)
	{
		m_Mesh = aMesh;
	}
	Pointer<Mesh> Renderer::GetMesh()
	{
		return m_Mesh;
	}

	void Renderer::SetMaterial(Pointer<Material> aMaterial)
	{
		m_Material = aMaterial;
	}
	Pointer<Material> Renderer::GetMaterial()
	{
		return m_Material;
	}


	void Renderer::OnPreRender()
	{
		if (m_Mesh.IsAlive() && m_Material.IsAlive())
		{
			Matrix4x4 modelMatrix = GetGameObject()->GetLocalToWorldMatrix();
			Graphics::Render(modelMatrix, m_Mesh, m_Material);
		}
	}
	void Renderer::OnRender()
	{

	}
	void Renderer::OnPostRender()
	{

	}
}