#version 330

in vec3 color2;
out vec4 fragColor;

void main()
{
	fragColor = vec4(color2, gl_FragCoord.z);
}