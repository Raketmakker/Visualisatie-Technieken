#version 330

uniform sampler2D s_texture;
uniform float time;
uniform int width;
uniform int height;
in vec2 texCoord;

void main()
{
    vec2 uv = texCoord; 
    vec3 tex = texture2D(s_texture, texCoord).rgb;
    float pixels = 10.0;
    float dx = pixels / width;
    float dy = pixels / height;
    uv = vec2(dx * (floor(uv.x/dx) + 0.5), dy * (floor(uv.y/dy) + 0.5));
    vec3 col = texture2D(s_texture, uv).rgb;
	gl_FragColor = vec4(col, 1.0);
}