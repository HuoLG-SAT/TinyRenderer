#include"../header/skybox.h"
#include"../header/model.h"
#include"../header/resourceManager.h"
#include"../header/camera.h"
Renderer::SkyBox::SkyBox() :skybox(nullptr)
{

}
Renderer::SkyBox::~SkyBox()
{
	if (skybox)
	{
		delete skybox;
		skybox = nullptr;
	}
}
bool Renderer::SkyBox::Init()
{
	Model* model = ResourceManagerI.LoadModel(CUBE_MODEL_PATH);
	if (!model)
	{
		std::cout << "Sky Box Model Load Fail!" << std::endl;
		return false;
	}
	skybox = new Model(*model);
	return true;
}
void Renderer::SkyBox::Draw(Shader& shader)
{
	shader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManagerI.skyBoxCubemap->id);
	shader.SetInt("skyBoxShader", 0);
	skybox->Draw(shader, ShaderType::SkyBox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Renderer::SkyBoxManager::SkyBoxManager() 
	:skyBoxShader(nullptr), skyBoxCubemap(nullptr), isEnableSkyBox(true), skyBoxCubeMapType(SkyBoxCubeMapType::Glacier)
{

}
Renderer::SkyBoxManager::~SkyBoxManager() = default;
bool Renderer::SkyBoxManager::InitSkyBox()
{
	skyBoxCubemap = ResourceManagerI.GetCubemap(CUBEMAP_GLACIER_SKYBOX_NAME);
	skyBoxShader = ShaderManagerI.GetShader(ShaderType::SkyBox);
	return skyBoxCubemap && skyBoxShader && skyBox.Init();
}
void Renderer::SkyBoxManager::Draw()
{
	glCullFace(GL_FRONT);
	skyBox.Draw(*skyBoxShader);
	glCullFace(GL_BACK);
}
bool Renderer::SkyBoxManager::IsEnableSkyBox() const
{
	return isEnableSkyBox;
}
void Renderer::SkyBoxManager::ChooseSkyBoxCubeMap()
{
	switch (skyBoxCubeMapType)
	{
		case Renderer::SkyBoxCubeMapType::Glacier:
			skyBoxCubemap = ResourceManagerI.GetCubemap(CUBEMAP_GLACIER_SKYBOX_NAME);
			break;

		case Renderer::SkyBoxCubeMapType::Palace:
			skyBoxCubemap = ResourceManagerI.GetCubemap(CUBEMAP_PALACE_SKYBOX_NAME);
			break;
	}
}
Renderer::SkyBoxManager& Renderer::SkyBoxManager::Instance = instance;
Renderer::SkyBoxManager Renderer::SkyBoxManager::instance;