#version 330


uniform sampler2D s_texture;
uniform float time;
uniform int width;
uniform int height;

in vec2 texCoord;

void main()
{

	vec3 texColor = texture2D(s_texture, texCoord).rgb;

	float pixelSize = 8.0f;
	float dx = pixelSize / width;
	float dy = pixelSize / height;

	vec2 uv = vec2(
		dx * (floor(texCoord.x/dx) + 0.5), 
		dy * (floor(texCoord.y/dy) + 0.5)
	);

	texColor = texture2D(s_texture, uv).rgb;
	gl_FragColor = vec4(texColor, 1.0);
}