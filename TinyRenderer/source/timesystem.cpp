#include"../header./timesystem.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>

namespace TimeSystem
{
	float _delta = 0;
	float _tick = 0;
	float _lastTick = 0;
}

void TimeSystem::Tick()
{
	_tick = glfwGetTime();
	_delta = _tick - _lastTick;
	_lastTick = _tick;
}

const float& TimeSystem::delta = _delta;
const float& TimeSystem::tick = _tick;