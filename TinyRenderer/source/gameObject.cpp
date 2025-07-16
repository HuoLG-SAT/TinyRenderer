#include"../header/gameObject.h"
#include"../header/resourceManager.h"
#include"../header/timeUtility.h"
#include"../header/mathUtility.h"
#include"../depend/tinyfiledialogs/tinyfiledialogs.h"
#include"../header/sceneManager.h"
#include"../header/gUIManager.h"

Renderer::GameObject::GameObject(int id)
	:model(nullptr), isEnable(true), isDelete(false), parent(nullptr), transform(*this), id(id), parentId(0)
{
	ChooseCubeModel();
}
Renderer::GameObject::~GameObject() = default;
void Renderer::GameObject::Updata()
{
	if (parent && parent->isDelete)
	{
		parent = nullptr;
		parentId = 0;
	}

	if (renderInfo.displayMode == DisplayMode::Static)
	{
		return;
	}

	switch (renderInfo.displayMode)
	{
		case Renderer::DisplayMode::RotationX:
			transform.Rotate(glm::vec3(std::fmod(TimeUtility::time * renderInfo.rotateSpeed, 360.0f), 0.0f, 0.0f));
			break;

		case Renderer::DisplayMode::RotationY:
			transform.Rotate(glm::vec3(0.0f, std::fmod(TimeUtility::time * renderInfo.rotateSpeed, 360.0f), 0.0f));
			break;

		case Renderer::DisplayMode::RotationZ:
			transform.Rotate(glm::vec3(0.0f, 0.0f, std::fmod(TimeUtility::time * renderInfo.rotateSpeed, 360.0f)));
			break;

		case Renderer::DisplayMode::SwayX:
			transform.Rotate(glm::vec3(MathUtility::sinTime * renderInfo.rotateSpeed, 0.0f, 0.0f));
			break;

		case Renderer::DisplayMode::SwayY:
			transform.Rotate(glm::vec3(0.0f, MathUtility::sinTime * renderInfo.rotateSpeed, 0.0f));
			break;

		case Renderer::DisplayMode::SwayZ:
			transform.Rotate(glm::vec3(0.0f, 0.0f, MathUtility::sinTime * renderInfo.rotateSpeed));
			break;
	}
}
void Renderer::GameObject::Draw(Renderer::Shader& shader)
{
	if (!isEnable || isDelete || !model)
	{
		return;
	}

	if (!renderInfo.isEnableOutLine)
	{
		glStencilMask(0x00);
	}
	if (renderInfo.isEnableCullFace)
	{
		if (!renderInfo.isCullBack)
		{
			glCullFace(GL_FRONT);
		}
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	switch (renderInfo.shaderType)
	{
		case ShaderType::Default:
			DrawDefaultMode(shader);
			break;

		case ShaderType::SolidColor:
			DrawSolidMode(shader);
			break;

		case ShaderType::Cutout:
			DrawCutoutMode(shader);
			break;

		case ShaderType::Transparent:
			DrawTransparentMode(shader);
			break;

		case ShaderType::Reflect:
			DrawReflectMode(shader);
			break;

		case ShaderType::Refract:
			DrawRefractMode(shader);
			break;

		case ShaderType::Expland:
			DrawExplandMode(shader);
			break;
	}

	if (!renderInfo.isEnableOutLine)
	{
		glStencilMask(0xFF);
	}
	if (renderInfo.isEnableCullFace)
	{
		if (!renderInfo.isCullBack)
		{
			glCullFace(GL_BACK);
		}
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
}
void Renderer::GameObject::DrawOutLine(Shader& shader)
{
	if (!isEnable || isDelete || !renderInfo.isEnableOutLine || !model)
	{
		return;
	}

	DrawOutLineMode(shader);
}
void Renderer::GameObject::DrawShadowMap(Shader& shader,ShaderType shaderType)
{
	if (!isEnable || isDelete ||  !model)
	{
		return;
	}

	DrawShadowMapModel(shader, shaderType);
}
void Renderer::GameObject::DrawDefaultMode(Shader& shader)
{
	shader.SetSetMatrix4("model", Model());
	model->Draw(shader, ShaderType::Default);
}
void Renderer::GameObject::DrawSolidMode(Shader& shader)
{
	shader.SetSetMatrix4("model", Model());
	shader.SetVec3("solidColor", renderInfo.solidColor);
	model->Draw(shader, ShaderType::SolidColor);
}
void Renderer::GameObject::DrawOutLineMode(Shader& shader)
{
	shader.SetSetMatrix4("model", Model() * glm::scale(glm::mat4(1.0f), glm::vec3(renderInfo.outLineWidth)));
	shader.SetVec3("outLineColor", renderInfo.outLineColor);
	model->Draw(shader, ShaderType::OutLine);
}
void Renderer::GameObject::DrawCutoutMode(Shader& shader)
{
	shader.SetSetMatrix4("model", Model());
	shader.SetFloat("cutoutValue", renderInfo.cutoutValue);
	model->Draw(shader, ShaderType::Cutout);
}
void Renderer::GameObject::DrawTransparentMode(Shader& shader)
{
	shader.SetSetMatrix4("model", Model());
	model->Draw(shader, ShaderType::Transparent);
}
void Renderer::GameObject::DrawReflectMode(Shader& shader)
{
	shader.SetSetMatrix4("model", Model());
	shader.SetFloat("reflectivity", renderInfo.reflectivity);
	model->Draw(shader, ShaderType::Reflect);
}
void Renderer::GameObject::DrawRefractMode(Shader& shader)
{
	shader.SetSetMatrix4(MODEL, Model());
	shader.SetFloat("refractive", renderInfo.refractive);
	model->Draw(shader, ShaderType::Refract);
}
void Renderer::GameObject::DrawExplandMode(Shader& shader)
{
	shader.SetSetMatrix4(MODEL, Model());
	shader.SetFloat(EXPLAND_DEGRESS, renderInfo.isFloowTime ? ((sin(TimeUtility::time * renderInfo.explandSpeed) + 1) / 2) * renderInfo.explandRange : renderInfo.explandDegress);
	model->Draw(shader, ShaderType::Expland);
}
void Renderer::GameObject::DrawNormalMode(Shader& shader)
{
	shader.SetSetMatrix4(MODEL, Model());
	shader.SetFloat("lineDistance", renderInfo.normalLineDistance);
	shader.SetVec3("normalColor", renderInfo.normalColor);
	model->Draw(shader, ShaderType::Normal);
}
void Renderer::GameObject::DrawShadowMapModel(Shader& shader, ShaderType shaderType)
{
	shader.SetSetMatrix4("model", Model());
	model->Draw(shader, shaderType);
}
void Renderer::GameObject::SetIsDelete(bool value)
{
	isDelete = value;
}
void Renderer::GameObject::ChooseDiffuseMap()
{
	switch (renderInfo.diffuseMapChoose)
	{
		case WOOD_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, WOOD_TEXTURE_PATH);
			break;

		case WALL_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, WALL_TEXTURE_PATH);
			break;

		case BOX_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, BOX_TEXTURE_PATH);
			break;

		case ICON_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, HUOLG_TEXTURE_PATH);
			break;

		case GRESS_CUTOOUT_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, GRASS_CUTOUT_TEXTURE_PATH);
			break;

		case FACE_CUTOOUT_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, FACE_CUTOUT_TEXTURE_PATH);
			break;

		case WINDOW_TRANSPARENT_DIFFUSE_INDEX:
			model->SetModelMap(TextureType::Diffuse, WINDOW_TRANSPARENT_TEXTURE_PATH);
			break;

		case CUSTOM_DIFFUSE_INDEX:
			ChooseCustomTexture(TextureType::Diffuse);
			break;
	}
}
void Renderer::GameObject::ChooseSpecularMap()
{
	switch (renderInfo.specularMapChoose)
	{
		case IRONY_SPECULAR_INDEX:
			model->SetModelMap(TextureType::Specular, IRONY_TEXTURE_PATH);
			break;

		case WHITE_SPECULAR_INDEX:
			model->SetModelMap(TextureType::Specular, WHITE_TEXTURE_PATH);
			break;

		case CUSTOM_SPECULAR_INDEX:
			ChooseCustomTexture(TextureType::Specular);
			break;
	}
}
void Renderer::GameObject::ChooseCustomTexture(TextureType type)
{
	const char* filter[] = { "*.png","*.jpg" };
	const char* path = tinyfd_openFileDialog("选择纹理文件", "", 2, filter, "PNG/JPG Files", 0);
	if (path)
	{
		ChooseMap(type, path);
	}
}
void Renderer::GameObject::ChooseMap(TextureType type, const std::string& path)
{
	if (!model)
	{
		return;
	}

	std::string name = path.substr(path.find_last_of('\\') + 1);
	model->SetModelMap(type, ResourceManagerI.LoadTexture(path, type, name, true));
}
void Renderer::GameObject::SetParent()
{
	GameObject* parent = SceneManagerI.GetGameObject(parentId);
	if (!parent || parent == this)
	{
		return;
	}

	this->parent = parent;
}
void Renderer::GameObject::SetModelMap(TextureType type, unsigned int value)
{
	if (!model)
	{
		return;
	}

	model->SetModelMap(type, value);
}
Renderer::Texture* Renderer::GameObject::GetModeMap(TextureType type)
{
	if (!model)
	{
		return nullptr;
	}

	return model->GetModeMap(type);
}
glm::mat4 Renderer::GameObject::Model()
{
	return parent ? parent->Model() * transform.Model() : transform.Model();
}
void Renderer::GameObject::ChooseCubeModel()
{
	ChooseModel(CUBE_MODEL_PATH);
	if (model)
	{
		model->SetModelMap(TextureType::Diffuse,WOOD_TEXTURE_PATH);
		model->SetModelMap(TextureType::Specular, IRONY_TEXTURE_PATH);
		renderInfo.diffuseMapChoose = WOOD_DIFFUSE_INDEX;
		renderInfo.specularMapChoose = IRONY_SPECULAR_INDEX;
	}
}
void Renderer::GameObject::ChooseSphereModel()
{
	ChooseModel(SPHERE_MODEL_PATH);
	if (model)
	{
		model->SetModelMap(TextureType::Diffuse, WALL_TEXTURE_PATH);
		model->SetModelMap(TextureType::Specular, WHITE_TEXTURE_PATH);
		renderInfo.diffuseMapChoose = WALL_DIFFUSE_INDEX;
		renderInfo.specularMapChoose = WHITE_SPECULAR_INDEX;
	}
}
void Renderer::GameObject::ChoosePanelModel()
{
	ChooseModel(PANEL_MODEL_PATH);
	if (model)
	{
		model->SetModelMap(TextureType::Diffuse, WOOD_TEXTURE_PATH);
		model->SetModelMap(TextureType::Specular, IRONY_TEXTURE_PATH);
		renderInfo.diffuseMapChoose = WOOD_DIFFUSE_INDEX;
		renderInfo.specularMapChoose = IRONY_SPECULAR_INDEX;
	}
}
void Renderer::GameObject::ChooseCustomModel()
{
	const char* filter[] = { "*.obj" };
	const char* path = tinyfd_openFileDialog("选择OBJ模型文件", "", 1, filter, "OBJ Files", 0);
	if (path)
	{
		renderInfo.diffuseMapChoose = CUSTOM_DIFFUSE_INDEX;
		renderInfo.specularMapChoose = CUSTOM_SPECULAR_INDEX;
		ChooseModel(path);
	}
}
void Renderer::GameObject::ChooseModel()
{
	switch (renderInfo.modelType)
	{
	case Renderer::ModelType::Cube:
		ChooseCubeModel();
		break;

	case Renderer::ModelType::Sphere:
		ChooseSphereModel();
		break;

	case Renderer::ModelType::Panel:
		ChoosePanelModel();
		break;

	case Renderer::ModelType::Custom:
		ChooseCustomModel();
		break;
	}
}
void Renderer::GameObject::ChooseModel(const char* path)
{
	Renderer::Model* model = Renderer::ResourceManagerI.LoadModel(path);
	if (!model)
	{
		std::cout << "Model Set Fail! Mode Load Is Empty!" << std::endl;
		return;
	}
	if (this->model)
	{
		delete this->model;
		this->model = nullptr;
	}
	this->model = new Renderer::Model(*model);
}