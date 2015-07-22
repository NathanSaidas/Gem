#include "RenderTexture.h"
#include "Graphics.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(RenderTexture, Texture)

	RenderTexture::RenderTexture()
		: m_DepthTextureHandle(0)
	{

	}

	RenderTexture::~RenderTexture()
	{
		Release();
	}

	void RenderTexture::Create(UInt32 aWidth, UInt32 aHeight)
	{
		Release();
		SetSize(aWidth, aHeight);
		Graphics::CheckForGLErrors(__FILE__, __LINE__);
		//Create Framebuffer
		
		glGenFramebuffers(1, &m_FBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBOHandle);

		//Create Color Texture
		GenerateTextureHandle();
		GLuint textureHandle = GetHandle();
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GetFilterMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GetFilterMode()); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GetWrapMode()); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GetWrapMode());
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RGB, 
			GetWidth(), GetHeight(),
			0,
			GL_RGB, 
			GL_UNSIGNED_BYTE, 
			0);
		glBindTexture(GL_TEXTURE_2D, 0);
		Graphics::CheckForGLErrors(__FILE__, __LINE__);
		//Generate Depth Texture
        glGenTextures(1, &m_DepthTextureHandle);
		glBindTexture(GL_TEXTURE_2D, m_DepthTextureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GetFilterMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GetFilterMode());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GetWrapMode()); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GetWrapMode()); 
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_DEPTH_COMPONENT24, 
			GetWidth(), GetHeight(),
			0,
			GL_DEPTH_COMPONENT, 
			GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		Graphics::CheckForGLErrors(__FILE__, __LINE__);

		//Attach texture handles to the frame buffer object.
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTextureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Debug::Error("Graphics", "Failed to create Render Texture");
			SetIsUploaded(true);
            Release();
		}
		else
		{
			SetIsUploaded(true);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Graphics::CheckForGLErrors(__FILE__, __LINE__);
		

	}
	void RenderTexture::ReleaseGPU()
	{
		if (IsUploaded())
		{
			DeleteTextureHandle();
			glDeleteTextures(1, &m_DepthTextureHandle);
			glDeleteFramebuffers(1, &m_FBOHandle);
			m_DepthTextureHandle = 0;
			m_FBOHandle = 0;
			SetSize(0, 0);
			SetIsUploaded(false);
		}
	}

	GLuint RenderTexture::GetFBOHandle()
	{
		return m_FBOHandle;
	}
	GLuint RenderTexture::GetDepthHandle()
	{
		return m_DepthTextureHandle;
	}

}