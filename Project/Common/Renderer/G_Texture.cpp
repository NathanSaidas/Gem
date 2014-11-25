#include "G_Texture.h"
#include "G_Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



namespace Gem
{
	Texture::Texture()
	{
		m_IsUploaded = false;
		m_Handle = 0;
		m_WrapMode = GL_REPEAT;
		m_FilterMode = GL_LINEAR;
		m_ImageFormat = GL_RGB;
		m_Width = 0;
		m_Height = 0;
		m_Data = nullptr;

	}
	Texture::~Texture()
	{
		Free();
	}

	unsigned int Texture::Handle()
	{
		return m_Handle;
	}
	bool Texture::IsUploaded()
	{
		return m_IsUploaded;
	}
	unsigned int Texture::WrapMode()
	{
		return m_WrapMode;
	}
	unsigned int Texture::WrapMode(unsigned int & aWrapMode)
	{
		if (aWrapMode != GL_CLAMP_TO_EDGE
			|| aWrapMode != GL_REPEAT
			|| aWrapMode != GL_MIRRORED_REPEAT
			|| aWrapMode != GL_CLAMP)
		{
			return m_WrapMode;
		}
		return m_WrapMode = aWrapMode;
	}
	unsigned int Texture::FilterMode()
	{
		return m_FilterMode;
	}
	unsigned int Texture::FilterMode(unsigned int & aFilterMode)
	{
		if (aFilterMode != GL_NEAREST
			|| aFilterMode != GL_LINEAR)
		{
			return m_FilterMode;
		}
		return m_FilterMode = aFilterMode;
	}
	int Texture::ImageFormat()
	{
		return m_ImageFormat;
	}
	void Texture::Upload(bool aFree)
	{
		FreeGPU();
		if (m_Data == nullptr)
		{
			return;
		}

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			m_ImageFormat,
			m_Width,
			m_Height,
			0,
			m_ImageFormat,
			GL_UNSIGNED_BYTE,
			m_Data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMode);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (aFree == true)
		{
			FreeCPU();
		}
		m_IsUploaded = true;
	}
	void Texture::FreeCPU()
	{
		if (m_Data != nullptr)
		{
			free(m_Data);
			m_ImageFormat = GL_RGB;
			m_Width = 0;
			m_Height = 0;
			m_Data = nullptr;
		}
	}
	void Texture::FreeGPU()
	{
		if (!m_IsUploaded || m_Handle == 0)
		{
			return;
		}
		glDeleteTextures(1, &m_Handle);
	}
	void Texture::Free()
	{
		FreeGPU();
		FreeCPU();
	}
	void Texture::Load(string & aFilename, int aImageFormat)
	{
		FreeCPU();

		int width, height;
		m_Data = stbi_load(aFilename.C_Str(), &width, &height, &m_ImageFormat, aImageFormat);
		m_Width = width;
		m_Height = height;
		switch (m_ImageFormat)
		{
		case STBI_grey:
			m_ImageFormat = GL_R;
			break;
		case STBI_grey_alpha:
			m_ImageFormat = GL_RG;
			break;
		case STBI_rgb:
			m_ImageFormat = GL_RGB;
			break;
		case STBI_rgb_alpha:
			m_ImageFormat = GL_RGBA;
			break;
		default:
			FreeCPU();
			break;
		}

		
	}

}