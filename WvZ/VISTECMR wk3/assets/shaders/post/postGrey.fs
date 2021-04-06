#version 330
uniform sampler2D s_texture;

in vec2 texCoord;

void main()
{

	vec4 color = texture2D(s_texture, texCoord);
	float grey = color.x * 0.3 + color.y * 0.59 + color.z * 0.11;
	gl_FragColor = vec4(grey, grey, grey, 1.0);
}