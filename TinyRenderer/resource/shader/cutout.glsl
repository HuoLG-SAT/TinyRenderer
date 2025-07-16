#version 330 core

in V2F
{
	vec3 worldNormal;
	vec3 worldPos;
	vec2 uv;
	vec3 cameraPos;
} v2f;

out vec4 FragColor;

uniform sampler2D cutoutTexture;
uniform float cutoutValue;

void main()
{
	vec4 color = texture(cutoutTexture,v2f.uv);
	if(color.a < cutoutValue)
	{
		discard;
	}
	FragColor = color;
}