#version 330

in layout(location=0)vec3 a_position;
in layout(location=1)vec3 a_color;

uniform mat4 modelViewProjectionMatrix;
uniform float time = 0;

out vec3 color;

void main()
{
	color = a_color;
	gl_Position = modelViewProjectionMatrix * vec4(a_position,1);
}