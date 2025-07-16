#version 330 core

out vec4 FragColor;

uniform vec3 outLineColor;

void main()
{
	FragColor = vec4(outLineColor,1);
}