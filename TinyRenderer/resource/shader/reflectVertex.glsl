#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out V2F
{
	vec3 worldPos;
	vec3 worldNormal;
	vec3 cameraPos; 
} v2f;

layout (std140) uniform PVCUBO
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos; 
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1);
	v2f.worldPos = vec3(model * vec4(aPos,1));
	v2f.worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
	v2f.cameraPos = cameraPos;
}