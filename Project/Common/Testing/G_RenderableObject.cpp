#include "G_RenderableObject.h"
#include "../Renderer/G_VertexInfo.h"

#include <glm\common.hpp>
#include <glm\gtc\matrix_transform.hpp>


namespace Gem
{

	RenderableObject::RenderableObject()
	{
		blurAmountX = 0.016f;
		blurAmountY = 0.016f;
		samples = 32;
		alpha = 1.0f;
		m_CullingMask = 1;
	}
	RenderableObject::~RenderableObject()
	{

	}
	void RenderableObject::OnUpdate()
	{
		
	}
	void RenderableObject::OnRender(string & aPass)
	{
		Render();
	}
	string RenderableObject::RenderTag()
	{
		return m_RenderTag;
	}
	string RenderableObject::RenderTag(string & aRenderTag)
	{
		m_RenderTag.Set(aRenderTag);
		return m_RenderTag;
	}
	MemoryHandle<Mesh> RenderableObject::Mesh()
	{
		return m_Mesh;
	}
	MemoryHandle<Texture> RenderableObject::Texture()
	{
		return m_Texture;
	}
	MemoryHandle<Shader> RenderableObject::Shader()
	{
		return m_Shader;
	}
	MemoryHandle<Gem::Mesh> RenderableObject::Mesh(MemoryHandle<Gem::Mesh> & aMesh)
	{
		return m_Mesh = aMesh;
	}
	MemoryHandle<Gem::Texture> RenderableObject::Texture(MemoryHandle<Gem::Texture> & aTexture)
	{
		return m_Texture = aTexture;
	}
	MemoryHandle<Gem::Shader> RenderableObject::Shader(MemoryHandle<Gem::Shader> & aShader)
	{
		return m_Shader = aShader;
	}
	ClassUtils::Vector3 RenderableObject::Position()
	{
		return m_Position;
	}
	ClassUtils::Vector3 RenderableObject::Rotation()
	{
		return m_Rotation;
	}
	ClassUtils::Vector3 RenderableObject::Scale()
	{
		return m_Scale;
	}
	ClassUtils::Vector3 RenderableObject::Position(ClassUtils::Vector3 & aPosition)
	{
		return m_Position = aPosition;
	}
	ClassUtils::Vector3 RenderableObject::Rotation(ClassUtils::Vector3 & aRotation)
	{
		return m_Rotation = aRotation;
	}
	ClassUtils::Vector3 RenderableObject::Scale(ClassUtils::Vector3 & aScale)
	{
		return m_Scale = aScale;
	}
	int RenderableObject::CullingMask()
	{
		return m_CullingMask;
	}
	int RenderableObject::CullingMask(int aMask)
	{
		return m_CullingMask = aMask;
	}

	void RenderableObject::Render()
	{
		//Check if the renderable object is valid for rendering.
		Gem::Shader * shader = m_Shader;
		Gem::Mesh * mesh = m_Mesh;
		Gem::Texture * texture = m_Texture;
		Camera * camera = Camera::CurrentCamera();
		
		if (shader == nullptr
			|| mesh == nullptr
			|| texture == nullptr
			|| camera == nullptr)
		{
			return;
		}
		if (!shader->IsLoaded()
			|| !mesh->IsStatic()
			|| !texture->IsUploaded())
		{
			return;
		}
		//Create the model matrix
		ClassUtils::Matrix model;
		model.SetIdentity();
		model.Scale(m_Scale.x, m_Scale.y, m_Scale.z);
		model.Rotate(m_Rotation.x, 1.0f, 0.0f, 0.0f);
		model.Rotate(m_Rotation.y, 0.0f, 1.0f, 0.0f);
		model.Rotate(m_Rotation.z, 0.0f, 0.0f, 1.0f);
		model.TranslatePostRotation(m_Position.x, m_Position.y, m_Position.z);
		ClassUtils::Matrix mvp;
		//Create MVP Matrix
		mvp = model;
		mvp.Multiply(&camera->View());
		mvp.Multiply(&camera->Projection());

		//Bind Shader
		glUseProgram(shader->ProgramHandle());
		//Bind VBO / IBO
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBOHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBOHandle());
		//Bind Texture
		glBindTexture(GL_TEXTURE_2D, texture->Handle());

		//Retrieve Attributes
		GLint aPos = glGetAttribLocation(shader->ProgramHandle(), "a_Position");
		GLint aTexCoord = glGetAttribLocation(shader->ProgramHandle(), "a_TexCoord");
		GLint aNormal = glGetAttribLocation(shader->ProgramHandle(), "a_Normal");
		//Retrieve Uniforms.
		GLint uMVP = glGetUniformLocation(shader->ProgramHandle(), "MATRIX_MVP");
		GLint uModel = glGetUniformLocation(shader->ProgramHandle(), "MODEL");
		GLint uMainTexture = glGetUniformLocation(shader->ProgramHandle(), "u_MainTexture");
		
		GLint uAlpha = glGetUniformLocation(shader->ProgramHandle(), "u_Alpha");
		

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
		if (uAlpha != -1)
		{
			glUniform1f(uAlpha, 1.0f);
		}
		if (shader->ShaderName() == "SimpleBlur")
		{
			GLint uSampleSize = glGetUniformLocation(shader->ProgramHandle(), "u_SampleSize");
			if (uSampleSize != -1)
			{
				glUniform2f(uSampleSize, 1.0f / 1024.0f, 1.0f / 1024.0f);
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

	void RenderableObject::TempRender(Camera * aCamera)
	{
		//Check if the renderable object is valid for rendering.
		Gem::Shader * shader = m_Shader;
		Gem::Mesh * mesh = m_Mesh;
		Gem::Texture * texture = m_Texture;
		Camera * camera = aCamera;
		
		if (shader == nullptr
			|| mesh == nullptr
			|| texture == nullptr
			|| camera == nullptr)
		{
			return;
		}
		if (!shader->IsLoaded()
			|| !mesh->IsStatic()
			|| !texture->IsUploaded())
		{
			return;
		}
		//Create the model matrix
		ClassUtils::Matrix model;
		model.SetIdentity();
		model.Scale(m_Scale.x, m_Scale.y, m_Scale.z);
		model.Rotate(m_Rotation.x, 1.0f, 0.0f, 0.0f);
		model.Rotate(m_Rotation.y, 0.0f, 1.0f, 0.0f);
		model.Rotate(m_Rotation.z, 0.0f, 0.0f, 1.0f);
		model.TranslatePostRotation(m_Position.x, m_Position.y, m_Position.z);
		ClassUtils::Matrix mvp;
		//Create MVP Matrix
		mvp = model;
		mvp.Multiply(&camera->View());
		mvp.Multiply(&camera->Projection());

		//Bind Shader
		glUseProgram(shader->ProgramHandle());
		//Bind VBO / IBO
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBOHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBOHandle());
		//Bind Texture
		glBindTexture(GL_TEXTURE_2D, texture->Handle());

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