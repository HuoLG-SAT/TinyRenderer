#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
#include"../depend/glm/gtx/euler_angles.hpp"

namespace Renderer
{
	class GameObject;
	class Transform
	{
	public:
		Transform(GameObject& gameObject);
		~Transform();
		
		glm::mat4& Model();

		inline void SetDirty()
		{
			isDirty = true;
		}

	public:
		glm::vec3& Position = position;
		glm::vec3& Rotate = rotate;
		glm::vec3& Scale = scale;

	private:
		GameObject& gameObject;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotate = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		bool isDirty = true;
	};
}