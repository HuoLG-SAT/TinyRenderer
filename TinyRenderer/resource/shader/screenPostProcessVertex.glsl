#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 uv;

uniform mat4 scale; //ʹ��Ƭ�ڲü��ռ��еĵ�X��Y����Ϊ[-1,1],����������Ļ

void main()
{
	gl_Position = scale * vec4(aPos,1);
	uv = vec2(aTexCoord.x,1.0f - aTexCoord.y);	//��תuv�����ֹͼ�����µߵ�
}