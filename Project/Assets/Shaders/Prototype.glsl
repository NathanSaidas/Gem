#implement vertex
in vec3 a_Position;
in vec4 a_Color;

uniform mat4 u_MVP;
out vec4 v_Color;

const float PI = 3.141592653589793;

float Deg2Rad(float deg)
{
	return deg * PI / 180.0;
}

void main()
{
	gl_Position = u_MVP * vec4(a_Position.x, a_Position.y, 0.0, 1.0);
	v_Color = a_Color;
}
#end
#implement fragment
in vec4 v_Color;
out vec4 v_FragColor;

void main()
{
	v_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#end