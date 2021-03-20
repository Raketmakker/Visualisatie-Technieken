#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 9) out;

in vec3 color[3];
out vec3 color2;

uniform float time;

void main() {  
	
	for(int i=0; i<3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		color2 = color[i];
		EmitVertex();
	}
	EndPrimitive();
}  
