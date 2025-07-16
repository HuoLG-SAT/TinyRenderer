#version 330 core

in V2F
{
	vec3 worldNormal;
	vec3 worldPos;
	vec2 uv;
	vec3 cameraPos;
} v2f;

out vec4 FragColor;

uniform sampler2D transparentTexture;

void main()
{
	FragColor = texture(transparentTexture,v2f.uv);
}