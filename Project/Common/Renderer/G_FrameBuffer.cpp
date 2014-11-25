#include "G_FrameBuffer.h"
#include "G_Graphics.h"

namespace Gem
{
	FrameBuffer::FrameBuffer()
	{
		m_FBOHandle = 0;

		m_TextureHandle = 0;
		m_DepthBufferHandle = 0;

		m_Width = 0;
		m_Height = 0;
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteTextures(1, &m_TextureHandle);
		glDeleteTextures(1, &m_DepthBufferHandle);
		//glDeleteRenderbuffers( 1, &m_DepthBufferHandle );
		glDeleteFramebuffers(1, &m_FBOHandle);
	}

	void FrameBuffer::Setup(int width, int height)
	{
		if (m_FBOHandle != 0)
		{
			glDeleteFramebuffers(1, &m_FBOHandle);
		}
		if (m_TextureHandle != 0)
		{
			glDeleteTextures(1, &m_TextureHandle);
		}
		if (m_DepthBufferHandle != 0)
		{
			glDeleteTextures(1, &m_DepthBufferHandle);
		}
			
		int maxsize;
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxsize);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsize);

		m_Width = width;
		m_Height = height;

		glGenFramebuffers(1, &m_FBOHandle);
		assert(m_FBOHandle != 0);

		glGenTextures(1, &m_TextureHandle);
		glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			GL_RGBA, 
			m_Width, m_Height, 
			0,
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenTextures(1, &m_DepthBufferHandle);
		glBindTexture(GL_TEXTURE_2D, m_DepthBufferHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			GL_DEPTH_COMPONENT, 
			m_Width, m_Height, 
			0,
			GL_DEPTH_COMPONENT, 
			GL_UNSIGNED_BYTE, 
			0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOHandle);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureHandle, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,	GL_TEXTURE_2D, m_DepthBufferHandle, 0);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			assert(false);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}