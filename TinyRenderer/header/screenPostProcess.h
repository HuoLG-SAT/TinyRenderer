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

		ScreenPostProcessType type = ScreenPostProcessType::None;

		Model* panel = nullptr;
		Shader* screenPostProcessShader = nullptr;
		bool isEnableScreenPostProcess = false;

		unsigned int FBO = 0;
		unsigned int CBO = 0;
		unsigned int RBO = 0;

		unsigned int MFBO = 0;
		unsigned int MCBO = 0;
		unsigned int MRBO = 0;

		int width = 0;
		int height = 0;
	};
}