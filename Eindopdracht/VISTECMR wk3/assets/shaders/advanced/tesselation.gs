#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 9) out;

in vec3 color[3];
out vec3 color2;

uniform float time;

void main() {  
	
	vec4 center;
	center.x = (gl_in[0].gl_Position.x + gl_in[1].gl_Position.x + gl_in[2].gl_Position.x) / 3.0;
	center.y = (gl_in[0].gl_Position.y + gl_in[1].gl_Position.y + gl_in[2].gl_Position.y) / 3.0;
	center.z = (gl_in[0].gl_Position.z + gl_in[1].gl_Position.z + gl_in[2].gl_Position.z) / 3.0;
	center.w = (gl_in[0].gl_Position.w + gl_in[1].gl_Position.w + gl_in[2].gl_Position.w) / 3.0;

	for(int i=0; i<3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		color2 = color[i];
		EmitVertex();
		int mo = int(mod(i + 1 ,3));
		gl_Position = gl_in[mo].gl_Position;
		color2 = color[i];
		EmitVertex();
		gl_Position = center;
		color2 = color[i];
		EmitVertex();
		EndPrimitive();
	}
}  
