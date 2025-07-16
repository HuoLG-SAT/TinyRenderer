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
	//将天空盒立方体在裁剪空间的Z分量设置为W分量,使得在进行透视除法时Z/W的值恒为1,表示深度值最大,以此可以在场景中全部物体渲染完成后,再渲染天空盒,由于天空盒深度值为1,表示最大,所以不会通过深度测试从而覆盖场景中物体,需要设置深度测试函数为小于等于
	gl_Position = vec4(projection * mat4(mat3(view)) * vec4(aPos,1)).xyww;	//设置view时,应只使用其3×3部分,进行缩放和平移,避免天空盒受到摄像机移动影响
	v2f.texCoords = aPos; //通过天空盒模型的坐标作为天空盒纹理的采样向量
}
