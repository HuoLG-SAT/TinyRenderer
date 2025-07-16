#pragma once
#include"renderer.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>

namespace Renderer
{
	enum class ScreenPostProcessType
	{
		None,
		Grey,
		Invert,
		Sharpen,
		Blur,
		Edge
	};

	class ScreenPostProcess
	{
		friend class GUIManager;
	public:
		bool InitFrameBuffer();
		bool InitPanelModel();
		bool InitScreenPostProssShader();
		bool IsEnableScreenPostProcess() const;

		void Draw();
		void IsBindDefaultFramebuffer(bool value);

		bool ResetFrameBuffer(int width,int height);

		static ScreenPostProcess& Instance;

	private:
		ScreenPostProcess();
		~ScreenPostProcess();
		ScreenPostProcess(const ScreenPostProcess&) = delete;
		ScreenPostProcess(ScreenPostProcess&&) = delete;
		ScreenPostProcess& operator=(const ScreenPostProcess&) = delete;
		ScreenPostProcess& operator=(ScreenPostProcess&&) = delete;

		bool CreateFrameBuffer(int width, int height);

	private:
		static ScreenPostProcess instance;

		ScreenPostProcessType type;

		Model* panel;
		Shader* screenPostProcessShader;
		bool isEnableScreenPostProcess;

		unsigned int FBO;
		unsigned int CBO;
		unsigned int RBO;

		unsigned int MFBO;
		unsigned int MCBO;
		unsigned int MRBO;

		int width;
		int height;
	};
}