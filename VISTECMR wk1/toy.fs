#version 330

uniform float time;

in vec3 color;
out vec3 fragColor;

void main()
{
	fragColor = vec3(mod(color.r + time, 1.0f), mod(color.g + time, 1.0f), mod(color.b + time, 1.0f)); 
}