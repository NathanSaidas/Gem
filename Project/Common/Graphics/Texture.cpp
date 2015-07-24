#include "Texture.h"
#include "Graphics.h"

namespace Gem
{
	RDEFINE_CLASS(Texture, Resource)
	Texture::Texture() : Resource()
	{
		m_IsUploaded = false;
		m_TextureHandle = 0;
		m_WrapMode = WrapMode::Repeat;
		m_FilterMode = FilterMode::Linear;
		m_Width = 0;
		m_Height = 0;

	}
	Texture::~Texture()
	{
		Release();
	}

	/**
	* Gets the filter mode last set.
	* @return The filter mode being used.
	*/
	FilterMode Texture::GetFilterMode()
	{
		return m_FilterMode;
	}
	/**
	* Sets the filter mode. Note that this has no effect if the texture is uploaded
	* @param aMode The filter mode to use.
	*/
	void Texture::SetFilterMode(FilterMode aMode)
	{
		if (!IsUploaded())
		{
			m_FilterMode = aMode;
		}
	}

	/**
	* Gets the wrap mode last set.
	* @return Returns the wrap mode being used.
	*/
	WrapMode Texture::GetWrapMode()
	{
		return m_WrapMode;
	}
	/**
	* Sets the wrap mode. Note that this has no effect if the texture is uploaded.
	* @param aMode The wrap mode to use.
	*/
	void Texture::SetWrapMode(WrapMode aMode)
	{
		if (!IsUploaded())
		{
			m_WrapMode = aMode;
		}
	}

	/**
	* Gets the width of the texture
	* @return Returns the height of the texture.
	*/
	UInt32 Texture::GetWidth()
	{
		return m_Width;
	}
	/**
	* Gets the height of the texture
	* @return Returns the height of the texture.
	*/
	UInt32 Texture::GetHeight()
	{
		return m_Height;
	}
	/**
	* Gets a raw handle to the texture from OpenGL
	* Use this wil caution. Do not deallocate the handle as its handled internally.
	* @return Returns a raw handle to the texture from OpenGL
	*/
	unsigned int Texture::GetHandle()
	{
		return m_TextureHandle;
	}
	/**
	* Releases resources allocated on both the CPU and the GPU
	*/
	void Texture::Release()
	{
		ReleaseCPU();
		ReleaseGPU();
		SetSize(0, 0);
	}
	/**
	* Releases resources allocated on the CPU
	*/
	void Texture::ReleaseCPU()
	{

	}
	/**
	* Releases resources allocated on the GPU
	*/
	void Texture::ReleaseGPU()
	{

	}
	/**
	* Determines if the texture is uploaded or not and can be used with rendering.
	* @return Returns true if the texture is uploaded to the GPU. Returns false otherwise.
	*/
	bool Texture::IsUploaded()
	{
		return m_IsUploaded;
	}

	/**
	* Sets whether or not the texture is uploaded.
	* @param aValue Whether or not the texture is uploaded or not.
	*/
	void Texture::SetIsUploaded(bool aValue)
	{
		m_IsUploaded = aValue;
	}

	void Texture::SetSize(UInt32 aWidth, UInt32 aHeight)
	{
		m_Width = aWidth;
		m_Height = aHeight;
	}

	/**
	* Generates a texture handle.
	*/
	void Texture::GenerateTextureHandle()
	{
		if (IsUploaded())
		{
			ReleaseGPU();
		}
		glGenTextures(1, &m_TextureHandle);
	}
	/**
	* Deletes a texture handle;
	*/
	void Texture::DeleteTextureHandle()
	{
		if (m_TextureHandle != 0)
		{
			glDeleteTextures(1, &m_TextureHandle);
			m_TextureHandle = 0;
		}
	}

}