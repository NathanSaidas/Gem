#ifndef G_GRAPHICS_H
#define G_GRAPHICS_H

#include "../Primitives/G_PrimitiveObjects.h"
#include <glew-1.10.0\include\GL\glew.h>
#include <GLFW\glfw3.h>
#include <queue>
#include "../Math/G_Math.h"
#include "G_FrameBuffer.h"
#include "G_Color.h"

namespace Gem
{
	class Mesh;
	class Shader;
	class Texture;
	class RenderableObject;
	class Camera;


	enum BlendMode
	{
		ZERO = GL_ZERO,
		ONE = GL_ONE,
		SRC_COLOR = GL_SRC_COLOR,
		DST_COLOR = GL_DST_COLOR,
		SRC_ALPHA = GL_SRC_ALPHA,
		DST_ALPHA = GL_DST_ALPHA,
		CONSTANT_COLOR = GL_CONSTANT_COLOR,
		CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
		ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
		ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
	};

	const string RENDER_TAG_OPAQUE = "Opaque";
	const string RENDER_TAG_TRANSPARENT = "Transparent";
	const string RENDER_PASS_NORMAL = "Normal";
	const string RENDER_PASS_FBO = "FBO";


	///The main class for rendering graphics.
    class Graphics : public Object
    {
    public:
		///Singleton Accessors
		static Graphics * Instance();
		static void Destroy();
		///OpenGL error checking
		static bool CheckForErrors(const char* file = __FILE__, int line = __LINE__);

		static Color BackgroundColor();
		static Color BackgroundColor(Color & aColor);
		static Color BackgroundColor(float aRed, float aGreen, float aBlue);
		static Color ForegroundColor();
		static Color ForegroundColor(Color & aColor);
		static Color ForegroundColor(float aRed, float aGreen, float aBlue);


		static void Init();
		static void EnableBlending(BlendMode aSource = BlendMode::SRC_ALPHA, BlendMode aDestination = BlendMode::ONE_MINUS_SRC_ALPHA);
		static void DisableBlending();
		static void Clear();

		static void RenderObject(RenderableObject * aObject, string & aPass);
		static void Render();
		
		///Adds a camera to the rendering loop.
		static void AddCamera(MemoryHandle<Camera> & aCamera);
		///Retrieves a camera by name from the list of vectors
		static MemoryHandle<Camera> GetCamera(string & aName);
		///Removes a camera from the list by name
		static void RemoveCamera(string & aName);

		static void RenderMesh(ClassUtils::Matrix & aModel, ClassUtils::Matrix & aView, ClassUtils::Matrix & aProjection, Mesh * aMesh, Shader * aShader, Texture * aTexture);
    
		std::vector<RenderableObject*> GetOpaque(Camera * aCamera);
		std::vector<RenderableObject*> GetTransparent(Camera * aCamera);

		static Shader * DefaultShader();
		static Shader * DefaultShader(Shader * aShader);
		static Mesh * DefaultMesh();
		static Mesh * DefaultMesh(Mesh * aMesh);
		static FrameBuffer * FrameBufferObject();
	private:
		///Singleton instance
        static Graphics * s_Instance;
        Graphics();
        ~Graphics();
		Color m_BackgroundColor;
		Color m_ForegroundColor;

		void ClearScreen();
		void DefaultPostProcess();
		// void RenderMesh(Matrix4x4 & aModel, Matrix4x4 & aView, Matrix4x4 & aProjection, Mesh * aMesh, Shader * aShader, Texture * aTexture);

		std::vector<MemoryHandle<Camera>> m_Cameras;
		
		Shader * m_DefaultShader;
		Mesh * m_DefaultMesh;
		MemoryHandle<FrameBuffer> m_FrameBuffer;

		std::vector<RenderableObject*> m_Opaque;
		std::vector<RenderableObject*> m_Transparent;
		///Friends with Memory / Reflection system since singleton design hides constructors.
		friend Allocator;
		friend Reflection::ReflectionFactory<Graphics>;
		friend class Application;
    };
    
}
GEM_CLASS(Graphics, Object)
#endif