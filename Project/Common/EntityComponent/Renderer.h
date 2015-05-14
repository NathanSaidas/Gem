#ifndef GEM_RENDERER_H
#define GEM_RENDERER_H

#include "Component.h"
#include "../Graphics/Graphics.h"

namespace Gem
{

	class Renderer;
	template class GEM_API Reflection::MetaObject<Renderer>;

	class GEM_API Renderer : public Component
	{
		RDECLARE_CLASS(Renderer)
	public:
		Renderer();
		~Renderer();

		void SetMesh(Pointer<Mesh> aMesh);
		Pointer<Mesh> GetMesh();

		void SetMaterial(Pointer<Material> aMaterial);
		Pointer<Material> GetMaterial();

	protected:

		virtual void OnPreRender();
		virtual void OnRender();
		virtual void OnPostRender();

	private:
		RDECLARE_PRIVATE_MEMBER(Renderer, m_Mesh);
		Pointer<Mesh> m_Mesh;
        RDECLARE_PRIVATE_MEMBER(Renderer, m_Material);
		Pointer<Material> m_Material;
	};

	TYPE_DEFINE(Renderer)
}

#endif