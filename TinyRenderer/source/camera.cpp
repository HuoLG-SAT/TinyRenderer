#include"../header/camera.h"
#include"../header/timesystem.h"
#include"../header/renderer.h"
#include"../header/mathutility.h"

Renderer::Camera::Camera() = default;
Renderer::Camera::~Camera() = default;
bool Renderer::Camera::InitCamera(int width, int height)
{
	this->width = width;
	this->height = height;
	return true;
}
void Renderer::Camera::Updata()
{
	if (isPerspective && isFovChanged)
	{
		isFovChanged = false;
		projection = glm::perspective(glm::radians(fov), width / height, near, far);
	}
	view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}
void Renderer::Camera::Move(float delta, Direction direction)
{
	switch (direction)
	{
		case Direction::Forward:
			position += forward * speed * delta;
			break;
	
		case Direction::Back:
			position -= forward * speed * delta;
			break;

		case Direction::Up:
			position.y += speed * delta;
			break;

		case Direction::Down:
			position.y -= speed * delta;
			break;

		case Direction::Left:
			position -= glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))) * speed * delta;
			break;

		case Direction::Right:
			position += glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))) * speed * delta;
			break;
	}
}
void Renderer::Camera::ProcessUserInput()
{
	GLFWwindow* window = Renderer::WindowI.GetWindow();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Move(TimeSystem::delta, Camera::Direction::Forward);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Move(TimeSystem::delta, Camera::Direction::Back);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Move(TimeSystem::delta, Camera::Direction::Left);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Move(TimeSystem::delta, Camera::Direction::Right);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Move(TimeSystem::delta, Camera::Direction::Up);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Move(TimeSystem::delta, Camera::Direction::Down);
	}
}
void Renderer::Camera::ProcessMouseMovement(glm::vec2 mousePosition, bool isRightButtonPress)
{
	if (isFirstMove || !isRightButtonPress)
	{
		mouseLastPosition = mousePosition;
		isFirstMove = false;
		return;
	}

	glm::vec2 delta = (mousePosition - mouseLastPosition) * sensitivity;
	mouseLastPosition = mousePosition;

	yaw += delta.x;
	pitch = glm::clamp(pitch - delta.y, -89.0f, 89.0f);

	forward = glm::normalize(glm::vec3(
		glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
		glm::sin(glm::radians(pitch)),
		glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))));
}
void Renderer::Camera::ProcessMouseScroll(float offset)
{
	if (!MathUtility::IsEqual(offset, 1) && !MathUtility::IsEqual(offset, -1))
	{
		return;
	}

	isFovChanged = true;
	fov = glm::clamp(fov - offset, 20.0f, 45.0f);
}
Renderer::Camera& Renderer::Camera::Instance = instance;
Renderer::Camera Renderer::Camera::instance;