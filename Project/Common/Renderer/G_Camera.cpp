#include "G_Camera.h"
#include "../Testing/G_RenderableObject.h"
#include "../Window/G_WindowManager.h"
#include "G_Mesh.h"
#include "G_VertexInfo.h"
#include "G_Shader.h"
namespace Gem
{ 
    //TestTemplate<Camera> & Camera::cam = TestTemplate<Camera>("Camera","Object");
	Camera * Camera::s_CurrentCamera = nullptr;
	Camera * Camera::CurrentCamera()
	{
		return s_CurrentCamera;
	}
    Camera::Camera()
    {
		m_FrameBuffer = Memory::Instantiate<FrameBuffer>();
		m_CullingMask = 1;
		m_Depth = 0;
		m_ClearDepth = true;

		samples = 10;
		blurDirection = Vector2(1.0f/1024.0f, 1.0f/1024.0f);
    }
    Camera::~Camera()
    {
		m_FrameBuffer = Memory::DestroyHandle<FrameBuffer>(m_FrameBuffer);
    }
	void Camera::SetFBO(float aWidth, float aHeight)
	{
		m_FrameBuffer->Setup(aWidth, aHeight);
	}
	unsigned int Camera::GetRenderTexture()
	{
		return m_FrameBuffer->m_TextureHandle;
	}
	unsigned int Camera::GetDepthTexture()
	{
		return m_FrameBuffer->m_DepthBufferHandle;
	}
    void Camera::Render()
    {
		///Set the current camera for rendering
		s_CurrentCamera = this;

		//Gather the Opaque and Transparent Geometry
		std::vector<RenderableObject*> opaque = Graphics::Instance()->GetOpaque(this);
		std::vector<RenderableObject*> transparent = Graphics::Instance()->GetTransparent(this);
		///Sort transparent objects back to front
		if (transparent.size() > 1)
		{
			std::sort(transparent.front(), transparent.back());
		}
		

		///Bind our current frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer->m_FBOHandle);
		///If clear depth is flagged, clear it
		if (m_ClearDepth == true)
		{
			Graphics::Clear();
		}
		///Adjust the viewport to the frame buffers size
		glViewport(0, 0, m_FrameBuffer->m_Width, m_FrameBuffer->m_Height);
		glFrontFace(GL_CW);
		if (opaque.size() > 0)
		{
			for (std::vector<RenderableObject*>::iterator it = opaque.begin(); it != opaque.end(); ++it)
			{
				(*it)->OnRender(string(RENDER_PASS_FBO));
			}
		}
		if (transparent.size() > 0)
		{
			for (std::vector<RenderableObject*>::iterator it = transparent.begin(); it != transparent.end(); ++it)
			{
				(*it)->OnRender(string(RENDER_PASS_FBO));
			}
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//TODO: Render for PostProcessing
		Graphics::Clear();
		PostProcessRender();
		
		s_CurrentCamera = nullptr;
    }
	string Camera::Name()
	{
		return m_Name;
	}
	int Camera::CullingMask()
	{
		return m_CullingMask;
	}
	int Camera::Depth()
	{
		return m_Depth;
	}
	bool Camera::ClearDepth()
	{
		return m_ClearDepth;
	}
	string Camera::Name(string & aName)
	{
		return m_Name = aName;
	}
	int Camera::CullingMask(int aCullingMask)
	{
		return m_CullingMask = aCullingMask;
	}
	int Camera::Depth(int aDepth)
	{
		return m_Depth = aDepth;
	}
	bool Camera::ClearDepth(bool aFlag)
	{
		return m_ClearDepth = aFlag;
	}

	ClassUtils::Vector3 Camera::Position()
	{
		return m_Position;
	}
	ClassUtils::Vector3 Camera::Orientation()
	{
		return m_Orientation;
	}
	ClassUtils::Vector3 Camera::Target()
	{
		return m_Target;
	}
	ClassUtils::Vector3 Camera::Position(ClassUtils::Vector3 & aPosition)
	{
		return m_Position = aPosition;
	}
	ClassUtils::Vector3 Camera::Orientation(ClassUtils::Vector3 & aRotation)
	{
		return m_Orientation = aRotation;
	}
	ClassUtils::Vector3 Camera::Target(ClassUtils::Vector3 & aScale)
	{
		return m_Target = aScale;
	}
	ClassUtils::Matrix Camera::View()
	{
		ClassUtils::Matrix mat;
		mat.SetIdentity();
		mat.SetLookAt(m_Position, m_Orientation, m_Target);
		return mat;
	}
	ClassUtils::Matrix Camera::Projection()
	{
		return m_Projection;
	}
	void Camera::SetProjection(float aFov, float aAspect, float aNear, float aFar)
	{
		m_Projection.SetPerspective(aFov, aAspect, aNear, aFar);
	}
	void Camera::PostProcessRender()
	{
		Mesh * mesh = Graphics::DefaultMesh();
		Shader * shader = Graphics::DefaultShader();
		unsigned int textureHandle = m_FrameBuffer->m_TextureHandle;


		if (mesh == nullptr
			|| shader == nullptr
			|| textureHandle == 0)
		{
			return;
		}

		if (!mesh->IsStatic()
			|| !shader->IsLoaded())
		{
			return;
		}
		glFrontFace(GL_CCW);
		Vector2 windowSize = WindowManager::Instance()->GetWindowSize(0);

		//ClassUtils::Matrix mvp;
		//mvp.SetIdentity();
		//mvp.TranslatePostRotation(0.0f, 0.0f, -1.0f);
		//ClassUtils::Matrix projection;
		//projection.SetFrustum(-windowSize.x * 0.5f, windowSize.x * 0.5f, -windowSize.y * 0.5f, windowSize.y * 0.5f, 0.1f, 1.0f);
		//mvp.Multiply(&projection);

		ClassUtils::Matrix model;
		model.SetIdentity();
		model.Scale(10.0f, 10.0f, 10.0f);
		ClassUtils::Matrix mvp;
		mvp = model;
		mvp.Multiply(&View());
		mvp.Multiply(&Projection());

		//Bind Shader
		glUseProgram(shader->ProgramHandle());
		//Bind VBO / IBO
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBOHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBOHandle());
		//Bind Texture
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		//Retrieve Attributes
		GLint aPos = glGetAttribLocation(shader->ProgramHandle(), "a_Position");
		GLint aTexCoord = glGetAttribLocation(shader->ProgramHandle(), "a_TexCoord");
		GLint aNormal = glGetAttribLocation(shader->ProgramHandle(), "a_Normal");
		//Retrieve Uniforms.
		GLint uMVP = glGetUniformLocation(shader->ProgramHandle(), "MATRIX_MVP");
		GLint uModel = glGetUniformLocation(shader->ProgramHandle(), "MODEL");
		GLint uMainTexture = glGetUniformLocation(shader->ProgramHandle(), "u_MainTexture");

		if (aPos != -1)
		{
			glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, position));
			glEnableVertexAttribArray(aPos);
		}
		if (aTexCoord != -1)
		{
			glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, texCoord));
			glEnableVertexAttribArray(aTexCoord);
		}
		if (aNormal != -1)
		{
			glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, normal));
			glEnableVertexAttribArray(aNormal);
		}

		if (uMVP != -1)
		{
			glUniformMatrix4fv(uMVP, 1, GL_FALSE, (GLfloat*)&mvp.m11);
		}
		if (uModel != -1)
		{
			glUniformMatrix4fv(uModel, 1, GL_FALSE, (GLfloat*)&model.m11);
		}
		if (uMainTexture != -1)
		{
			glUniform1i(uMainTexture, 0);
		}
		if (shader->ShaderName() == "SimpleBlur")
		{
			GLint uSampleSize = glGetUniformLocation(shader->ProgramHandle(), "u_SampleSize");
			if (uSampleSize != -1)
			{
				glUniform2f(uSampleSize, 5.0f / 1024.0f, 5.0f / 1024.0f);
			}
		}
		if (shader->ShaderName() == "DirectionalBlur")
		{
			GLint uSampleSize = glGetUniformLocation(shader->ProgramHandle(), "u_SampleSize");
			GLint uSamples = glGetUniformLocation(shader->ProgramHandle(), "u_Samples");
			GLint uBlurDirection = glGetUniformLocation(shader->ProgramHandle(), "u_BlurDirection");
			if (uSampleSize != -1)
			{
				glUniform2f(uSampleSize, 1.0f / 1024.0f, 1.0f / 1024.0f);
			}
			if (uSamples != -1)
			{
				glUniform1i(uSamples, samples);
			}
			if (uBlurDirection != -1)
			{
				glUniform2f(uBlurDirection, blurDirection.x, blurDirection.y);
			}
		}

		glDrawElements(GL_TRIANGLES, mesh->UploadedIndexCount(), GL_UNSIGNED_SHORT, 0);
		//Unbind Shader
		glUseProgram(0);
		//Unbind VBO / IBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//Unbind Texture
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}