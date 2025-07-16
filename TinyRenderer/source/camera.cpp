#include"../header/camera.h"
#include"../header/timeUtility.h"
#include"../header/renderer.h"
#include"../header/mathUtility.h"

Renderer::Camera::Camera()
	:position(0.0f), forward(0.0f,0.0f,-1.0f), isPerspective(true), fov(45.0f), width(1280), height(720), near(0.1f),far(100.0f), isFovChanged(false),
	speed(5.0f), sensitivity(0.1f), isFirstMove(true), yaw(-90.0f), pitch(0.0f), mouseLastPosition(0.0f), projection(glm::perspective(glm::radians(fov), width / height, near, far)), view(glm::lookAt(position, position + forward, glm::vec3(0, 1, 0)))
{

}
Renderer::Camera::~Camera() = default;
bool Renderer::Camera::InitCamera(int width, int height)
{
	this->width = width;
	this->height = height;
	return true;
}
void Renderer::Camera::Updata()
{
	//projection = isPerspective ? glm::perspective(glm::radians(fov), width / height, near, far) : glm::mat4(1.0f);
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
		Move(TimeUtility::delta, Camera::Direction::Forward);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Move(TimeUtility::delta, Camera::Direction::Back);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Move(TimeUtility::delta, Camera::Direction::Left);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Move(TimeUtility::delta, Camera::Direction::Right);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Move(TimeUtility::delta, Camera::Direction::Up);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Move(TimeUtility::delta, Camera::Direction::Down);
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
glm::vec3 Renderer::Camera::Position() const
{
	return position;
}
glm::vec3 Renderer::Camera::Forward() const
{
	return forward;
}
glm::mat4 Renderer::Camera::View() const
{
	return view;
}
glm::mat4 Renderer::Camera::Projection() const
{
	return projection;
}
Renderer::Camera& Renderer::Camera::Instance = instance;
Renderer::Camera Renderer::Camera::instance;