#version 330 core

in G2F
{
	vec4 worldPos;
} g2f;

uniform float far;
uniform vec3 lightPos;

void main()
{
	float _distance = distance(g2f.worldPos.xyz, lightPos);
	gl_FragDepth = _distance / far; //������ӳ�䵽[0-1]֮��
}