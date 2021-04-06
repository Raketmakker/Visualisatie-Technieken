#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform float time;

out vec2 texCoord;

void main()
{

	texCoord = a_texcoord;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(
		a_position + 10 * sin(time) * a_normal,
	1);
}