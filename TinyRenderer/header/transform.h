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

		void SetDirty();

		glm::vec3& Position();
		void Position(glm::vec3 value);
		glm::vec3& Rotate();
		void Rotate(glm::vec3 value);
		glm::vec3& Scale();
		void Scale(glm::vec3 value);

		glm::mat4& Model();

	private:
		GameObject& gameObject;
		glm::vec3 position;
		glm::vec3 rotate;
		glm::vec3 scale;
		glm::mat4 model;
		bool isDirty;
	};
}