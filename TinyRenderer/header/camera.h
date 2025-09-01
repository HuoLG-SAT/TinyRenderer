#pragma once
#include"../header/window.h"
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"

namespace Renderer
{
	class Camera
	{
	public:
		enum class Direction
		{
			Forward,
			Back,
			Up,
			Down,
			Left,
			Right
		};

	public:
		bool InitCamera(int width, int height);

		void Updata();
		void Move(float delta, Direction direction);

		void ProcessUserInput();
		void ProcessMouseMovement(glm::vec2 mousePosition, bool isRightButtonPress);
		void ProcessMouseScroll(float offset);

		static Camera& Instance;

	private:
		Camera();
		~Camera();
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator= (const Camera&) = delete;
		Camera& operator= (Camera&&) = delete;

	public:
		const glm::vec3& Position = position;
		const glm::vec3& Forward = forward;
		const glm::mat4& View = view;
		const glm::mat4& Projection = projection;

	private:
		static Camera instance;

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);

		bool isPerspective = true;
		bool isFovChanged = false;

		float fov = 45.0f;
		float width = 1280;
		float height = 720;
		float near = 0.1f;
		float far = 100.0f;

		float speed = 5.0f;
		float sensitivity = 0.1f;
		bool isFirstMove = true;
		float yaw = -90.0f;
		float pitch = 0.0f;
		glm::vec2 mouseLastPosition = glm::vec2(0.0f);

		glm::mat4 projection = glm::perspective(glm::radians(fov), width / height, near, far);
		glm::mat4 view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
	};
}