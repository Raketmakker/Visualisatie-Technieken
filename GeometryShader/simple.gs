#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 100) out;

in vec3 color[1];
out vec3 color2;

uniform float time;

#define M_PI 3.1415926535897932384626433832795
const int points = 5;
const float inc = M_PI / points;

void main() {  

	float inner = 0.01 + 0.05 * abs(sin(time));
	float outer = 2 * inner;

	
	float f = 0;
	float angle = gl_PrimitiveIDIn+time*4;
	for(int i = 0; i < points; i++)
	{

	    gl_Position = gl_in[0].gl_Position + vec4(outer  * cos(angle), outer * sin(angle), 0.0, 0.0);
		color2 = color[0];
	    EmitVertex();

	    gl_Position = gl_in[0].gl_Position + vec4(inner * cos(angle-inc), inner * sin(angle-inc), 0.0, 0.0); 
		color2 = color[0] * .25;
	    EmitVertex();

	    gl_Position = gl_in[0].gl_Position + vec4(inner * cos(angle+inc), inner * sin(angle+inc), 0.0, 0.0); 
		color2 = color[0] * .25;
	    EmitVertex();
		EndPrimitive();


//middle
	    gl_Position = gl_in[0].gl_Position; 
		color2 = color[0] * 2;
	    EmitVertex();
	    gl_Position = gl_in[0].gl_Position + vec4(inner * cos(angle+inc), inner * sin(angle+inc), 0.0, 0.0); 
		color2 = color[0] * 0.25;
	    EmitVertex();
	    gl_Position = gl_in[0].gl_Position + vec4(inner * cos(angle-inc), inner * sin(angle-inc), 0.0, 0.0); 
		color2 = color[0] * 0.25;
	    EmitVertex();
		EndPrimitive();


		angle += M_PI/(points/2.0);
    }
}  