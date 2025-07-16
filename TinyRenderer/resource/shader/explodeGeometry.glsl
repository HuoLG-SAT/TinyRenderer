#version 330 core
#define MAGNITUDE (2.0f)
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in V2G
{
	mat4 projection;
	vec2 uv;
} v2g[];

out G2F
{
	vec2 uv;
} g2f;

uniform float explandDegress;

vec3 ExplandNormal();
vec4 Expland(vec3 position, vec3 normal);

void main()
{
	vec3 normal = ExplandNormal();

	gl_Position = v2g[0].projection * Expland(vec3(gl_in[0].gl_Position), normal);
	g2f.uv = v2g[0].uv;
	EmitVertex();

	gl_Position = v2g[1].projection * Expland(vec3(gl_in[1].gl_Position), normal);
	g2f.uv = v2g[1].uv;
	EmitVertex();

	gl_Position = v2g[2].projection * Expland(vec3(gl_in[2].gl_Position), normal);
	g2f.uv = v2g[2].uv;
	EmitVertex();

	EndPrimitive();
}

vec3 ExplandNormal()
{
	vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
	return normalize(cross(a,b));
}
vec4 Expland(vec3 position, vec3 normal)
{
	return vec4(position + (normal * explandDegress * MAGNITUDE), 1.0f);
}