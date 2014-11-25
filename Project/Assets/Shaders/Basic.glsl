#implement vertex vert
#implement fragment frag
#vert
attribute vec3 a_Position;
attribute vec3 a_PositionB;
attribute vec4 a_Color;
attribute vec2 a_TexCoord;

uniform float u_Time;
uniform mat4 MATRIX_MVP;
varying vec4 v_Color;
varying vec2 v_TexCoord;
void main()
{
	vec3 position = mix(a_Position,a_PositionB,u_Time);
	gl_Position = MATRIX_MVP * vec4(position, 1.0);
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
}
#frag

uniform sampler2D u_MainTexture;
varying vec4 v_Color;
varying vec2 v_TexCoord;
void main()
{
	//vec4 texColor = texture2D(u_MainTexture,v_TexCoord);
	//gl_FragColor = texColor;
	gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}