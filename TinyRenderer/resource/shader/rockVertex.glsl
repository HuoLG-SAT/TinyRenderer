#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aModel;

out V2F
{
	vec2 uv;
} v2f;

layout (std140) uniform PVCUBO
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
};

void main()
{
	gl_Position = projection * view * aModel * vec4(aPos,1);
	v2f.uv = aTexCoord;
}