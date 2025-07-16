#pragma once
#include"shader.h"
#include"transform.h"
#include"model.h"
#include"gUIManager.h"

namespace Renderer
{
#define MODEL "model"
#define EXPLAND_DEGRESS "explandDegress"

	enum class DisplayMode
	{
		Static,
		SwayX,
		SwayY,
		SwayZ,
		RotationX,
		RotationY,
		RotationZ,
	};

	enum class ModelType
	{
		Cube,
		Sphere,
		Panel,
		Custom
	};

	struct GORenderInfo
	{
		float reflectivity = 1.0f;
		float refractive = 0.75f;
		float cutoutValue = 0.1f;
		float outLineWidth = 1.05f;
		glm::vec3 outLineColor = glm::vec3(1.0f);
		glm::vec3 solidColor = glm::vec3(1.0f);
		int diffuseMapChoose = WOOD_DIFFUSE_INDEX;
		int specularMapChoose = IRONY_SPECULAR_INDEX;
		bool isFloowTime = true;
		bool isEnableOutLine = false;
		bool isEnableCullFace = true;
		bool isCullBack = true;
		bool isEnableNormalVisable = false;
		float normalLineDistance = 0.75f;
		glm::vec3 normalColor = glm::vec3(1.0f, 1.0f, 0.0f);
		Renderer::ShaderType shaderType = ShaderType::Default;
		Renderer::DisplayMode displayMode = DisplayMode::Static;
		Renderer::ModelType modelType = Renderer::ModelType::Cube;
		float rotateSpeed = 90.0f;
		float explandDegress = 0.0f;
		float explandRange = 1.0f;
		float explandSpeed = 1.0f;
		bool isShadowCaster = true;
	};

	class GameObject
	{
		friend class GUIManager;
		friend class SceneManager;
		friend class Transform;
	public:
		GameObject(int id);
		~GameObject();

		void Updata();
		void Draw(Shader& shader);
		void DrawOutLine(Shader& shader);
		void DrawShadowMap(Shader& shader, ShaderType shaderType);
		void ChooseModel();
		void ChooseDiffuseMap();
		void ChooseSpecularMap();

		void SetParent();
		void SetModelMap(TextureType type, unsigned int);
		Renderer::Texture* GetModeMap(TextureType type);

		void SetIsDelete(bool value);

		glm::mat4 Model();

	private:
		void ChooseCubeModel();
		void ChooseSphereModel();
		void ChoosePanelModel();
		void ChooseCustomModel();
		void ChooseModel(const char* path);

		void ChooseCustomTexture(TextureType type);
		void ChooseMap(TextureType type, const std::string& path);

		void DrawDefaultMode(Shader& shader);
		void DrawSolidMode(Shader& shader);
		void DrawOutLineMode(Shader& shader);
		void DrawCutoutMode(Shader& shader);
		void DrawTransparentMode(Shader& shader);
		void DrawReflectMode(Shader& shader);
		void DrawRefractMode(Shader& shader);
		void DrawExplandMode(Shader& shader);
		void DrawNormalMode(Shader& shader);
		void DrawShadowMapModel(Shader& shader, ShaderType shaderType);

	private:
		Renderer::Transform transform;
		Renderer::Model* model;

		GameObject* parent;
		bool isEnable;
		bool isDelete;

		int id;
		int parentId;
	
		//物体渲染参数信息
		GORenderInfo renderInfo;
	};
}