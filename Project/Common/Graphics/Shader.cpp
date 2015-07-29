#include "Shader.h"
#include <fstream>
#include "../Utilities/Utilities.h"

using namespace Gem::Debugging;

namespace Gem
{
	RDEFINE_CLASS(Shader, Object)
	Shader::Shader() : Object(),
		m_FragmentShader(0),
		m_FragmentShaderLoaded(false),
		m_VertexShader(0),
		m_VertexShaderLoaded(false),
		m_GeometryShader(0),
		m_GeometryShaderLoaded(false),
		m_ShaderHandle(0),
		m_ShaderHandleLoaded(false)
    {
    }
    Shader::~Shader()
    {
        Release();
    }

    //Loads the shader file
    void Shader::LoadFile(const std::string & aFilename)
    {
        std::ifstream fileStream;
        fileStream.open(aFilename);
        if (!fileStream.is_open())
        {
			Debug::ErrorFormat("Graphics", nullptr, "Failed to load in file %s", aFilename.c_str());
            return;
        }
        std::string shaderSource;
        std::string line;
        ShaderType shaderType;
        bool buildingShaderSource = false;
        std::vector<std::string> words;
        while (std::getline(fileStream, line))
        {
            if (line.size() == 0)
            {
                continue;
            }
            words.clear();
            Utilities::GetWords(line, words);
            if (words.size() > 0)
            {
                if (words[0] == "#implement" && words.size() > 1)
                {
                    if (buildingShaderSource)
                    {
                        switch (shaderType)
                        {
                            case ShaderType::Vertex:
                                m_VertexShaderSource = shaderSource;
                                break;
                            case ShaderType::Fragment:
                                m_FragmentShaderSource = shaderSource;
                                break;
                            case ShaderType::Geometry:
                                m_GeometryShaderSource = shaderSource;
                                break;
                        }
                        buildingShaderSource = false;
                        shaderSource.clear();
                    }
                    if (words[1] == "vertex")
                    {
                        shaderType = ShaderType::Vertex;
                        buildingShaderSource = true;
                    }
                    else if (words[1] == "fragment")
                    {
                        shaderType = ShaderType::Fragment;
                        buildingShaderSource = true;
                    }
                    else if (words[1] == "geometry")
                    {
                        shaderType = ShaderType::Geometry;
                        buildingShaderSource = true;
                    }
                }
                else if (words[0] == "#end")
                {
                    if (buildingShaderSource)
                    {
                        switch (shaderType)
                        {
                        case ShaderType::Vertex:
                            m_VertexShaderSource = shaderSource;
                            break;
                        case ShaderType::Fragment:
                            m_FragmentShaderSource = shaderSource;
                            break;
                        case ShaderType::Geometry:
                            m_GeometryShaderSource = shaderSource;
                            break;
                        }
                        buildingShaderSource = false;
                        shaderSource.clear();
                    }
                }
                else
                {
                    shaderSource.append(line).append("\n");
                }
            }
        }

        fileStream.close();

    }
    //Loads the loaded shader source into opengl/direct3D
    void Shader::LoadProgram()
    {
		//Check for Pre-existing errors.
		//Graphics::CheckForGLErrors(__FILE__, __LINE__);
        ReleaseProgram();

        if (m_VertexShaderSource.size() != 0)
        {
            m_VertexShaderLoaded = CompileShader(m_VertexShader, ShaderType::Vertex, m_VertexShaderSource);
			//Graphics::CheckForGLErrors(__FILE__, __LINE__);
        }
        if (m_FragmentShaderSource.size() != 0)
        {
            m_FragmentShaderLoaded = CompileShader(m_FragmentShader, ShaderType::Fragment, m_FragmentShaderSource);
			//Graphics::CheckForGLErrors(__FILE__, __LINE__);
        }
        if (m_GeometryShaderSource.size() != 0)
        {
            m_GeometryShaderLoaded = CompileShader(m_GeometryShader, ShaderType::Geometry, m_GeometryShaderSource);
			//Graphics::CheckForGLErrors(__FILE__, __LINE__);
        }

		//Order Matters, Vertex => Fragment => Geometry
        GLuint shaders[] = { m_VertexShader, m_FragmentShader, m_GeometryShader };
        bool shadersLoaded[] = { m_VertexShaderLoaded, m_FragmentShaderLoaded, m_GeometryShaderLoaded };

        if (!LinkShaderProgram(m_ShaderHandle, shaders, shadersLoaded))
        {
            ReleaseProgram();
			Debug::ErrorFormat("Graphics", nullptr, "Error loading shader %s.", GetName().c_str());
        }
		else
		{
			//Graphics::CheckForGLErrors(__FILE__, __LINE__);
			m_ShaderHandleLoaded = true;

			//TODO(Nathan): Extract Shader Variables
			//glUseProgram(m_ShaderHandle);
			//glBindFragDataLocation(m_ShaderHandle, 0, "v_FragColor");
			//glUseProgram(0);
			//Graphics::CheckForGLErrors(__FILE__, __LINE__);
		}
        //Else Successful Shader Compile

		


        
    } 
    //Loads the shader file then loads the shader source into opengl/direct3D
    void Shader::Load(const std::string & aFilename)
    {
        LoadFile(aFilename);
        LoadProgram();
    }

    void Shader::ReleaseFile()
    {
        m_VertexShaderSource.clear();
        m_FragmentShaderSource.clear();
        m_GeometryShaderSource.clear();
    }
    void Shader::ReleaseProgram()
    {
        //Clean Up
        if (m_VertexShaderLoaded)
        {
            if (m_ShaderHandleLoaded)
            {
                glDetachShader(m_ShaderHandle, m_VertexShader);
            }
            glDeleteShader(m_VertexShader);
            m_VertexShaderLoaded = false;
            m_VertexShader = 0;
        }
        if (m_FragmentShaderLoaded)
        {
            if (m_ShaderHandleLoaded)
            {
                glDetachShader(m_ShaderHandle, m_FragmentShader);
            }
            glDeleteShader(m_FragmentShader);
            m_FragmentShaderLoaded = false;
            m_FragmentShader = 0;
        }
        if (m_GeometryShaderLoaded)
        {
            if (m_ShaderHandleLoaded)
            {
                glDetachShader(m_ShaderHandle, m_GeometryShader);
            }
            glDeleteShader(m_GeometryShader);
            m_GeometryShaderLoaded = false;
            m_GeometryShader = 0;
        }
        if (m_ShaderHandleLoaded)
        {
            glDeleteProgram(m_ShaderHandle);
            m_ShaderHandleLoaded = false;
            m_ShaderHandle = 0;
        }
    }
    void Shader::Release()
    {
        ReleaseFile();
        ReleaseProgram();
    }

    bool Shader::UseShader()
    {
        if (m_ShaderHandleLoaded)
        {
            glUseProgram(m_ShaderHandle);
            return true;
        }
        return false;
    }
	/**
	* Determines if the shader is uploaded or not.
	* @return Returns true if the shader has been uploaded returns false otherwise.
	*/
	bool Shader::IsUploaded()
	{
		return m_ShaderHandleLoaded;
	}
    GLint Shader::GetAttributeLocation(const std::string & aName)
    {
        return glGetAttribLocation(m_ShaderHandle, aName.c_str());
    }
    GLint Shader::GetUniformLocation(const std::string & aName)
    {
        return glGetUniformLocation(m_ShaderHandle, aName.c_str());
    }

    bool Shader::CompileShader(GLuint & aHandle, const ShaderType& aShaderType, const std::string aSource)
    {
        std::string shaderTypename;
        switch (aShaderType)
        {
        case ShaderType::Vertex:
            shaderTypename = "Vertex";
            break;
        case ShaderType::Fragment:
            shaderTypename = "Fragment";
            break;
        case ShaderType::Geometry:
            shaderTypename = "Geometry";
            break;
        default:
            Debug::ErrorFormat("Graphics", nullptr, "Unsupported shader type %u", (unsigned int)aShaderType);
            return false;
        }


        aHandle = glCreateShader((GLenum)aShaderType);
        char * src = (char *)aSource.c_str();
        glShaderSource(aHandle, 1, (const char **)&src, NULL);
        glCompileShader(aHandle);
        int status = 0;
        glGetShaderiv(aHandle, GL_COMPILE_STATUS, &status);
        if (status == 0)
        {
            int logLength = 0;
            glGetShaderiv(aHandle, GL_INFO_LOG_LENGTH, &logLength);
            char * infobuffer = new char[logLength + 1];
            glGetShaderInfoLog(aHandle, logLength + 1, 0, infobuffer);
            Debug::ErrorFormat("Graphics", nullptr, "Shader Error: %s", infobuffer);
            delete infobuffer;
            glDeleteShader(aHandle);
            aHandle = 0;
            return false;
        }
        return true;
    }

    bool Shader::LinkShaderProgram(GLuint & aProgram, const GLuint aShaders[], const bool aShadersEnabled[])
    {
        aProgram = glCreateProgram();

        if (aShadersEnabled[0])
        {
            glAttachShader(aProgram, aShaders[0]);
        }
        if (aShadersEnabled[1])
        {
            glAttachShader(aProgram, aShaders[1]);
        }
        if (aShadersEnabled[2])
        {
            glAttachShader(aProgram, aShaders[2]);
        }

        glLinkProgram(aProgram);

        int status = 0;
        glGetProgramiv(aProgram, GL_LINK_STATUS, &status);
        if (status == 0)
        {
            int logLength = 0;
            glGetProgramiv(aProgram, GL_INFO_LOG_LENGTH, &logLength);
            char * infobuffer = new char[logLength + 1];
            glGetProgramInfoLog(aProgram, logLength + 1, 0, infobuffer);
            Debug::ErrorFormat("Graphics", nullptr, "Shader Program Error: %s", infobuffer);
            delete infobuffer;
            return false;
        }
        return true;
    }
}