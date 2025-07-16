#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out V2G
{
	mat4 projection;
	vec3 viewNormal;
} v2g;

layout (std140) uniform PVCUBO
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
};

uniform mat4 model;

void main()
{
	gl_Position = view * model * vec4(aPos,1);
	v2g.projection = projection;
	v2g.viewNormal = normalize(mat3(transpose(inverse(view * model))) * aNormal);
}