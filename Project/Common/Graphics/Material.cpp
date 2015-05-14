#include "Material.h"

namespace Gem
{
	RDEFINE_CLASS(Material, Resource)
	Material::Material() : Resource()
	{
		SetDefaultState();
	}
	Material::~Material()
	{

	}

	void Material::SetShader(Pointer<Shader> & aShader)
	{
		m_Shader = aShader;
	}
	Pointer<Shader> Material::GetShader()
	{
		return m_Shader;
	}

	void Material::SetTexture(Pointer<Texture> & aTexture)
	{
		m_Texture = aTexture;
	}
	Pointer<Texture> Material::GetTexture()
	{
		return m_Texture;
	}
	void Material::SetDefaultState()
	{
		m_Cull = true;
		m_Blend = true;
		m_DepthTest = true;
		m_CullMode = CullMode::Back;
		m_CullFace = CullFace::ClockWise;
		m_DepthFunc = DepthFunc::Less;
		m_BlendSource = BlendFunc::SrcAlpha;
		m_BlendSource = BlendFunc::OneMinusSrcAlpha;
	}
	void Material::SetCullFace(CullFace aCullFace)
	{
		m_CullFace = aCullFace;
	}
	void Material::SetCullMode(CullMode aCullMode)
	{
		m_CullMode = aCullMode;
	}
	void Material::SetDepthFunc(DepthFunc aDepthFunc)
	{
		m_DepthFunc = aDepthFunc;
	}
	void Material::SetBlendFunc(BlendFunc aSource, BlendFunc aDestination)
	{
		m_BlendSource = aSource;
		m_BlendDestination = aDestination;
	}
	void Material::EnableState(GraphicsState aState)
	{
		switch (aState)
		{
		case GraphicsState::CullFace:
			m_Cull = true;
			break;
		case GraphicsState::Blending:
			m_Blend = true;
			break;
		case GraphicsState::DepthTesting:
			m_DepthTest = true;
			break;
		}
	}
	void Material::DisableState(GraphicsState aState)
	{
		switch (aState)
		{
		case GraphicsState::CullFace:
			m_Cull = false;
			break;
		case GraphicsState::Blending:
			m_Blend = false;
			break;
		case GraphicsState::DepthTesting:
			m_DepthTest = false;
			break;
		}
	}
	bool Material::GetState(GraphicsState aState)
	{
		switch (aState)
		{
		case GraphicsState::CullFace:
			return m_Cull;
		case GraphicsState::Blending:
			return m_Blend;
		case GraphicsState::DepthTesting:
			return m_DepthTest;
		}
		return false;
	}
	CullMode Material::GetCullMode()
	{
		return m_CullMode;
	}
	CullFace Material::GetCullFace()
	{
		return m_CullFace;
	}
	DepthFunc Material::GetDepthFunc()
	{
		return m_DepthFunc;
	}
	BlendFunc Material::GetBlendSource()
	{
		return m_BlendSource;
	}
	BlendFunc Material::GetBlendDestination()
	{
		return m_BlendDestination;
	}


}