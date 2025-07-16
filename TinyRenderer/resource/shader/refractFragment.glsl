#version 330 core

in V2F
{
	vec3 worldPos;
	vec3 worldNormal;
	vec3 cameraPos; 
} v2f;

out vec4 FragColor;

uniform samplerCube cubemap;
uniform float refractive;

void main()
{
	vec3 viewDir = normalize(v2f.cameraPos - v2f.worldPos);
	vec3 reflact = normalize(refract(-viewDir,v2f.worldNormal,refractive));
	FragColor = texture(cubemap, reflact);
}