#version 330 core

layout (location = 0) in vec3 aPos;

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
}