#include "G_Graphics.h"
#include "G_Mesh.h"
#include "G_Shader.h"
#include "G_Texture.h"
#include "G_VertexInfo.h"
#include "G_Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Testing/G_RenderableObject.h"
namespace Gem
{

	Graphics * Graphics::s_Instance = nullptr;
	Graphics * Graphics::Instance()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = Memory::Instantiate<Graphics>();
		}
		return s_Instance;
	}
	void Graphics::Destroy()
	{
		if (s_Instance != nullptr)
		{
			s_Instance = Memory::Destroy<Graphics>(s_Instance);
		}

	}
	Graphics::Graphics()
	{
		m_FrameBuffer = Memory::Instantiate<FrameBuffer>();
	}
	Graphics::~Graphics()
	{
		m_FrameBuffer = Memory::DestroyHandle<FrameBuffer>(m_FrameBuffer);
	}

	bool Graphics::CheckForErrors(const char* file, int line)
	{
		GLenum errorID = GL_NO_ERROR;
		char* errorStr;
		int errorCount = 0;
		while ((errorID = glGetError()) != GL_NO_ERROR)
		{
			errorCount++;

			switch (errorID)
			{
			case GL_INVALID_ENUM:
				errorStr = "GL_INVALID_ENUM";
				break;

			case GL_INVALID_VALUE:
				errorStr = "GL_INVALID_VALUE";
				break;

			case GL_INVALID_OPERATION:
				errorStr = "GL_INVALID_OPERATION";
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;

			case GL_OUT_OF_MEMORY:
				errorStr = "GL_OUT_OF_MEMORY";
				break;

			default:
				errorStr = "Unknown GL message type.";
				break;
			}

			printf("GLError -> %d - %s - %d - %s(%d)\n", errorID, errorStr, errorCount, file, line);

			//assert(false);
			system("pause");
		}
		return(errorCount > 0 ? true : false);
	}
	Color Graphics::BackgroundColor()
	{
		if (s_Instance != nullptr)
		{
			return s_Instance->m_BackgroundColor;
		}
		return Color();
	}
	Color Graphics::BackgroundColor(Color & aColor)
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_BackgroundColor = aColor;
			return s_Instance->m_BackgroundColor;
		}
		return Color();
	}
	Color Graphics::BackgroundColor(float aRed, float aGreen, float aBlue)
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_BackgroundColor.r = aRed;
			s_Instance->m_BackgroundColor.g = aGreen;
			s_Instance->m_BackgroundColor.b = aBlue;
			s_Instance->m_BackgroundColor.a = 1.0f;
			return s_Instance->m_BackgroundColor;
		}
		return Color();
	}
	Color Graphics::ForegroundColor()
	{
		if (s_Instance != nullptr)
		{
			return s_Instance->m_ForegroundColor;
		}
		return Color();
	}
	Color Graphics::ForegroundColor(Color & aColor)
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_ForegroundColor = aColor;
			return s_Instance->m_ForegroundColor;
		}
		return Color();
	}
	Color Graphics::ForegroundColor(float aRed, float aGreen, float aBlue)
	{
		if (s_Instance != nullptr)
		{
			s_Instance->m_ForegroundColor.r = aRed;
			s_Instance->m_ForegroundColor.g = aGreen;
			s_Instance->m_ForegroundColor.b = aBlue;
			
			return s_Instance->m_ForegroundColor;
		}
		return Color();
	}
	void Graphics::Init()
	{
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	void Graphics::EnableBlending(BlendMode aSource, BlendMode aDestination)
	{
		glEnable(GL_BLEND);
		glBlendFunc(aSource, aDestination);
	}
	void Graphics::DisableBlending()
	{
		glDisable(GL_BLEND);
	}
	void Graphics::Clear()
	{
		Instance()->ClearScreen();
	}
	void Graphics::RenderObject(RenderableObject * aObject, string & aPass)
	{
		if (aObject != nullptr)
		{
			if (aObject->RenderTag() == RENDER_TAG_OPAQUE)
			{
				Instance()->m_Opaque.push_back(aObject);
			}
			else if (aObject->RenderTag() == RENDER_TAG_TRANSPARENT)
			{
				Instance()->m_Transparent.push_back(aObject);
			}
		}
	}
	void Graphics::Render()
	{
		for (std::vector<MemoryHandle<Camera>>::iterator it = Instance()->m_Cameras.begin();
			it != Instance()->m_Cameras.end(); ++it)
		{
			(*it)->Render();
		}
		Instance()->m_Opaque.clear();
		Instance()->m_Transparent.clear();
	}
	void Graphics::AddCamera(MemoryHandle<Camera> & aCamera)
	{
		Instance()->m_Cameras.push_back(aCamera);
	}
	MemoryHandle<Camera> Graphics::GetCamera(string & aName)
	{
		for (std::vector<MemoryHandle<Camera>>::iterator it = Instance()->m_Cameras.begin();
			it != Instance()->m_Cameras.end(); ++it)
		{
			if ((*it)->Name() == aName)
			{
				return (*it);
			}
		}
		return MemoryHandle<Camera>(Memory::HANDLE_INVALID_MEMORY, nullptr);
	}
	void Graphics::RemoveCamera(string & aName)
	{
		for (int i = 0; i < Instance()->m_Cameras.size(); i++)
		{
			if (Instance()->m_Cameras[i]->Name() == aName)
			{
				Instance()->m_Cameras.erase(Instance()->m_Cameras.begin() + i);
				break;
			}
		}
	}
	void Graphics::ClearScreen()
	{
		glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Graphics::RenderMesh(ClassUtils::Matrix & aModel, ClassUtils::Matrix & aView, ClassUtils::Matrix & aProjection, Mesh * aMesh, Shader * aShader, Texture * aTexture)
	{
		if (aMesh == nullptr || aShader == nullptr || aTexture == nullptr)
		{
			return;
		}
		if (aMesh->IsStatic() == false || aShader->IsLoaded() == false || aTexture->IsUploaded() == false)
		{
			return;
		}

		ClassUtils::Matrix mvp;
		mvp = aModel;
		mvp.Multiply(&aView);
		mvp.Multiply(&aProjection);

		int shader = aShader->ProgramHandle();
		glUseProgram(aShader->ProgramHandle());

		GLint aPos = glGetAttribLocation(shader, "a_Position");
		GLint aColor = glGetAttribLocation(shader, "a_Color");
		GLint aTexCoord = glGetAttribLocation(shader, "a_TexCoord");
		GLint aNormal = glGetAttribLocation(shader, "a_Normal");
		GLint uMVP = glGetUniformLocation(shader, "MATRIX_MVP");
		GLint uMainTexture = glGetUniformLocation(shader, "u_MainTexture");

		glBindBuffer(GL_ARRAY_BUFFER, aMesh->VBOHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aMesh->IBOHandle());

		CheckForErrors(__FILE__,__LINE__);

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
		if (aColor != -1)
		{
			glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, color));
			glEnableVertexAttribArray(aColor);
		}
		if (aNormal != -1)
		{
			//glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, normal));
			//glEnableVertexAttribArray(aNormal);
		}
		CheckForErrors(__FILE__, __LINE__);
		if (uMainTexture != -1)
		{
			glBindTexture(GL_TEXTURE_2D, aTexture->Handle());
			glUniform1i(uMainTexture, 0);
		}

		if (uMVP != -1)
		{
			//glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mvp));
			glUniformMatrix4fv(uMVP, 1, GL_FALSE, (GLfloat*)&mvp.m11);
		}
		CheckForErrors(__FILE__, __LINE__);
		glDrawElements(GL_TRIANGLES, aMesh->UploadedIndexCount(), GL_UNSIGNED_SHORT, 0);

		if (aPos != -1)
		{
			glDisableVertexAttribArray(aPos);
		}
		if (aTexCoord != -1)
		{
			glDisableVertexAttribArray(aTexCoord);
		}
		if (aColor != -1)
		{
			glDisableVertexAttribArray(aColor);
		}
		if (aNormal != -1)
		{
			glDisableVertexAttribArray(aNormal);
		}
		CheckForErrors(__FILE__, __LINE__);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
		CheckForErrors(__FILE__, __LINE__);
	}

	std::vector<RenderableObject*> Graphics::GetOpaque(Camera * aCamera)
	{
		std::vector<RenderableObject*> objects;
		if (aCamera != nullptr)
		{
			int cullingmask = aCamera->CullingMask();
			for (std::vector<RenderableObject*>::iterator it = m_Opaque.begin();
				it != m_Opaque.end(); ++it)
			{
				if ((*it)->RenderTag() == RENDER_TAG_OPAQUE && 
					((*it)->CullingMask() & cullingmask) == cullingmask)
				{
					objects.push_back((*it));
				}
			}
		}

		return objects;
	}
	std::vector<RenderableObject*> Graphics::GetTransparent(Camera * aCamera)
	{
		std::vector<RenderableObject*> objects;
		if (aCamera != nullptr)
		{
			int cullingmask = aCamera->CullingMask();
			for (std::vector<RenderableObject*>::iterator it = m_Opaque.begin();
				it != m_Opaque.end(); ++it)
			{
				if ((*it)->RenderTag() == RENDER_TAG_TRANSPARENT && 
					((*it)->CullingMask() & cullingmask) == cullingmask)
				{
					objects.push_back((*it));
				}
			}
		}
		return objects;
	}

	Shader * Graphics::DefaultShader()
	{
		return Instance()->m_DefaultShader;
	}
	Shader * Graphics::DefaultShader(Shader * aShader)
	{
		return Instance()->m_DefaultShader = aShader;
	}
	Mesh * Graphics::DefaultMesh()
	{
		return Instance()->m_DefaultMesh;
	}
	Mesh * Graphics::DefaultMesh(Mesh * aMesh)
	{
		return Instance()->m_DefaultMesh = aMesh;
	}
	FrameBuffer * Graphics::FrameBufferObject()
	{
		return Instance()->m_FrameBuffer;
	}

	void DefaultPostProcess()
	{

	}
		
}