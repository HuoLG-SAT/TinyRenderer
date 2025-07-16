#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 uv;

uniform mat4 scale; //使面片在裁剪空间中的的X、Y分量为[-1,1],覆盖整个屏幕

void main()
{
	gl_Position = scale * vec4(aPos,1);
	uv = vec2(aTexCoord.x,1.0f - aTexCoord.y);	//翻转uv坐标防止图像上下颠倒
}