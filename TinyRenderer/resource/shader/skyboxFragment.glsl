#version 330 core

in V2F
{
	vec3 texCoords;
}v2f;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox,v2f.texCoords);
}