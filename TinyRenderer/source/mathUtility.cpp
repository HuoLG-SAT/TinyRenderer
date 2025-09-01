#include"../header/mathutility.h"

namespace MathUtility
{
	float _sinTime = 0.0f;
	float _cosTime = 0.0f;
}

void MathUtility::Tick()
{
	_sinTime = glm::sin(TimeSystem::tick); 
	_cosTime = glm::cos(TimeSystem::tick);
}

const float& MathUtility::sinTime = _sinTime;
const float& MathUtility::cosTime = _cosTime;

std::random_device MathUtility::Random::random;