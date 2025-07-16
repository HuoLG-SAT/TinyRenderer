#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aIntensity;

out V2F
{
	float intensity;
}v2f;

layout (std140) uniform PVCUBO
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
};

uniform mat4 model;

void main()
{
	//gl_Position内置变量表示裁剪空间位置
	gl_Position = projection * view * model * vec4(aPos,1);
	v2f.intensity = aIntensity;
}