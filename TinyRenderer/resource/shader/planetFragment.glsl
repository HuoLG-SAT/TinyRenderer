#version 330 core

in V2F
{
	vec2 uv;
} v2f;

out vec4 FragColor;

uniform sampler2D _texture;

void main()
{
	FragColor = texture(_texture, v2f.uv);
}