#include "ImageTexture.h"
#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage\stb_image.h>

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(ImageTexture,Texture)
	/**
	* Default Constructor of ImageTexture. Sets default values
	*/
	ImageTexture::ImageTexture() : Texture(),
	m_ImageFormat(ImageFormat::RGBA)
	{

	}
	/**
	Destructor of ImageTexture. Releases used resources.
	*/
	ImageTexture::~ImageTexture()
	{
		m_Pixels.Release();
	}

	/**
	* Gets the image format being used.
	* @return Returns the image format being used.
	*/
	ImageFormat ImageTexture::GetImageFormat()
	{
		return m_ImageFormat;
	}
	/**
	* Sets the image format. This has no effect on uploaded textures.
	* @param aFormat The format for the image to use.
	*/
	void ImageTexture::SetImageFormat(ImageFormat aFormat)
	{
		if (!IsUploaded())
		{
			m_ImageFormat = aFormat;
		}
	}

	/**
	* Loads an array of bytes as a texture. Call upload after to use this texture.
	* @param aData The array of bytes to read from
	*/
	void ImageTexture::Load(const Array<UInt8> & aData)
	{
		ReleaseCPU();
		Array<UInt8>::Copy(aData, m_Pixels);
	}
	/**
	* Loads a filename as a texture onto the CPU. Call upload after to use this texture.
	* @param aFilename The filename to load. (This is the full path)
	*/
	void ImageTexture::Load(const std::string & aFilename)
	{
		ReleaseCPU();

		int requestedformat = 0;
		int sourceFormat = 0;
		int width = 0;
		int height = 0;
		int size = 0;
		switch (m_ImageFormat)
		{
		case ImageFormat::RGB:
			requestedformat = STBI_rgb;
			break;
		case ImageFormat::RGBA:
			requestedformat = STBI_rgb_alpha;
			break;
		case ImageFormat::GreyAlpha:
			requestedformat = STBI_grey;
			break;
		case ImageFormat::Greyscale:
			requestedformat = STBI_grey_alpha;
			break;
		default:
			requestedformat = STBI_rgb;
			Debug::Error("Graphics", "Unsupported target image format.");
			break;
		}

		UInt8 * bytes = stbi_load(aFilename.c_str(), &width, &height, &sourceFormat, requestedformat);
		

		SetSize(width, height);
		switch (sourceFormat)
		{
		case STBI_rgb:
			m_ImageFormat = ImageFormat::RGB;
			size = 3;
			break;
		case STBI_rgb_alpha:
			m_ImageFormat = ImageFormat::RGBA;
			size = 4;
			break;
		case STBI_grey_alpha:
			m_ImageFormat = ImageFormat::GreyAlpha;
			size = 2;
			break;
		case STBI_grey:
			m_ImageFormat = ImageFormat::Greyscale;
			size = 1;
			break;
		default:
			Debug::Error("Graphics", "Unsupported loaded image format");
			break;
		}

		m_Pixels.Allocate(width * height * size);
		memcpy(m_Pixels.GetElements(), bytes, m_Pixels.GetCount());
		stbi_image_free(bytes);
	}
	/**
	* Uploads the data to the GPU.
	* @param aReleaseCPU This is true by default but its an option whether or not to upload it right away.
	*/
	void ImageTexture::Upload(bool aReleaseCPU)
	{
		ReleaseGPU();

		if (m_Pixels.GetCount() == 0)
		{
			return;
		}
		if (Graphics::CheckForGLErrors(__FILE__, __LINE__))
		{
			Debug::ErrorFormat("Graphics", nullptr, "Graphics errors exist before uploading Image Texture %s", GetName().c_str());
		}

		GenerateTextureHandle();
		GLuint textureHandle = GetHandle();
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GetWrapMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GetWrapMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GetFilterMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GetFilterMode());

		glTexImage2D(GL_TEXTURE_2D,
			0,
			(GLint)GetImageFormat(),
			GetWidth(),
			GetHeight(),
			0,
			(GLint)GetImageFormat(),
			GL_UNSIGNED_BYTE,
			m_Pixels.GetElements());

		glBindTexture(GL_TEXTURE_2D, 0);


		if (Graphics::CheckForGLErrors(__FILE__, __LINE__))
		{
			Debug::ErrorFormat("Graphics", nullptr, "Graphics errors present after uploading ImageTexture %s", GetName().c_str());
		}

		SetIsUploaded(true);
		if (aReleaseCPU)
		{
			ReleaseCPU();
		}
	}
	void ImageTexture::Upload()
	{
		Upload(true);
	}

	/**
	* Sets the pixels of the texture. Call upload to apply the changes to the texture
	* @param aPixels The new pixels of the texture.
	*/
	void ImageTexture::SetPixels(const Array<Color> & aPixels)
	{
		ReleaseCPU();

		m_Pixels.Allocate(aPixels.GetCount() * 4);

		UInt32 pixelIndex = 0;
		for (UInt32 i = 0; i < aPixels.GetCount(); i++)
		{
			m_Pixels[pixelIndex + 0] = (UInt8)(aPixels[i].r * 255.0f);
			m_Pixels[pixelIndex + 1] = (UInt8)(aPixels[i].g * 255.0f);
			m_Pixels[pixelIndex + 2] = (UInt8)(aPixels[i].b * 255.0f);
			m_Pixels[pixelIndex + 3] = (UInt8)(aPixels[i].a * 255.0f);
			pixelIndex += 4;
		}
		m_ImageFormat = ImageFormat::RGBA;
	}
	/**
	* Gets the pixels of the texture
	* @return Returns the pixels of the texture when uploaded.
	*/
	Array<Color> ImageTexture::GetPixels()
	{
		
		Array<Color> colors;
		switch (m_ImageFormat)
		{
		case ImageFormat::RGBA:
		{
			colors.Allocate(m_Pixels.GetCount() / 4);
			UInt32 pixelIndex = 0;
			for (UInt32 i = 0; i < colors.GetCount(); i++)
			{
				colors[i].r = (Float32)m_Pixels[pixelIndex + 0] / 255.0f;
				colors[i].g = (Float32)m_Pixels[pixelIndex + 1] / 255.0f;
				colors[i].b = (Float32)m_Pixels[pixelIndex + 2] / 255.0f;
				colors[i].a = (Float32)m_Pixels[pixelIndex + 3] / 255.0f;
				pixelIndex += 4;
			}
		}
			break;
		case ImageFormat::RGB:
		{
			colors.Allocate(m_Pixels.GetCount() / 3);
			UInt32 pixelIndex = 0;
			for (UInt32 i = 0; i < colors.GetCount(); i++)
			{
				colors[i].r = (Float32)m_Pixels[pixelIndex + 0] / 255.0f;
				colors[i].g = (Float32)m_Pixels[pixelIndex + 1] / 255.0f;
				colors[i].b = (Float32)m_Pixels[pixelIndex + 2] / 255.0f;
				colors[i].a = 1.0f;
				pixelIndex += 3;
			}
		}
			break;
		case ImageFormat::GreyAlpha:
		{
			colors.Allocate(m_Pixels.GetCount() / 2);
			UInt32 pixelIndex = 0;
			for (UInt32 i = 0; i < colors.GetCount(); i++)
			{
				colors[i].r = (Float32)m_Pixels[pixelIndex + 0] / 255.0f;
				colors[i].g = (Float32)m_Pixels[pixelIndex + 1] / 255.0f;
				colors[i].b = 0.0f;
				colors[i].a = 1.0f;
				pixelIndex += 2;
			}
		}
			break;
		case ImageFormat::Greyscale:
		{
			colors.Allocate(m_Pixels.GetCount() / 1);
			UInt32 pixelIndex = 0;
			for (UInt32 i = 0; i < colors.GetCount(); i++)
			{
				colors[i].r = (Float32)m_Pixels[pixelIndex + 0] / 255.0f;
				colors[i].g = 0.0f;
				colors[i].b = 0.0f;
				colors[i].a = 1.0f;
				pixelIndex += 1;
			}
		}
			break;
		}


		return colors;
	}

	/**
	* Releases resources allocated on the CPU
	*/
	void ImageTexture::ReleaseCPU()
	{
		m_Pixels.Release();
	}
	/**
	* Releases resources allocated on the GPU
	*/
	void ImageTexture::ReleaseGPU()
	{
		if (IsUploaded())
		{
			DeleteTextureHandle();
		}
	}

	void ImageTexture::SaveMeta(IniFileStream & aFileStream)
	{
		Resource::SaveMeta(aFileStream);

		if (!aFileStream.BindSection("ImageTexture"))
		{
			aFileStream.AddSection("ImageTexture");
			aFileStream.BindSection("ImageTexture");
		}

		if (IniInt::IsBadValue(aFileStream.GetInt("FilterMode")))
		{
			aFileStream.AddInt("FilterMode", (int)GetFilterMode());
		}
		else
		{
			aFileStream.SetInt("FilterMode", (int)GetFilterMode());
		}

		if (IniInt::IsBadValue(aFileStream.GetInt("WrapMode")))
		{
			aFileStream.AddInt("WrapMode", (int)GetWrapMode());
		}
		else
		{
			aFileStream.SetInt("WrapMode", (int)GetWrapMode());
		}
	}
	void ImageTexture::LoadMeta(IniFileStream & aFileStream)
	{
		Resource::LoadMeta(aFileStream);
		if (aFileStream.BindSection("ImageTexture"))
		{
			IniInt filterMode = aFileStream.GetInt("FilterMode");
			IniInt wrapMode = aFileStream.GetInt("WrapMode");
			if (!IniInt::IsBadValue(filterMode))
			{
				SetFilterMode((FilterMode)filterMode.GetValue());
			}
			if (!IniInt::IsBadValue(wrapMode))
			{
				SetWrapMode((WrapMode)wrapMode.GetValue());
			}


		}
	}
}