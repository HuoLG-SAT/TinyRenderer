#version 330 core

out vec4 FragColor;

uniform vec3 solidColor;

void main()
{
	FragColor = vec4(solidColor,1);
}
