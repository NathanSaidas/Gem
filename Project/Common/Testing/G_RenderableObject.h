#ifndef G_RENDERABLE_OBJECT_H
#define G_RENDERABLE_OBJECT_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "../Math/G_Math.h"
#include "../Renderer/G_Graphics.h"
#include "../Renderer/G_Camera.h"
#include "../Renderer/G_Mesh.h"
#include "../Renderer/G_Texture.h"
#include "../Renderer/G_Shader.h"
namespace Gem
{
	class RenderableObject : public Object
	{
	public:
		RenderableObject();
		~RenderableObject();
		
		void OnUpdate();
		void OnRender(string & aPass);

		int samples;
		float blurAmountX;
		float blurAmountY;
		float alpha;

		string RenderTag();
		string RenderTag(string & aRenderTag);
		MemoryHandle<Mesh> Mesh();
		MemoryHandle<Texture> Texture();
		MemoryHandle<Shader> Shader();
		MemoryHandle<Gem::Mesh> Mesh(MemoryHandle<Gem::Mesh> & aMesh);
		MemoryHandle<Gem::Texture> Texture(MemoryHandle<Gem::Texture> & aTexture);
		MemoryHandle<Gem::Shader> Shader(MemoryHandle<Gem::Shader> & aShader);
		ClassUtils::Vector3 Position();
		ClassUtils::Vector3 Rotation();
		ClassUtils::Vector3 Scale();
		ClassUtils::Vector3 Position(ClassUtils::Vector3 & aPosition);
		ClassUtils::Vector3 Rotation(ClassUtils::Vector3 & aRotation);
		ClassUtils::Vector3 Scale(ClassUtils::Vector3 & aScale);
		int CullingMask();
		int CullingMask(int aMask);
		bool operator<(RenderableObject const & aOther)
		{
			if (Camera::CurrentCamera() == nullptr)
			{
				return false;
			}
			ClassUtils::Vector3 cameraPos = Camera::CurrentCamera()->Position();
			float distanceA = abs((cameraPos - m_Position).Length());
			float distanceB = abs((cameraPos - aOther.m_Position).Length());
			return distanceA < distanceB;
		}
		void TempRender(Camera * aCamera);
	private:
		void Render();

		string m_RenderTag;
		MemoryHandle<Gem::Mesh> m_Mesh;
		MemoryHandle<Gem::Texture> m_Texture;
		MemoryHandle<Gem::Shader> m_Shader;
		ClassUtils::Vector3 m_Position;
		ClassUtils::Vector3 m_Rotation;
		ClassUtils::Vector3 m_Scale;
		int m_CullingMask;


	};
}
GEM_CLASS(RenderableObject,Object)

#endif