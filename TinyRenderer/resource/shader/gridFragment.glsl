#version 330 core

in V2F
{
	float intensity;
}v2f;

out vec4 FragColor;

uniform float gridFadeDistance;
uniform vec3 gridLineColor;

void main()
{
	//gl_FragCoord���ñ�����ʾ��Ļ�ռ�������,z�д洢�˲ü��ռ����͸�ӳ�����(���ü��ռ��е�Z/W)�淶���������Ϣ,ֵ��[0,1]֮��,w�д洢�˲ü��ռ��е�W�ĵ���(��1/W),�ü��ռ�ռ��е�W��ʾ�۲�ռ��е�z�ĸ���,��ʾ����������ľ���
	float depth = 1 / gl_FragCoord.w;
	float alpha = 1.0f - smoothstep(gridFadeDistance * 0.8f, gridFadeDistance, depth);
	FragColor = vec4(gridLineColor * v2f.intensity, alpha * 0.6);
}