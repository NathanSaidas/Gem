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
///The uv coordinate for this fragment
varying vec2 v_TexCoord;
//Constant blend weights
const float[] blendStr = {0.14,0.9,0.9,0.9,
						0.14,0.14,0.9,0.9,
						0.9};
void main()
{	
	//Offset the tex coord
	vec2 texCoord = v_TexCoord + (u_SampleSize * 0.5);
	vec4 finalColor;
	//Begin Sampling
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x + u_SampleSize.x,texCoord.y			       )) *  blendStr[0]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x + u_SampleSize.x,texCoord.y + u_SampleSize.y)) * blendStr[1]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x				   ,texCoord.y + u_SampleSize.y)) *  blendStr[2]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x - u_SampleSize.x,texCoord.y + u_SampleSize.y)) * blendStr[3]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x - u_SampleSize.x,texCoord.y				   )) *  blendStr[4]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x - u_SampleSize.x,texCoord.y - u_SampleSize.y)) * blendStr[5]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x				   ,texCoord.y - u_SampleSize.y)) *  blendStr[6]);
	finalColor += (texture2D(u_MainTexture, vec2(texCoord.x + u_SampleSize.x,texCoord.y - u_SampleSize.y)) * blendStr[7]);
	finalColor += (texture2D(u_MainTexture, texCoord)  * blendStr[8]);
	//Normalize the value - force alpha to 1	
	finalColor = vec4(finalColor.x / 9.0,finalColor.y / 9.0,finalColor.z / 9.0, 1.0);
	gl_FragColor = finalColor;
}