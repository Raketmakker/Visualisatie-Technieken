#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

out VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPosLightSpace;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

void main()
{

	vs_out.fragPos = vec3(modelMatrix * vec4(a_position, 1.0));
	vs_out.normal = transpose(inverse(mat3(modelMatrix))) * a_normal;
	vs_out.texCoord = a_texcoord;
	vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fragPos, 1.0);

	gl_Position = projectionMatrix * viewMatrix * vec4(vs_out.fragPos, 1.0);
}