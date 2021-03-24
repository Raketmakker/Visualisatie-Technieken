#version 330

in vec3 color2;
//out vec4 fragColor;

void main()
{
	gl_FragColor = vec4(color2, gl_FragCoord.z);
}