#include"../header/screenPostProcess.h"
#include"../header/resourceManager.h"
#include"../header/window.h"
#include<iostream>
Renderer::ScreenPostProcess::ScreenPostProcess()
	:isEnableScreenPostProcess(false), FBO(0), CBO(0), RBO(0), MFBO(0), MCBO(0), MRBO(0), width(0), height(0), 
	panel(nullptr), screenPostProcessShader(nullptr), type(ScreenPostProcessType::None)
{

}
Renderer::ScreenPostProcess::~ScreenPostProcess()
{
	if (panel)
	{
		delete panel;
		panel = nullptr;
	}

	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &CBO);
	glDeleteRenderbuffers(1, &RBO);
	FBO = 0;
	CBO = 0;
	RBO = 0;

	glDeleteFramebuffers(1, &MFBO);
	glDeleteTextures(1, &MCBO);
	glDeleteRenderbuffers(1, &MRBO);
	MFBO = 0;
	MCBO = 0;
	MRBO = 0;
}
bool Renderer::ScreenPostProcess::InitFrameBuffer()
{
	return CreateFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
}
bool Renderer::ScreenPostProcess::InitPanelModel()
{
	Model* model = Renderer::ResourceManagerI.LoadModel(PANEL_MODEL_PATH);
	if (!model)
	{
		std::cout << "Panel Load Fail!" << std::endl;
		return false;
	}
	panel = new Model(*model);
	return true;
}
bool Renderer::ScreenPostProcess::InitScreenPostProssShader()
{
	screenPostProcessShader = ShaderManagerI.GetShader(ShaderType::ScreenPostScreen);
	return screenPostProcessShader;
}
bool Renderer::ScreenPostProcess::IsEnableScreenPostProcess() const
{
	return isEnableScreenPostProcess;
}
void Renderer::ScreenPostProcess::IsBindDefaultFramebuffer(bool value)
{
	glBindFramebuffer(GL_FRAMEBUFFER, value ? 0 : MFBO);
}
bool Renderer::ScreenPostProcess::ResetFrameBuffer(int width, int height)
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &CBO);
	glDeleteRenderbuffers(1, &RBO);
	FBO = 0;
	CBO = 0;
	RBO = 0;

	glDeleteFramebuffers(1, &MFBO);
	glDeleteTextures(1, &MCBO);
	glDeleteRenderbuffers(1, &MRBO);
	MFBO = 0;
	MCBO = 0;
	MRBO = 0;

	return CreateFrameBuffer(width, height);
}
bool Renderer::ScreenPostProcess::CreateFrameBuffer(int width, int height)
{
	//FBO
	glGenFramebuffers(1, &FBO);				//创建帧缓冲区对象
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);	//绑定帧缓冲区
	//CBO
	glGenTextures(1, &CBO);					//创建纹理作为颜色缓冲区
	glBindTexture(GL_TEXTURE_2D, CBO);		//绑定纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);		//设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);		//设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//设置纹理参数
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //配置纹理属性
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CBO, 0);		//绑定帧缓冲区的颜色附件指向该纹理
	//RBO
	glGenRenderbuffers(1, &RBO);				//创建渲染缓冲对象
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);	//绑定渲染缓冲区对象
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);					  //配置渲染缓冲对象属性
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); //设置帧缓冲对象的深度模板配件指向该渲染缓冲对象
	//判断帧缓冲区是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frame Buffer Object Not Complete!  " << glGetError() << std::endl;
		return false;
	}

	//多采样点帧缓冲区
	glGenFramebuffers(1, &MFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, MFBO);
	//多采样纹理作为颜色缓冲区
	glGenTextures(1, &MCBO);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MCBO);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, MCBO, 0);
	//多采样渲染对象作为深度、模板缓冲区
	glGenRenderbuffers(1, &MRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, MRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MRBO);
	//判断帧缓冲区是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Multisample Frame Buffer Object Not Complete!  " << glGetError() << std::endl;
		return false;
	}
	///解除绑定
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->width = width;
	this->height = height;

	return true;
}
void Renderer::ScreenPostProcess::Draw()
{
	if (!panel || !screenPostProcessShader)
	{
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, MFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Renderer::WindowI.ClearFrameBuffer();

	glDisable(GL_MULTISAMPLE);
	screenPostProcessShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, CBO);
	screenPostProcessShader->SetInt("screenTexture", 0);
	screenPostProcessShader->SetInt("type", static_cast<int>(type));
	screenPostProcessShader->SetSetMatrix4("scale", glm::scale(glm::mat4(1.0), glm::vec3(2.0f, 2.0f, 2.0f)));
	panel->Draw(*screenPostProcessShader, ShaderType::ScreenPostScreen);
	glEnable(GL_MULTISAMPLE);
}
Renderer::ScreenPostProcess& Renderer::ScreenPostProcess::Instance = instance;
Renderer::ScreenPostProcess Renderer::ScreenPostProcess::instance;