#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 spotLightSpaceMatrix;
uniform mat4 model;

void main()
{
	gl_Position = spotLightSpaceMatrix * model * vec4(aPos,1);
}