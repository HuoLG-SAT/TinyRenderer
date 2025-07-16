#version 330 core

in V2F
{
	vec3 worldPos;
	vec3 worldNormal;
	vec3 cameraPos; 
} v2f;

out vec4 FragColor;

uniform samplerCube cubemap;
uniform float reflectivity;

void main()
{
	vec3 viewDir = normalize(v2f.cameraPos - v2f.worldPos);
	vec3 reflectDir = normalize(reflect(-viewDir,v2f.worldNormal));
	float fresnel = reflectivity + (1 - reflectivity)*pow(1 - dot(viewDir,v2f.worldNormal), 5);
	FragColor = texture(cubemap,reflectDir) * fresnel;
}