#ifndef G_SHADER_H
#define G_SHADER_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{

	

	struct ShaderSource
	{
		ShaderSource();
		~ShaderSource();
		string vertex;
		string fragment;
	};

	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY
	};

	class Shader : public Object
	{
	public:
		const string SHADER_EXTENSION = ".glsl";
		const int SHADER_IMPLEMENT_LENGTH = 10;
		const int SHADER_IMPLEMENT_SUBSTRING = 9;
		const int SHADER_VERTEX_LENGTH = 7;
		const int SHADER_VERTEX_SUBSTRING = 6;
		const int SHADER_FRAGMENT_LENGTH = 9;
		const int SHADER_FRAGMENT_SUBSTRING = 8;

		const string SHADER_IMPLEMENT = "implement ";
		const string SHADER_VERTEX = "vertex ";
		const string SHADER_FRAGMENT = "fragment ";

		static string ShaderFolderPath();
		static string ShaderFolderPath(string & aPath);
		static unsigned int Compile(ShaderType aType, string & aSource);

		Shader();
		~Shader();
		void Load(string & aShaderName);
		void Load(string & aShaderName, ShaderSource & aShaderInfo);
		void Free();

		string ShaderName();
		unsigned int ProgramHandle();
		unsigned int FragmentHandle();
		unsigned int VertexHandle();
		unsigned int GeometryHandle();
		bool IsLoaded();
	private:
		static string s_ShaderFolderPath;
		string m_Shadername;
		unsigned int m_FragHandle;
		unsigned int m_VertexHandle;
		unsigned int m_GeometryHandle;
		unsigned int m_ProgramHandle;
		
	};
}
GEM_CLASS(Shader,Object)

#endif