#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out V2F
{
	vec3 worldNormal;
	vec3 worldPos;
	vec2 uv;
	vec3 cameraPos;
	vec4 directionalLightSpacePos;
	vec4 spotLightSpacePos;
} v2f;

layout (std140) uniform PVCUBO
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
};

layout (std140) uniform DSLUBO
{
	mat4 directionalLightSpaceMatrix;
	mat4 spotLightSpaceMatrix;
};

uniform mat4 model;
uniform bool isEnableDirectionalLightShadowCaster;
uniform bool isEnableSpotLightShadowCaster;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0f);
	v2f.worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
	v2f.worldPos = vec3(model * vec4(aPos,1.0f));
	v2f.uv = texCoord;
	v2f.cameraPos = cameraPos;
	if(isEnableDirectionalLightShadowCaster)
	{
		v2f.directionalLightSpacePos = directionalLightSpaceMatrix * vec4(v2f.worldPos,1);
	}
	if(isEnableSpotLightShadowCaster)
	{
		v2f.spotLightSpacePos = spotLightSpaceMatrix * vec4(v2f.worldPos,1);
	}
}