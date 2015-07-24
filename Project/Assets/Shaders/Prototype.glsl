#implement vertex
#version 130
in vec3 a_Position;
in vec4 a_Color;

out vec4 v_Color;

void main()
{
	gl_Position = vec4(a_Position, 0.0);
	v_Color = a_Color;
}
#end
#implement fragment
#version 130
in vec4 v_Color;
out vec4 v_FragColor;

void main()
{
	v_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#end