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

	vec4 sobel_edge_h = (p[2] + (2.0 * p[5]) + p[8]) - (p[0] + (2.0 * p[3]) + p[6]);
  	vec4 sobel_edge_v = (p[0] + (2.0 * p[1]) + p[2]) - (p[6] + (2.0 * p[7]) + p[8]);

	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	gl_FragColor = vec4(1.0 - sobel.rgb, 1.0 );
}

//void kernel(inout vec4 positions[3][3], kernelX, kernelY)
//{

    //float w = 1.0 / 1900;
    //float h = 1.0 / 1000;

    //int offsetX = floor(kernelX / 2);
    //int offsetY = floor(kernelY / 2);

    //for (int x = 0; x < kernelX, x++)
        //for (int y = 0; y < kernelY; y++)
            //n[y][x] = texture2D(s_texture, texCoord + vec2(x - offsetX, y - offsetY));
//}

//void main()
//{

    //vec4 n[3][3];
    //kernel(n, 3, 3);

    //vec4 sobel_edge_h = (n[0][2] + (2.0 * n[1][2]) + n[2][2]) - (n[0][0] + (2.0 * n[1][0]) + n[2][0]);
    //vec4 sobel_edge_w = (n[0][0] + (2.0 * n[0][1]) + n[0][2]) - (n[2][0] + (2.0 * n[2][1]) + n[2][2]);
    
    //vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) +  (sobel_edge_v * sobel_edge_v));

    //gl_FragColor = vec4(1.0 - sobel.rgb, 1.0);
//}
