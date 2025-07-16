#version 330 core

out vec4 FragColor;

uniform vec3 normalColor;

void main()
{
	FragColor = vec4(normalColor, 1.0f);
}