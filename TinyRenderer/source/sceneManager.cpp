#include"../header/sceneManager.h"
#include"../header/renderer.h"
#include"../header/camera.h"
#include"../header/lightManager.h"
#include"../header/skybox.h"
#include"../header/shadowMapManager.h"

Renderer::SceneManager::SceneManager():isDirty(false), ids(1)
{

}
Renderer::SceneManager::~SceneManager()
{
	for (auto& gameObject : gameObjectList)
	{
		delete gameObject;
		gameObject = nullptr;
	}
	gameObjectList.clear();
	gameObjectMap.clear();
	defaultShaderGOs.clear();
	solidColorShaderGOs.clear();
	outLineShaderGOs.clear();
	cutoutShaderGOs.clear();
	transparentShaderGosList.clear();
	transparentShaderGos.clear();
	reflectShaderGOs.clear();
	refractShaderGOs.clear();
	explandShaderGOs.clear();
	normalShadeGOs.clear();
}
void Renderer::SceneManager::Updata()
{
	for (auto& gameObject : gameObjectList)
	{
		gameObject->Updata();
	}

	gameObjectList.erase(std::remove_if(gameObjectList.begin(), gameObjectList.end(), [&](GameObject*& gameObject)
		{
			if (gameObject->isDelete)
			{
				gameObjectMap.erase(gameObject->id);
				ShadowMapManagerI.RemoveShadowCaster(gameObject->id);
				delete gameObject;
				gameObject = nullptr;
				return true;
			}
			return false;
		}), gameObjectList.end());

	if (!isDirty && (transparentShaderGos.size() > 0))
	{
		SortTransparentGOs();
	}

	if (isDirty)
	{
		isDirty = false;
		defaultShaderGOs.clear();
		solidColorShaderGOs.clear();
		outLineShaderGOs.clear();
		cutoutShaderGOs.clear();
		transparentShaderGosList.clear();
		transparentShaderGos.clear();
		reflectShaderGOs.clear();
		refractShaderGOs.clear();
		explandShaderGOs.clear();
		normalShadeGOs.clear();
		for (auto& gameObject : gameObjectList)
		{
			if (!gameObject->isEnable)
			{
				continue;
			}

			switch (gameObject->renderInfo.shaderType)
			{
				case ShaderType::Default:
					defaultShaderGOs.push_back(gameObject);
					break;

				case ShaderType::SolidColor:
					solidColorShaderGOs.push_back(gameObject);
					break;

				case ShaderType::Cutout:
					cutoutShaderGOs.push_back(gameObject);
					break;

				case ShaderType::Transparent:
					transparentShaderGosList.push_back(gameObject);
					transparentShaderGos.emplace(glm::distance(CameraI.Position(), gameObject->transform.Position()), gameObject);
					break;

				case ShaderType::Reflect:
					reflectShaderGOs.push_back(gameObject);
					break;

				case ShaderType::Refract:
					refractShaderGOs.push_back(gameObject);
					break;

				case ShaderType::Expland:
					explandShaderGOs.push_back(gameObject);
					break;
			}

			if (gameObject->renderInfo.isEnableOutLine)
			{
				outLineShaderGOs.push_back(gameObject);
			}
			if (gameObject->renderInfo.isEnableNormalVisable)
			{
				normalShadeGOs.push_back(gameObject);
			}
		}
	}
}
void Renderer::SceneManager::SortTransparentGOs()
{
	transparentShaderGos.clear();
	for (auto& gameObject : transparentShaderGosList)
	{
		transparentShaderGos.emplace(glm::distance(CameraI.Position(), gameObject->transform.Position()), gameObject);
	}
}
void Renderer::SceneManager::AddGameObject()
{
	int id = ids++;
	GameObject* gameObject = new GameObject(id);
	gameObjectList.push_back(gameObject);
	gameObjectMap.emplace(id, gameObject);
	ShadowMapManagerI.AddShadowCaster(id, gameObject);
	isDirty = true;
}
Renderer::GameObject* Renderer::SceneManager::GetGameObject(int id)
{
	if (gameObjectMap.count(id) > 0)
	{
		return gameObjectMap[id];
	}
	return nullptr;
}
void Renderer::SceneManager::DrawGeometry()
{
	Shader* defaultShader = ShaderManagerI.GetShader(ShaderType::Default);
	if (defaultShader && (defaultShaderGOs.size() > 0))
	{
		defaultShader->Use();
		Renderer::LightManagerI.SetShader(*defaultShader);
		for (auto& gameObject : defaultShaderGOs)
		{
			if (gameObject)
			{
				gameObject->Draw(*defaultShader);
			}
		}
	}

	Shader* explodeShader = ShaderManagerI.GetShader(ShaderType::Expland);
	if (explodeShader && (explandShaderGOs.size() > 0))
	{
		explodeShader->Use();
		for (auto& gameObject : explandShaderGOs)
		{
			if (gameObject)
			{
				gameObject->Draw(*explodeShader);
			}
		}
	}

	Shader* solidShader = ShaderManagerI.GetShader(ShaderType::SolidColor);
	if (solidShader && (solidColorShaderGOs.size() > 0))
	{
		solidShader->Use();
		for (auto& gameObject : solidColorShaderGOs)
		{
			if (gameObject)
			{
				gameObject->Draw(*solidShader);
			}
		}
	}

	Shader* reflectShader = ShaderManagerI.GetShader(ShaderType::Reflect);
	if (reflectShader && (reflectShaderGOs.size() > 0))
	{
		reflectShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManagerI.skyBoxCubemap->id);
		reflectShader->SetInt("cubemap", 0);
		for (auto& gameObject : reflectShaderGOs)
		{
			if (gameObject)
			{
				gameObject->Draw(*reflectShader);
			}
		}
	}

	Shader* refractShader = ShaderManagerI.GetShader(ShaderType::Refract);
	if (refractShader && (refractShaderGOs.size() > 0))
	{
		refractShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManagerI.skyBoxCubemap->id);
		refractShader->SetInt("cubemap", 0);
		for (auto& gameObject : refractShaderGOs)
		{
			if (gameObject)
			{
				gameObject->Draw(*refractShader);
			}
		}
	}

	Shader* cutoutShader = ShaderManagerI.GetShader(ShaderType::Cutout);
	if (cutoutShader && (cutoutShaderGOs.size() > 0))
	{
		cutoutShader->Use();
		for (auto& gameObject : cutoutShaderGOs)
		{
			if (gameObject)
			{
				gameObject->Draw(*cutoutShader);
			}
		}
	}

	Shader* normalShader = ShaderManagerI.GetShader(ShaderType::Normal);
	if (normalShader && (normalShadeGOs.size() > 0))
	{
		normalShader->Use();
		for (auto& gameObject : normalShadeGOs)
		{
			if (gameObject)
			{
				gameObject->DrawNormalMode(*normalShader);
			}
		}
	}

	Shader* outLineShader = ShaderManagerI.GetShader(ShaderType::OutLine);
	if (outLineShader && (outLineShaderGOs.size() > 0))
	{
		glDisable(GL_CULL_FACE);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		outLineShader->Use();
		for (auto& gameObject : outLineShaderGOs)
		{
			if (gameObject)
			{
				gameObject->DrawOutLine(*outLineShader);
			}
		}
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glEnable(GL_CULL_FACE);
	}
}
void Renderer::SceneManager::DrawTransparent()
{
	Shader* transparentShader = ShaderManagerI.GetShader(ShaderType::Transparent);
	if (transparentShader && (transparentShaderGos.size() > 0))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		transparentShader->Use();
		std::map<float, GameObject*>::reverse_iterator reite = transparentShaderGos.rbegin();
		while (reite != transparentShaderGos.rend())
		{
			if (reite->second)
			{
				reite->second->Draw(*transparentShader);
			}
			reite++;
		}
		glDisable(GL_BLEND);
	}
}
void Renderer::SceneManager::SetDirty()
{
	isDirty = true;
}
Renderer::SceneManager& Renderer::SceneManager::Instance = instance;
Renderer::SceneManager Renderer::SceneManager::instance;