#ifndef GEM_GRAPHICS_H
#define GEM_GRAPHICS_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Graphics which contains a list of rendering functions.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan - Wrapped OpenGL states within the rendering engine.
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
#pragma endregion

#include <glew-1.10.0/include/GL/glew.h>
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Math/Math.h"
#include "Color.h"
#include "GraphicEnums.h"
#include "Mesh.h"
#include "PrimitiveShapeBuffer.h"
#include "Shader.h"
#include "Geometry.h"
#include "DrawCall.h"
#include "RenderTexture.h"
#include "Texture.h"
#include "ImageTexture.h"
#include "Material.h"

#define CONFIG_GRAPHICS_SHADOWMAPPING



namespace Gem
{
	class Camera;
	class Scene;

	//class Graphics;
	//template class GEM_API Reflection::MetaObject<Graphics>;
    //
	//template class GEM_API std::vector<DrawCall>;
	//template class GEM_API std::vector<Camera*>;

    ///This is the manager of graphics operations.
    class Graphics : public object
    {
		RDECLARE_CLASS(Graphics)
    public:
        // -- Creates / Gets the instance of Graphics
        static Graphics * GetInstance();
        // -- Initializes the Graphics system
        static void Initialize();
        // -- Terminates the Graphics system.
        static void Terminate();


		static void Render(Matrix4x4 & aModel, Pointer<Mesh> aMesh, Pointer<Material> aMaterial);
		static void Render(const Matrix4x4 & aModel, const Matrix4x4 & aView, const Matrix4x4 & aProjection, Mesh * aMesh, Material * aMaterial);
		static void RenderImmediate(const Matrix4x4 & aModel, const Matrix4x4 & aView, const Matrix4x4 & aProjection, const Pointer<Mesh> & aMesh, const Pointer<Material> & aMaterial);
		static void RenderImmediate(
			const Matrix4x4 & aModel,				//The model being rendererd
			const Matrix4x4 & aView,				//The view from the camera
			const Matrix4x4 & aProjection,			//The projection from the camera
			const Matrix4x4 & aDepthView,			//The view from the light
			const Matrix4x4 & aDepthProjection,		//The projection from the light
			const Matrix4x4 & aShadowBias,			//The shadow bias matrix
			const Pointer<Mesh> & aMesh,			//The model vbo/ibo being rendered
			const Pointer<Material> & aMaterial);	//The material being used to render.

		static void RenderDepthImmediate(const Matrix4x4 & aModel, const Matrix4x4 & aView, const Matrix4x4 & aProjection, const Pointer<Mesh> & aMesh, const Pointer<Material> & aMaterial);
        // -- Wrapper around OpenGL glUseShader
        static void UseShader(GLuint & aProgramID);
        // -- Wrapped around OpenGL glDetatchShader
        static void DetatchShader(GLuint aProgramID, GLuint aShaderID);
        // -- Wrapper around OpenGL glDeleteShader
        static void DeleteShader(GLuint aShaderID);
        // -- Wrapper around OpenGL glDeleteProgram
        static void DeleteProgram(GLuint aProgramID);
        // -- Wrapper around OpenGL glBindBuffer
        static void BindBuffer(BufferTarget aTarget, GLuint aBuffer);
        // -- Wrapper around OpenGL glBufferData
        static void BufferData(BufferTarget aTarget, GLsizeiptr aSize, GLvoid * aData, BufferMode aMode);
        // -- Creates a shader based on the shader type with the given source. The result is a valid shader handle. Returns false if failed to create shader.
        static bool CompileShader(GLuint & aHandle, const ShaderType & aShaderType, const std::string & aSource);
		// -- Links a shader program.
		static bool LinkShaderProgram(GLuint & aProgram, const GLuint aShaders[], const bool aShadersEnabled[]);
        // -- Links a shader program with the given shaders. (Vertex, Fragment, Geometry) in 
        static BufferTarget GetCurrentBoundTarget();
        // -- Retrieves the current buffer bound.
        static GLuint GetCurrentBoundBuffer();

		static void Clear();
		static void SetBackgroundColor(const Color & aColor);

        // -- Uploads a mesh to OpenGL, use Mesh.Upload instead.
        static bool LoadMesh(Mesh * aMesh, GLuint & aVBO, GLuint & aIBO);

		static bool CheckForGLErrors(const char* file = __FILE__, int line = __LINE__);
    
		/**
		* Sets the default state of the graphics pipeline.
		*/
		static void SetDefaultState();
		/**
		* Sets the faces to cull.
		* @param aCullFace The face to cull
		*/
		static void SetCullFace(CullFace aCullFace);
		/**
		* Sets the winding order for face culling
		* @param aCullMode The winding order for face culling.
		*/
		static void SetCullMode(CullMode aCullMode);
		/**
		* Sets the depth func for depth testing.
		* @param aDepthFunc The function to use for depth testing
		*/
		static void SetDepthFunc(DepthFunc aDepthFunc);
		/**
		* Sets the blend equation for fragment blending
		* @param aSource The source factor for blending
		* @param aDestination The destination factor for blending
		*/
		static void SetBlendFunc(BlendFunc aSource, BlendFunc aDestination);
		/**
		* Enables a rendering state. (Depth Testing, Blending, Culling etc)
		* @param aState The state to enable
		*/
		static void EnableState(GraphicsState aState);
		/**
		* Disables a rendering state. (Depth Testing, Blending, Culling etc)
		* @param aState The state to disable.
		*/
		static void DisableState(GraphicsState aState);
	private:
        // -- Instance of the Graphics singleton
        static Graphics * s_Instance;

        Graphics();
        ~Graphics();
        friend class Gem::Reflection::MetaObject < Graphics > ;

		//These colors should move to camera.
        Color m_BackgroundColor;
        Color m_ForeGroundColor;
		//Current States
		bool m_Cull;
		bool m_Blend;
		bool m_DepthTest;
		CullMode m_CullMode;
		CullFace m_CullFace;
		DepthFunc m_DepthFunc;
		BlendFunc m_BlendSource;
		BlendFunc m_BlendDestination;


        // -- OpenGL
        BufferTarget m_CurrentBoundTarget;
        GLuint m_CurrentBoundBuffer;
        
        // -- Primitive Buffer Targets End.

		// -- Draw Call Stuffs
		std::vector<DrawCall> m_DrawCalls;

		static void EnableVertexAttrib(GLint aLocation, GLint aSize, const GLvoid * aOffset);
		static void DisableVertexAttrib(GLint aLocation);
		static void SetMatrix(GLint aLocation, const Matrix4x4 & aMatrix);
		static void SetFloat(GLint aLocation, Float32 aFloat);
		static void SetTexture(GLint aLocation, GLenum aUnit, Pointer<Texture> aTexture);
        static void SetTexture(GLint aLocation, GLenum aUnit, Pointer<RenderTexture> aTexture);

		friend Camera;


        //-1, 1----1, 1
        //  |       |
        //  |       |
        //-1,-1----1,-1

        //0.0
        //0.0
		Pointer<Shader> m_DebugShader;
        Pointer<Mesh> m_PostProcessMesh;
        Pointer<Material> m_PostProcessMaterial;
		static  Pointer<RenderTexture> GetShadowMap();
    };

	TYPE_DEFINE(Graphics)
}

#endif // GEM_GRAPHICS_H 