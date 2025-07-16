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

		glm::vec3 Position() const;
		glm::vec3 Forward() const;

		glm::mat4 View() const;
		glm::mat4 Projection() const;

		static Camera& Instance;

	private:
		Camera();
		~Camera();
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator= (const Camera&) = delete;
		Camera& operator= (Camera&&) = delete;

	private:
		static Camera instance;

		glm::vec3 position;
		glm::vec3 forward;

		bool isPerspective;
		bool isFovChanged;

		float fov;
		float width;
		float height;
		float near;
		float far;

		float speed;
		float sensitivity;
		bool isFirstMove;
		float yaw;
		float pitch;
		glm::vec2 mouseLastPosition;

		glm::mat4 projection;
		glm::mat4 view;
	};
}