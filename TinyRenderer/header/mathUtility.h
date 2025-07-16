#pragma once
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
#include"../header/timeUtility.h"
#include<random>

namespace MathUtility
{
#define EPSILON_VALUE (1e-6f)

	extern float sinTime;
	extern float cosTime;

	inline void Updata()
	{
		sinTime = glm::sin(TimeUtility::time);
		cosTime = glm::cos(TimeUtility::time);
	}

	inline bool IsEqual(float a, float b)
	{
		return glm::epsilonEqual(a, b, EPSILON_VALUE);
	}

	namespace Random
	{
		extern std::random_device random;

		inline int Range(int min, int max)
		{
			return min + random() % (max - min + 1);
		}
	}
}