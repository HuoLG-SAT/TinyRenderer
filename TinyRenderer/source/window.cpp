#include"../header/window.h"
#include"../header/camera.h"
#include"../header/renderer.h"
#include"../depend/stb_image/stb_image.h"
#include"../header/screenPostProcess.h"
Renderer::Window::Window()
	:window(nullptr), width(0.0f),height(0.0f)
{

}
Renderer::Window::~Window()
{
	//销毁窗口
	glfwDestroyWindow(window);

	//卸载GLFW库
	glfwTerminate();
}
bool Renderer::Window::InitWindow(int width, int height)
{
	//GLFW库
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW Load Error!" << std::endl;
		return false;
	}

	//设置GLFW版本号3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	//创建窗口
	window = glfwCreateWindow(width, height, "TinyRenderer", NULL, NULL);
	//检测窗口是否创建成功
	if (!window)
	{
		std::cout << "Create Window Error!" << std::endl;
		glfwTerminate();//卸载库
		return false;
	}
	//打开窗口
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Renderer::WindowCallBack::FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, Renderer::WindowCallBack::MouseCursorMoveCallback);
	glfwSetScrollCallback(window, Renderer::WindowCallBack::MouseScrollCallback);

	//加载图标
	int _width, _height, _channel;
	unsigned char* pixels = stbi_load(HUOLG_ICON_PATH, &_width, &_height, &_channel, 4);
	GLFWimage icon;
	icon.width = _width;
	icon.height = _height;
	icon.pixels = pixels;
	glfwSetWindowIcon(window, 1, &icon);
	stbi_image_free(pixels);

	//初始化GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Init Error!" << std::endl;
		glfwDestroyWindow(window); //销毁窗口
		glfwTerminate(); //卸载GLFW库
		return false;
	}

	glEnable(GL_DEPTH_TEST);	//开启深度测试
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);		//开启面剔除
	glEnable(GL_STENCIL_TEST);	//开启模板测试
	glStencilFunc(GL_ALWAYS, 1, 0xFF);			//设置总是通过模板测试
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//设置更新模板缓冲区方式
	glStencilMask(0xFF);						//设置允许写入模板缓冲区
	glEnable(GL_MULTISAMPLE);					//启用多重采样抗锯齿
	glEnable(GL_FRAMEBUFFER_SRGB);				//开启SRGB帧缓冲区进行Gamma矫正

	this->width = width;
	this->height = height;

	return true;
}
bool Renderer::Window::IsClose()
{
	return static_cast<bool>(glfwWindowShouldClose(window));
}
void Renderer::Window::ClearFrameBuffer()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClearDepth(1.0f);
	glClearStencil(0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Renderer::Window::FlashWindow()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}
void Renderer::Window::ProcessUserInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
int Renderer::Window::Width() const
{
	return width;
}
int Renderer::Window::Height() const
{
	return height;
}
void Renderer::Window::SetWindowSize(int width, int height)
{
	this->width = width;
	this->height = height;
}
void Renderer::Window::SetWindowWidth(int width)
{
	this->width = width;
}
void Renderer::Window::SetWindowHeight(int height)
{
	this->height = height;
}
void Renderer::Window::ResetWindowViewPort()
{
	glViewport(0, 0, width, height);
}
GLFWwindow* Renderer::Window::GetWindow() const
{
	return window;
}
Renderer::Window& Renderer::Window::Instance = instance;
Renderer::Window Renderer::Window::instance;

void Renderer::WindowCallBack::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	WindowI.SetWindowSize(width, height);
	glViewport(0, 0, width, height);
	if ((width > 0) && (height > 0))
	{
		if (!ScreenPostProcessI.ResetFrameBuffer(width, height))
		{
			std::cout << "Reset Frame Buffer Fail!" << std::endl;
			exit(-1);
		}
	}
}
void Renderer::WindowCallBack::MouseCursorMoveCallback(GLFWwindow* window, double xPosition, double yPosition)
{
	Renderer::CameraI.ProcessMouseMovement(glm::vec2(static_cast<float>(xPosition), static_cast<float>(yPosition)), glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT));
}
void Renderer::WindowCallBack::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GL_TRUE)
	{
		CameraI.ProcessMouseScroll(yOffset);
	}
}