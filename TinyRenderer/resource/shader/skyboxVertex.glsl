#version 330 core
layout (location = 0) in  vec3 aPos;

out V2F
{
	vec3 texCoords;
}v2f;

layout (std140) uniform PVCUBO
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
};

void main()
{
	//����պ��������ڲü��ռ��Z��������ΪW����,ʹ���ڽ���͸�ӳ���ʱZ/W��ֵ��Ϊ1,��ʾ���ֵ���,�Դ˿����ڳ�����ȫ��������Ⱦ��ɺ�,����Ⱦ��պ�,������պ����ֵΪ1,��ʾ���,���Բ���ͨ����Ȳ��ԴӶ����ǳ���������,��Ҫ������Ȳ��Ժ���ΪС�ڵ���
	gl_Position = vec4(projection * mat4(mat3(view)) * vec4(aPos,1)).xyww;	//����viewʱ,Ӧֻʹ����3��3����,�������ź�ƽ��,������պ��ܵ�������ƶ�Ӱ��
	v2f.texCoords = aPos; //ͨ����պ�ģ�͵�������Ϊ��պ�����Ĳ�������
}
