#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

namespace Renderer
{
	class Window
	{
	public:
		bool InitWindow(int width, int height);

		inline bool IsClose() const
		{
			return static_cast<bool>(glfwWindowShouldClose(window));
		}
		inline void ClearFrameBuffer() const
		{
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClearDepth(1.0f);
			glClearStencil(0x00);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
		inline void PollEvents() const
		{
			glfwPollEvents();
		}
		inline void FlashWindow() const
		{
			glfwSwapBuffers(window);
		}
		inline void ProcessUserInput() const
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
		}
		inline void ResetWindowViewPort() const
		{
			glViewport(0, 0, width, height);
		}

		inline void SetWindowSize(int width, int height)
		{
			this->width = width;
			this->height = height;
		}
		inline void SetWindowWidth(int width)
		{
			this->width = width;
		}
		inline void SetWindowHeight(int height)
		{
			this->height = height;
		}

		inline GLFWwindow* GetWindow() const
		{
			return window;
		}

	public:
		const int& Width = width;
		const int& Height = height;

		static Window& Instance;
	private:
		Window();
		~Window();

	private:
		GLFWwindow* window = nullptr;

		int width = 0;
		int height = 0;

		static Window instance;
	};

	namespace WindowCallBack
	{
		void FrameBufferSizeCallback(GLFWwindow* window, int widht, int height);
		void MouseCursorMoveCallback(GLFWwindow* window, double xPosition, double yPosition);
		void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	}
}
