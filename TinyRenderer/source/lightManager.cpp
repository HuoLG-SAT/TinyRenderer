#include"../header/lightmanager.h"
#include"../header/camera.h"
#include"../header/renderer.h"
#include"../header/shader.h"
#include"../header/shadowmapmanager.h"
#include"../header/timesystem.h"
#include"../header/mathutility.h"

namespace Renderer
{
	namespace ShaderParameter
	{
		constexpr const char* isOpenDirectionalLight = "isOpenDirectionalLight";
		constexpr const char* isOpenPointLight = "isOpenPointLight";
		constexpr const char* isOpenSpotLight = "isOpenSpotLight";
		constexpr const char* isOpenAmbient = "isOpenAmbient";

		constexpr const char* isEnableDirectionalLightShadowCaster = "isEnableDirectionalLightShadowCaster";
		constexpr const char* isEnablePointLightShadowCaster = "isEnablePointLightShadowCaster";
		constexpr const char* isEnableSpotLightShadowCaster = "isEnableSpotLightShadowCaster";

		constexpr const char* directionalLight_direction = "directionalLight.direction";
		constexpr const char* directionalLight_diffuse = "directionalLight.diffuse";
		constexpr const char* directionalLight_specular = "directionalLight.specularn";

		//纹理单元10用于平行光ShadowMap
		constexpr const unsigned int directionalLightShadowMapTextureUnit = 10;
		constexpr const char* directionalLight_shadowMap = "directionalLight.shadowMap";

		//纹理单元12-16用于点光源ShadowMap
		constexpr const unsigned int pointLightShadowMapTextureUnit = 12;
		constexpr const char* pointLightNum = "pointLightNum";

		//纹理单元11用于聚光灯ShadowMap
		constexpr const unsigned int spotLightShadowMapTextureUnit = 11;
		constexpr const char* spotLight_position = "spotLight.position";
		constexpr const char* spotLight_direction = "spotLight.direction";
		constexpr const char* spotLight_diffuse = "spotLight.diffuse";
		constexpr const char* spotLight_specular = "spotLight.specular";
		constexpr const char* spotLight_inCutOff = "spotLight.inCutOff";
		constexpr const char* spotLight_outCutOff = "spotLight.outCutOff";
		constexpr const char* spotLight_shadowMap = "spotLight.shadowMap";

		constexpr const char* ambient = "ambient";
	}
}

Renderer::LightManager::LightManager() = default;
Renderer::LightManager::~LightManager() = default;
void Renderer::LightManager::Updata()
{
	if (isOpenDirectionalLight)
	{
		if (directionalLight.isPickUp)
		{
			directionalLight.direction = Renderer::CameraI.Forward;
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
		for (int i = 0; i < pointLightNum && i < LightConfig::MAX_POINT_LIGHT_NUM; i++)
		{
			if (pointLights[i].isPickUp)
			{
				pointLights[i].position = Renderer::CameraI.Position;
				ShadowMapManagerI.UpdataPointtLightSpaceMatrix(i);
			}
		}
	}
	if (isOpenSpotLight && spotLight.isPickUp)
	{
		spotLight.position = Renderer::CameraI.Position;
		spotLight.direction = Renderer::CameraI.Forward;
		ShadowMapManagerI.UpdataSpotLightSpaceMatrix();
	}
}
void Renderer::LightManager::SetShader(Shader& shader)
{
	shader.SetBool(ShaderParameter::isOpenDirectionalLight, isOpenDirectionalLight);
	shader.SetBool(ShaderParameter::isOpenPointLight, isOpenPointLight);
	shader.SetBool(ShaderParameter::isOpenSpotLight, isOpenSpotLight);
	shader.SetBool(ShaderParameter::isOpenAmbient, isOpenAmbient);

	shader.SetBool(ShaderParameter::isEnableDirectionalLightShadowCaster, ShadowMapManagerI.isEnableDirectionalShadowCaster);
	shader.SetBool(ShaderParameter::isEnablePointLightShadowCaster, ShadowMapManagerI.isEnablePointLightShadowCaster);
	shader.SetBool(ShaderParameter::isEnableSpotLightShadowCaster, ShadowMapManagerI.isEnableSpotLightShadowCaster);

	if (isOpenDirectionalLight)
	{
		shader.SetVec3(ShaderParameter::directionalLight_direction, directionalLight.direction);
		shader.SetVec3(ShaderParameter::directionalLight_diffuse, directionalLight.diffuse);
		shader.SetVec3(ShaderParameter::directionalLight_specular, directionalLight.specular);

		//纹理单元10用于平行光ShadowMap
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, ShadowMapManagerI.DLDBO);
		shader.SetInt(ShaderParameter::directionalLight_shadowMap, ShaderParameter::directionalLightShadowMapTextureUnit);
	}
	if (isOpenPointLight)
	{
		shader.SetInt(ShaderParameter::pointLightNum, pointLightNum);
		for (int i = 0; i < pointLightNum && i < LightConfig::MAX_POINT_LIGHT_NUM; i++)
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
			shader.SetInt("pointLights[" + std::to_string(i) + "].shadowMap", ShaderParameter::pointLightShadowMapTextureUnit + i);
		}
	}
	if (isOpenSpotLight)
	{
		shader.SetVec3(ShaderParameter::spotLight_position, spotLight.position);
		shader.SetVec3(ShaderParameter::spotLight_direction, spotLight.direction);
		shader.SetVec3(ShaderParameter::spotLight_diffuse, spotLight.diffuse);
		shader.SetVec3(ShaderParameter::spotLight_specular, spotLight.specular);
		shader.SetFloat(ShaderParameter::spotLight_inCutOff, glm::radians(spotLight.inCutOff));
		shader.SetFloat(ShaderParameter::spotLight_outCutOff, glm::radians(spotLight.outCutOff));

		//纹理单元11用于聚光灯ShadowMap
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, ShadowMapManagerI.SLDBO);
		shader.SetInt(ShaderParameter::spotLight_shadowMap, ShaderParameter::spotLightShadowMapTextureUnit);
	}
	if (isOpenAmbient)
	{
		shader.SetVec3(ShaderParameter::ambient, ambient);
	}
}
Renderer::LightManager& Renderer::LightManager::Instance = instance;
Renderer::LightManager Renderer::LightManager::instance;