#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out G2F
{
	vec4 worldPos;
} g2f;

uniform mat4 pointLightSpaceMatrix[6];

void main()
{
	for(int face = 0;face < 6;face++)
	{
		gl_Layer = face;	//指定当前渲染图层,立方体纹理有+X、-X、+Y、-Y、+Z、-Z共6个图层,分别对应索引0-5
		for(int i = 0;i < 3;i++)
		{
			gl_Position = pointLightSpaceMatrix[face] * gl_in[i].gl_Position;
			g2f.worldPos = gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}
