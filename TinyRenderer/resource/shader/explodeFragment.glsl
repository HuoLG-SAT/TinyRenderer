#version 330 core

in G2F
{
	vec2 uv;
} g2f;

out vec4 FragColor;

uniform sampler2D _texture;

void main()
{
	FragColor = texture(_texture, g2f.uv);
}