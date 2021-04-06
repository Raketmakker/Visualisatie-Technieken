#version 330

uniform sampler2D s_texture;

in vec2 texCoord;

void kernel(inout vec4 positions[9])
{

	float w = 1.0 / 1900;
	float h = 1.0 / 1000;

	positions[0] = texture2D(s_texture, texCoord + vec2( -w,  -h));
	positions[1] = texture2D(s_texture, texCoord + vec2(0.0,  -h));
	positions[2] = texture2D(s_texture, texCoord + vec2(  w,  -h));
	positions[3] = texture2D(s_texture, texCoord + vec2( -w, 0.0));
	positions[4] = texture2D(s_texture, texCoord);
	positions[5] = texture2D(s_texture, texCoord + vec2(  w, 0.0));
	positions[6] = texture2D(s_texture, texCoord + vec2( -w,   h));
	positions[7] = texture2D(s_texture, texCoord + vec2(0.0,   h));
	positions[8] = texture2D(s_texture, texCoord + vec2(  w,   h));
}

void main(void) 
{

	vec4 p[9];
	kernel(p);

	vec4 median;
	for (int i = 0; i < 9; i++)
		median += p[i];

	median /= 9.0;

	// no visual (to me) difference

	median *= 2;

	// looks smoother now. Visual difference form the standard. Can conclude the algorithm does something, just not very visualy chaning.

	gl_FragColor = vec4(median.rgb, 1.0);
}