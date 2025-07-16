#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

namespace TimeUtility
{
	extern float delta;
	extern float time;
	extern float lastTick;

	inline void UpdataTime()
	{
		time = glfwGetTime();
		delta = time - lastTick;
		lastTick = time;
	}
}