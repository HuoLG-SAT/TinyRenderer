#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

namespace Renderer
{
#define HUOLG_ICON_PATH ("resource\\icon\\Huolg.png")

	class Window
	{
	public:
		bool InitWindow(int width, int height);
		bool IsClose();

		void ClearFrameBuffer();
		void FlashWindow();

		void ProcessUserInput();
		void ResetWindowViewPort();

		int Width() const;
		int Height() const;

		void SetWindowSize(int width, int height);
		void SetWindowWidth(int width);
		void SetWindowHeight(int height);

		GLFWwindow* GetWindow() const;

		static Window& Instance;
	private:
		Window();
		~Window();

	private:
		GLFWwindow* window;

		int width;
		int height;

		static Window instance;
	};

	namespace WindowCallBack
	{
		void FrameBufferSizeCallback(GLFWwindow* window, int widht, int height);
		void MouseCursorMoveCallback(GLFWwindow* window, double xPosition, double yPosition);
		void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	}
}
