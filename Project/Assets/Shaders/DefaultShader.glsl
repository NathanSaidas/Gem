#implement vertex vert
#implement fragment frag
#vert
attribute vec3 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TexCoord;
attribute vec3 a_Normal;
varying vec2 v_TexCoord;
varying vec3 v_Color;
uniform mat4 MATRIX_MVP;
void main()
{
	gl_Position = MATRIX_MVP * vec4(a_Position, 1.0);
	vec3 someOperationBecauseOpenGLIsOptimizingMyShit = a_Normal + vec3(1.0,1.0,1.0);
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
}

#frag
struct Light
{
	vec2 position;
	vec4 color;
	vec3 attenutation;
};
uniform sampler2D u_MainTexture;
uniform Light u_Lights[1];
varying vec2 v_TexCoord;
varying vec3 v_Color;
void main()
{
	vec4 color = texture2D(u_MainTexture, v_TexCoord);
	gl_FragColor = vec4(v_Color, 1.0);

}


