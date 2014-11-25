#implement vertex vert
#implement fragment fragment
#vert
///Required Attributes/Uniforms/Varyings
attribute vec3 a_Position;
attribute vec2 a_TexCoord;
attribute vec3 a_Normal;

uniform mat4 MATRIX_MVP;
uniform mat4 MODEL;

varying vec3 v_Normal;
varying vec2 v_TexCoord;
varying vec4 v_WSVertexPos;
///End Required

void main()
{
	gl_Position = MATRIX_MVP * vec4(a_Position, 1.0);
	v_WSVertexPos = MODEL * vec4(a_Position, 1.0);
	v_Normal = a_Normal;
	v_TexCoord = a_TexCoord;
}
#fragment
varying vec3 v_Normal;
varying vec4 v_WSVertexPos;

///The texture to sample from
uniform sampler2D u_MainTexture;
///The size of each sample (n / textureSize)
uniform vec2 u_SampleSize;
///How many samples to take in the blur direction
uniform int u_Samples;
///The direction to move each sample
uniform vec2 u_BlurDirection;
///The uv coordinate for this fragment
varying vec2 v_TexCoord;
void main()
{
	//Offset the tex coord
	vec2 texCoord = v_TexCoord + (u_SampleSize * 0.5);
	vec4 finalColor;
	///Sample for u_Samples and move the texcoord in u_BlurDirection
	for(int i = 1; i < u_Samples; i++)
	{
		finalColor += texture2D(u_MainTexture, texCoord);
		texCoord += u_BlurDirection;
	}
	gl_FragColor = finalColor / u_Samples;
}