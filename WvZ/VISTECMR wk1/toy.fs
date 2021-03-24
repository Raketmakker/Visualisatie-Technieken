#version 330

uniform float time;

in vec3 color;
out vec3 fragColor;

void main()
{
	fragColor = vec3(mod(color.r + time, 255.0f), mod(color.g + time, 255.0f), mod(color.b + time, 255.0f)); 
}