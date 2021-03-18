#version 330
uniform sampler2D s_texture;
uniform int height;
in vec2 texCoord;

const float density = 5;
const float opacityScanline = .5;

void main(void) 
{
    vec3 col = texture2D(s_texture, texCoord).rgb;
    
    float count = height * density;
    vec2 sl = vec2(sin(texCoord.y * count), cos(texCoord.y * count));
	vec3 scanlines = vec3(sl.x, sl.y, sl.x);

    col += col * scanlines * opacityScanline;
	gl_FragColor = vec4(col, 1.0);
}