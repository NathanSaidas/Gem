#ifndef GEM_SHADER_H
#define GEM_SHADER_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class Shader used in Graphics.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan - Added IsUploaded method to check if the shader is uploaded to the GPU
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
#pragma endregion

#include <glew-1.10.0/include/GL\glew.h>
#include "../Core/BasicTypes.h"

#include "GraphicEnums.h"


namespace Gem
{

    //FORCE_EXPORT_META(Shader);
    class Shader : public Object
    {
		RDECLARE_CLASS(Shader)
    public:
        ///Sets the shader to default values.
        Shader();
        ~Shader();

        //Loads the shader file
        void LoadFile(const std::string & aFilename);
        //Loads the loaded shader source into opengl/direct3D
        void LoadProgram();
        //Loads the shader file then loads the shader source into opengl/direct3D
        void Load(const std::string & aFilename);
        //Release the shader file from memory
        void ReleaseFile();
        //Release the shader program from memory
        void ReleaseProgram();
        //Releases the shader file and program from memory
        void Release();
           
        //Tells OpenGL to use this shader
        bool UseShader();
		/**
		* Determines if the shader is uploaded or not.
		* @return Returns true if the shader has been uploaded returns false otherwise.
		*/
		bool IsUploaded();
        //Retrieve an attributes location from the shader. Call UseShader before calling this.
        GLint GetAttributeLocation(const std::string & aName);
        //Retrieve a uniforms location from the shader. Call UseShader before calling this.
        GLint GetUniformLocation(const std::string & aName);

    private:
        std::string m_VertexShaderSource;
        std::string m_FragmentShaderSource;
        std::string m_GeometryShaderSource;

        bool m_VertexShaderLoaded;
        bool m_FragmentShaderLoaded;
        bool m_GeometryShaderLoaded;
        bool m_ShaderHandleLoaded;

        GLuint m_VertexShader;
        GLuint m_FragmentShader;
        GLuint m_GeometryShader;
        GLuint m_ShaderHandle;

        static bool CompileShader(GLuint & aHandle, const ShaderType& aShaderType, const std::string aSource);
        static bool LinkShaderProgram(GLuint & aProgram, const GLuint aShaders[], const bool aShadersEnabled[]);
    };

	TYPE_DEFINE(Shader)
}

#endif // GEM_SHADER_H