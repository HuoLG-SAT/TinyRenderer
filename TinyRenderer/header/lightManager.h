#pragma once
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
#include"renderer.h"
namespace Renderer
{
	enum class Axis
	{
		X,Y,Z
	};

	struct DirectionalLight
	{
		glm::vec3 direction = glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f));

		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		bool isPickUp = false;
		bool isSelfMovement = false;
		float speed = 1.0f;
		Axis moveAxis = Axis::X;
	};

	struct PointLight
	{
		glm::vec3 position = glm::vec3(1.5f, 1.5f, 1.5f);

		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;

		bool isPickUp = false;
	};

	struct SpotLight
	{
		glm::vec3 position = glm::vec3(-1.5f, 1.0f, 1.25f);
		glm::vec3 direction = glm::normalize(glm::vec3(0.65f, -0.5f, -0.5f));

		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		float inCutOff = 8.5f;
		float outCutOff = 12.5f;

		bool isPickUp = false;
	};

	namespace LightConfig
	{
		constexpr const unsigned int MAX_POINT_LIGHT_NUM = 5;
	}

	class LightManager
	{
		friend class GUIManager;
		friend class ShadowMapManager;
		friend class ShaderManager;
	public:
		void Updata();
		void SetShader(Shader& shader);

		inline void AddPointNum()
		{
			pointLightNum = glm::clamp(pointLightNum + 1, 0, 5);
		}
		inline void DecreasePointNum()
		{
			pointLightNum = glm::clamp(pointLightNum - 1, 0, 5);
		}

	public:
		static LightManager& Instance;

	private:
		LightManager();
		~LightManager();
		LightManager(const LightManager&) = delete;
		LightManager(LightManager&&) = delete;
		LightManager& operator=(const LightManager&) = delete;
		LightManager& operator=(LightManager&&) = delete;

	private:
		Renderer::DirectionalLight directionalLight;
		Renderer::PointLight pointLights[LightConfig::MAX_POINT_LIGHT_NUM];
		Renderer::SpotLight spotLight;
		glm::vec3 ambient = glm::vec3(0.4f);

		int pointLightNum = 1;

		bool isOpenDirectionalLight = true;
		bool isOpenPointLight = true;
		bool isOpenSpotLight = true;
		bool isOpenAmbient = true;

		static LightManager instance;
	};
}