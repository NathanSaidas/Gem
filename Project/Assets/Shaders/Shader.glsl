#implement vertex vert
#implement fragment frag
#vert
attribute vec3 a_Position;
attribute vec2 a_TexCoord;
attribute vec3 a_Normal;

uniform mat4 MATRIX_MVP;
uniform mat4 MODEL;

varying vec3 v_Normal;
varying vec2 v_TexCoord;
varying vec4 v_WSVertexPos;

void main()
{
	gl_Position = MATRIX_MVP * vec4(a_Position, 1.0);
	v_WSVertexPos = MODEL * vec4(a_Position, 1.0);
	v_Normal = a_Normal;
	v_TexCoord = a_TexCoord;
}
#frag
uniform sampler2D u_MainTexture;
uniform int u_Samples;
uniform vec2 u_BlurAmount;
uniform float u_Alpha;

varying vec3 v_Normal;
varying vec2 v_TexCoord;
varying vec4 v_WSVertexPos;
void main()
{
	//vec2 texCoord = v_TexCoord;
	//vec4 finalColor = 0.0;
	//for(int i = 1; i < u_Samples; i++)
	//{
	//	finalColor += texture2D(u_MainTexture, texCoord);
	//	texCoord += u_BlurAmount;
	//}


	//gl_FragColor = finalColor / u_Samples;
	gl_FragColor = texture2D(u_MainTexture,v_TexCoord) * vec4(1.0,1.0,1.0,u_Alpha);
	//gl_FragColor = vec4(0.3176,0.2313,0.0235,u_Alpha);
}