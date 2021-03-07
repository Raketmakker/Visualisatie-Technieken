#version 330

uniform sampler2D s_texture;
uniform float time;
in vec2 texCoord;

void main()
{
    vec2 coord = vec2(0.01 * sin(time + texCoord.y * 10), 0.001 * cos(time + texCoord.x * 20));
	
    gl_FragColor = texture2D(s_texture, texCoord + coord);
}