#include"../header/lightManager.h"
#include"../header/camera.h"
#include"../header/renderer.h"
#include"../header/shader.h"
#include"../header/shadowMapManager.h"
#include"../header/timeUtility.h"
#include"../header/mathUtility.h"

Renderer::LightManager::LightManager() = default;
Renderer::LightManager::~LightManager() = default;
void Renderer::LightManager::Updata()
{
	if (isOpenDirectionalLight)
	{
		if (directionalLight.isPickUp)
		{
			directionalLight.direction = Renderer::CameraI.Forward();
			ShadowMapManagerI.UpdataDirectionaLightSpaceMatrix();
		}
		else if (directionalLight.isSelfMovement)
		{
			switch (directionalLight.moveAxis)
			{
				case Axis::X:
					directionalLight.direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(directionalLight.speed), glm::vec3(1.0f, 0.0f, 0.0f))) * directionalLight.direction;
					break;

				case Axis::Y:
					directionalLight.direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(directionalLight.speed), glm::vec3(0.0f, 1.0f, 0.0f))) * directionalLight.direction;
					break;

				case Axis::Z:
					directionalLight.direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(directionalLight.speed), glm::vec3(0.0f, 0.0f, 1.0f))) * directionalLight.direction;
					break;
			}
			ShadowMapManagerI.UpdataDirectionaLightSpaceMatrix();
		}
	}
	if (isOpenPointLight)
	{
		for (int i = 0; i < pointLightNum && i < MAX_POINT_NUM; i++)
		{
			if (pointLights[i].isPickUp)
			{
				pointLights[i].position = Renderer::CameraI.Position();
				ShadowMapManagerI.UpdataPointtLightSpaceMatrix(i);
			}
		}
	}
	if (isOpenSpotLight && spotLight.isPickUp)
	{
		spotLight.position = Renderer::CameraI.Position();
		spotLight.direction = Renderer::CameraI.Forward();
		ShadowMapManagerI.UpdataSpotLightSpaceMatrix();
	}
}
void Renderer::LightManager::AddPointNum()
{
	pointLightNum = glm::clamp(pointLightNum + 1, 0, 5);
}
void Renderer::LightManager::DecreasePointNum()
{
	pointLightNum = glm::clamp(pointLightNum - 1, 0, 5);
}
void Renderer::LightManager::SetShader(Shader& shader)
{
	shader.SetBool("isOpenDirectionalLight", isOpenDirectionalLight);
	shader.SetBool("isOpenPointLight", isOpenPointLight);
	shader.SetBool("isOpenSpotLight", isOpenSpotLight);
	shader.SetBool("isOpenAmbient", isOpenAmbient);

	shader.SetBool("isEnableDirectionalLightShadowCaster", ShadowMapManagerI.isEnableDirectionalShadowCaster);
	shader.SetBool("isEnablePointLightShadowCaster", ShadowMapManagerI.isEnablePointLightShadowCaster);
	shader.SetBool("isEnableSpotLightShadowCaster", ShadowMapManagerI.isEnableSpotLightShadowCaster);

	if (isOpenDirectionalLight)
	{
		shader.SetVec3("directionalLight.direction", directionalLight.direction);
		shader.SetVec3("directionalLight.diffuse", directionalLight.diffuse);
		shader.SetVec3("directionalLight.specular", directionalLight.specular);

		//纹理单元10用于平行光ShadowMap
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, ShadowMapManagerI.DLDBO);
		shader.SetInt("directionalLight.shadowMap", 10);
	}
	if (isOpenPointLight)
	{
		shader.SetInt("pointLightNum", pointLightNum);
		for (int i = 0; i < pointLightNum && i < MAX_POINT_NUM; i++)
		{
			shader.SetVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), pointLights[i].position);
			shader.SetVec3(("pointLights[" + std::to_string(i) + "].diffuse").c_str(), pointLights[i].diffuse);
			shader.SetVec3(("pointLights[" + std::to_string(i) + "].specular").c_str(), pointLights[i].specular);
			shader.SetFloat(("pointLights[" + std::to_string(i) + "].constant").c_str(), pointLights[i].constant);
			shader.SetFloat(("pointLights[" + std::to_string(i) + "].linear").c_str(), pointLights[i].linear);
			shader.SetFloat(("pointLights[" + std::to_string(i) + "].quadratic").c_str(), pointLights[i].quadratic);

			//纹理单元12-16用于点光源ShadowMap
			glActiveTexture(GL_TEXTURE12 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, ShadowMapManagerI.PLDBO[i]);
			shader.SetInt("pointLights[" + std::to_string(i) + "].shadowMap", 12 + i);
		}
	}
	if (isOpenSpotLight)
	{
		shader.SetVec3("spotLight.position", spotLight.position);
		shader.SetVec3("spotLight.direction", spotLight.direction);
		shader.SetVec3("spotLight.diffuse", spotLight.diffuse);
		shader.SetVec3("spotLight.specular", spotLight.specular);
		shader.SetFloat("spotLight.inCutOff", glm::radians(spotLight.inCutOff));
		shader.SetFloat("spotLight.outCutOff", glm::radians(spotLight.outCutOff));

		//纹理单元11用于聚光灯ShadowMap
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, ShadowMapManagerI.SLDBO);
		shader.SetInt("spotLight.shadowMap", 11);
	}
	if (isOpenAmbient)
	{
		shader.SetVec3("ambient", ambient);
	}
}
Renderer::LightManager& Renderer::LightManager::Instance = instance;
Renderer::LightManager Renderer::LightManager::instance;