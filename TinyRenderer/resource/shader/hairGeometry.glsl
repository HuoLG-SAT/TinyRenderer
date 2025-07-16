#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in V2G
{
	mat4 projection;
	vec3 viewNormal;
} v2g[];

uniform float lineDistance;

void GenLine(int index);

void main()
{
	GenLine(0);
	GenLine(1);
	GenLine(2);
}

void GenLine(int index)
{
	gl_Position = v2g[index].projection * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = v2g[index].projection * vec4(vec3(gl_in[index].gl_Position) + v2g[index].viewNormal * lineDistance, 1.0f);
	EmitVertex();

	EndPrimitive();
}