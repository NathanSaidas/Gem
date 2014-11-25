#include "G_Shader.h"
#include "G_Graphics.h"
#include <fstream>

namespace Gem
{
	enum class ShaderParserState
	{
		NONE,
		READING,
		READ
	};

	ShaderSource::ShaderSource()
	{
		vertex = string::Empty();
		fragment = string::Empty();
	}
	ShaderSource::~ShaderSource()
	{

	}


	string Shader::s_ShaderFolderPath = string("..\\..\\Assets\\Shaders\\");
	string Shader::ShaderFolderPath()
	{
		return s_ShaderFolderPath;
	}
	string Shader::ShaderFolderPath(string & aPath)
	{
		s_ShaderFolderPath.Set(aPath);
		return s_ShaderFolderPath;
	}

	Shader::Shader()
	{
		m_Shadername = string::Empty();
		m_FragHandle = 0;
		m_VertexHandle = 0;
		m_GeometryHandle = 0;
		m_ProgramHandle = 0;
	}
	Shader::~Shader()
	{
		Free();
	}

	void Shader::Load(string & aShaderName)
	{
		Load(aShaderName, ShaderSource());
	}

	void Shader::Load(string & aShaderName, ShaderSource & aShaderInfo)
	{
		Free();
		m_Shadername = aShaderName;
		string filename = ShaderFolderPath() + aShaderName + SHADER_EXTENSION;

		char buffer[256];
		string sbuffer = string::Empty();
		string keyword = string::Empty();

		string vertexFuncName = string::Empty();
		string fragmentFuncName = string::Empty();
		string vertexShaderSource = string::Empty();
		string fragmentShaderSource = string::Empty();

		ShaderParserState vertexParseState = ShaderParserState::NONE;
		ShaderParserState fragmentParseState = ShaderParserState::NONE;

		bool wasPreprocessing = false;

		std::ifstream readStream(filename.C_Str());
		while (!readStream.eof())
		{
			wasPreprocessing = false;
			readStream.getline(buffer, 1000);
			sbuffer = buffer;
			///Check for preprocessing symbol
			if (sbuffer[0] == '#' && sbuffer.Length() > 1)
			{
				sbuffer = string::SubString(sbuffer, 1);
				int length = sbuffer.Length();
				///Check for the Vertex Shader parsing begin
				///Turn off the Fragment Shader parsing begin.
				if (length == vertexFuncName.Length() && vertexFuncName.Length() > 0)
				{
					keyword = string::SubString(sbuffer, 0);
					if (keyword == vertexFuncName)
					{
						wasPreprocessing = true;
						if (fragmentParseState == ShaderParserState::READING)
						{
							fragmentParseState = ShaderParserState::READ;
							//TODO: Save Fragment source string
						}
						if (vertexParseState != ShaderParserState::READ)
						{
							vertexParseState = ShaderParserState::READING;
#ifdef _SHADER_DEBUG
							std::cout << "::Vertex Shader Source::\n";
#endif
						}

					}
				}
				///Check for Fragment Shader parsing begin,
				///Finish the vertex shader parsing if it was parsing.
				if (length == fragmentFuncName.Length() && fragmentFuncName.Length() > 0)
				{
					keyword = string::SubString(sbuffer, 0);
					if (keyword == fragmentFuncName)
					{
						wasPreprocessing = true;
						if (vertexParseState == ShaderParserState::READING)
						{
							vertexParseState = ShaderParserState::READ;
						}
						if (fragmentParseState != ShaderParserState::READ)
						{
							fragmentParseState = ShaderParserState::READING;
#ifdef _SHADER_DEBUG
							std::cout << "::Fragment Shader Source::\n";
#endif
						}
					}
				}
				///Check for shader implementation preprocessing macros.
				if (length >= SHADER_IMPLEMENT_LENGTH)
				{
					keyword = string::SubString(sbuffer, 0, SHADER_IMPLEMENT_SUBSTRING);
					if (keyword == SHADER_IMPLEMENT)
					{
						wasPreprocessing = true;

						sbuffer = string::SubString(sbuffer, SHADER_IMPLEMENT_LENGTH);
						length = sbuffer.Length();
						if (length > SHADER_VERTEX_LENGTH)
						{
							keyword = string::SubString(sbuffer, 0, SHADER_VERTEX_SUBSTRING);
							if (keyword == SHADER_VERTEX && vertexFuncName.Length() == 0)
							{
								vertexFuncName = string::SubString(sbuffer, SHADER_VERTEX_LENGTH);
							}
							else if (length > SHADER_FRAGMENT_LENGTH)
							{
								keyword = string::SubString(sbuffer, 0, SHADER_FRAGMENT_SUBSTRING);
								if (keyword == SHADER_FRAGMENT && fragmentFuncName.Length() == 0)
								{
									fragmentFuncName = string::SubString(sbuffer, SHADER_FRAGMENT_LENGTH);
								}
							}
						}
					}
					else if (wasPreprocessing == false)
					{
						///Append the source
						if (vertexParseState == ShaderParserState::READING)
						{
							vertexShaderSource += buffer;
							std::cout << buffer << std::endl;
						}
						else if (fragmentParseState == ShaderParserState::READING)
						{
							fragmentShaderSource += buffer;
							std::cout << buffer << std::endl;
						}
					}

				}
				///Check for custom pre processing
				if (wasPreprocessing == false)
				{
					///Append the source
					if (vertexParseState == ShaderParserState::READING)
					{
						vertexShaderSource += buffer;
						vertexShaderSource += "\n";
#if _SHADER_DEBUG
						std::cout << buffer << std::endl;
#endif
					}
					else if (fragmentParseState == ShaderParserState::READING)
					{
						fragmentShaderSource += buffer;
						fragmentShaderSource += "\n";
#if _SHADER_DEBUG
						std::cout << buffer << std::endl;
#endif
					}
				}
			}
			else
			{
				///Append the source
				if (vertexParseState == ShaderParserState::READING)
				{
					vertexShaderSource += buffer;
					vertexShaderSource += "\n";
#if _SHADER_DEBUG
					std::cout << buffer << std::endl;
#endif
				}
				else if (fragmentParseState == ShaderParserState::READING)
				{
					fragmentShaderSource += buffer;
					fragmentShaderSource += "\n";
#if _SHADER_DEBUG
					std::cout << buffer << std::endl;
#endif
				}
			}

		}
		///Set the parsing states of each shader so that it can be compiled later.
		if (vertexParseState == ShaderParserState::READING)
		{
			vertexParseState = ShaderParserState::READ;
		}
		if (fragmentParseState == ShaderParserState::READING)
		{
			fragmentParseState = ShaderParserState::READ;
		}
		m_ProgramHandle = -1;
		///Return the shader source.
		if (vertexParseState == ShaderParserState::READ && vertexShaderSource.Length() != 0)
		{
			aShaderInfo.vertex = vertexShaderSource;
			m_VertexHandle = Compile(ShaderType::VERTEX, vertexShaderSource);
			if (m_ProgramHandle == -1)
			{
				m_ProgramHandle = glCreateProgram();
			}
			glAttachShader(m_ProgramHandle, m_VertexHandle);
		}
		if (fragmentParseState == ShaderParserState::READ && fragmentShaderSource.Length() != 0)
		{
			aShaderInfo.fragment = fragmentShaderSource;
			m_FragHandle = Compile(ShaderType::FRAGMENT, fragmentShaderSource);
			if (m_ProgramHandle == -1)
			{
				m_ProgramHandle = glCreateProgram();
			}
			glAttachShader(m_ProgramHandle, m_FragHandle);
		}

		if (m_ProgramHandle != -1)
		{
			//glBindAttribLocation(m_ProgramHandle, 0, "a_Position");
			//glBindAttribLocation(m_ProgramHandle, 1, "a_Color");
			//glBindAttribLocation(m_ProgramHandle, 2, "a_TexCoord");
			//glBindAttribLocation(m_ProgramHandle, 3, "a_Normal");
			Graphics::CheckForErrors();
			glLinkProgram(m_ProgramHandle);
		}
		else
		{
			if (vertexFuncName.Length() > 0)
			{
				std::cout << "::Shader Error::\n" << "Vertex Implementation not found: " << vertexFuncName.C_Str() << std::endl;
			}
			if (fragmentFuncName.Length() > 0)
			{
				std::cout << "::Shader Error::\n" << "Fragment Implementation not found: " << fragmentFuncName.C_Str() << std::endl;
			}
			return;
		}

		Graphics::CheckForErrors();
		int status;
		glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &status);
		if (status == 0)
		{
			int logLength = 0;
			glGetProgramiv(m_ProgramHandle, GL_INFO_LOG_LENGTH, &logLength);
			char * infobuffer = new char[logLength + 1];
			glGetProgramInfoLog(m_ProgramHandle, logLength + 1, 0, infobuffer);
			std::cout << "::Shader Error\n" << infobuffer << std::endl;

			delete infobuffer;
			Free();
		}
		else
		{
			std::cout << "::Shader " << m_Shadername.C_Str() << " successfully compiled::\n";
			glUseProgram(m_ProgramHandle);
			std::cout << "Position Location: " << glGetAttribLocation(m_ProgramHandle, "a_Position") << std::endl;
			std::cout << "Color Location: " << glGetAttribLocation(m_ProgramHandle, "a_Color") << std::endl;
			std::cout << "UV Location: " << glGetAttribLocation(m_ProgramHandle, "a_TexCoord") << std::endl;
			std::cout << "Normal Location: " << glGetAttribLocation(m_ProgramHandle, "a_Normal") << std::endl;
			glUseProgram(0);
		}
		
	}
	unsigned int Shader::Compile(ShaderType aType, string & aSource)
	{
		GLuint shaderHandle = 0;
		GLenum shaderType = 0;
		switch (aType)
		{
		case ShaderType::VERTEX:
			shaderType = GL_VERTEX_SHADER;
			break;
		case ShaderType::GEOMETRY:
			shaderType = GL_GEOMETRY_SHADER;
			break;
		case ShaderType::FRAGMENT:
			shaderType = GL_FRAGMENT_SHADER;
			break;
		default:
			shaderType = GL_VERTEX_SHADER;
			break;
		}
		shaderHandle = glCreateShader(shaderType);
		const char * strings[] = { aSource.C_Str() };
		glShaderSource(shaderHandle, 1, strings, 0);
		glCompileShader(shaderHandle);

		Graphics::CheckForErrors();
		int status;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
		if (status == 0)
		{
			int logLength = 0;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
			char * infobuffer = new char[logLength + 1];
			glGetShaderInfoLog(shaderHandle, logLength + 1, 0, infobuffer);
			std::cout << "::Shader Error\n" << infobuffer << std::endl;

			delete infobuffer;
			glDeleteShader(shaderHandle);
			return 0;
		}
		return shaderHandle;
	}

	void Shader::Free()
	{
		if (m_ProgramHandle != 0)
		{
			
			if (m_FragHandle != 0)
			{
				glDetachShader(m_ProgramHandle, m_FragHandle);
				glDeleteShader(m_FragHandle);
			}
			if (m_VertexHandle != 0)
			{
				glDetachShader(m_ProgramHandle, m_VertexHandle);
				glDeleteShader(m_VertexHandle);
			}
			if (m_GeometryHandle != 0)
			{
				glDetachShader(m_ProgramHandle, m_GeometryHandle);
				glDeleteShader(m_GeometryHandle);
			}
			glDeleteProgram(m_ProgramHandle);
		}
		else
		{
			if (m_FragHandle != 0)
			{
				glDeleteShader(m_FragHandle);
			}
			if (m_VertexHandle != 0)
			{
				glDeleteShader(m_VertexHandle);
			}
			if (m_GeometryHandle != 0)
			{
				glDeleteShader(m_GeometryHandle);
			}
		}
		m_Shadername = string::Empty();
		m_ProgramHandle = 0;
		m_FragHandle = 0;
		m_VertexHandle = 0;
		m_GeometryHandle = 0;
	}

	string Shader::ShaderName()
	{
		return m_Shadername;
	}
	unsigned int Shader::ProgramHandle()
	{
		return m_ProgramHandle;
	}
	unsigned int Shader::FragmentHandle()
	{
		return m_FragHandle;
	}
	unsigned int Shader::VertexHandle()
	{
		return m_VertexHandle;
	}
	unsigned int Shader::GeometryHandle()
	{
		return m_GeometryHandle;
	}
	bool Shader::IsLoaded()
	{
		return m_ProgramHandle != 0;
	}
}