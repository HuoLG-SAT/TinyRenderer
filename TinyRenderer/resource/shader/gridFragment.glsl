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
	//gl_FragCoord内置变量表示屏幕空间中坐标,z中存储了裁剪空间进行透视除法后(即裁剪空间中的Z/W)规范化的深度信息,值在[0,1]之间,w中存储了裁剪空间中的W的倒数(即1/W),裁剪空间空间中的W表示观察空间中的z的负数,表示距离摄像机的距离
	float depth = 1 / gl_FragCoord.w;
	float alpha = 1.0f - smoothstep(gridFadeDistance * 0.8f, gridFadeDistance, depth);
	FragColor = vec4(gridLineColor * v2f.intensity, alpha * 0.6);
}