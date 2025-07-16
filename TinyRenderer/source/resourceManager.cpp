#include"../header/resourceManager.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
Renderer::ResourceManager::ResourceManager() = default;
Renderer::ResourceManager::~ResourceManager()
{
	for (auto& pair : textures)
	{
		if (pair.second)
		{
			glDeleteTextures(1, &pair.second->id);
			delete pair.second;
			pair.second = nullptr;
		}
	}
	textures.clear();

	for (auto& pair : models)
	{
		if (pair.second)
		{
			pair.second->Dispose();
			delete pair.second;
			pair.second = nullptr;
		}
	}
	models.clear();

	for (auto& pair : cubemaps)
	{
		if (pair.second)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}
	cubemaps.clear();

	for (auto& pair : mmapRGBTetxures)
	{
		if (pair.second)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}
	mmapRGBTetxures.clear();
}
Renderer::Texture* Renderer::ResourceManager::LoadTexture(const std::string& path, TextureType type, const std::string& name, bool isSRGB)
{
	if (textures.count(path) > 0)
	{
		return textures[path];
	}

	Texture* texture = new Texture();
	texture->id = Renderer::TextureUtility::LoadTexture(path.c_str(), isSRGB);
	if (texture->id == -1)
	{
		delete texture;
		texture = nullptr;
		return nullptr;
	}
	texture->type = type;
	texture->name = name;
	texture->path = path;
	texture->isSRGB = isSRGB;
	textures.emplace(path, texture);
	texturesUnsignedInt.emplace(texture->id, texture);
	return texture;
}
Renderer::Texture* Renderer::ResourceManager::MmapRGBTexture(Texture* texture)
{
	if (!texture)
	{
		std::cout << "Mmap RGB Fail Tetxure Is Empty!" << std::endl;
		return nullptr;
	}
	
	if (mmapRGBTetxures.count(texture) > 0)
	{
		return mmapRGBTetxures[texture];
	}

	Texture* RGBTexture = new Texture();
	RGBTexture->id = Renderer::TextureUtility::LoadTexture(texture->path.c_str(), false);
	if (RGBTexture->id == -1)
	{
		delete RGBTexture;
		RGBTexture = nullptr;
		return nullptr;
	}
	RGBTexture->name = texture->name;
	RGBTexture->path = texture->path;
	RGBTexture->type = texture->type;
	RGBTexture->isSRGB = false;
	mmapRGBTetxures.emplace(texture, RGBTexture);
}
Renderer::Texture* Renderer::ResourceManager::GetTexture(const std::string& path)
{
	if (textures.count(path) > 0)
	{
		return textures[path];
	}
	return nullptr;
}
Renderer::Texture* Renderer::ResourceManager::GetTexture(unsigned int id)
{
	if (texturesUnsignedInt.count(id) > 0)
	{
		return texturesUnsignedInt[id];
	}
	return nullptr;
}
Renderer::Model* Renderer::ResourceManager::LoadModel(const std::string& path)
{
	if (models.count(path) <= 0)
	{
		Model* model = new Model();
		if (!model->Load(path))
		{
			delete model;
			model = nullptr;
			std::cout << "Model Load Fail! In Path " << path << std::endl;
			return nullptr;
		}
		models.emplace(path, model);
	}
	return models[path];
}
Renderer::Cubemap* Renderer::ResourceManager::LoadCubemap(const std::vector<const char*>& paths, const std::string& name, bool isSRGB)
{
	if (cubemaps.count(name) <= 0)
	{
		Cubemap* cubemap = new Cubemap();
		cubemap->id = Renderer::CubemapUtility::LoadCubemap(paths, isSRGB);
		if (cubemap->id == -1)
		{
			std::cout << "Load Cube Map Fail!" << std::endl;
			delete cubemap;
			cubemap = nullptr;
			return nullptr;
		}
		cubemap->name = name;
		cubemaps.emplace(name, cubemap);
	}

	return cubemaps[name];
}
Renderer::Cubemap* Renderer::ResourceManager::GetCubemap(const std::string& name)
{
	if (cubemaps.count(name) > 0)
	{
		return cubemaps[name];
	}
	return nullptr;
}
bool Renderer::ResourceManager::CopyModel(const char* srcPath, const char* dstPath)
{
	Model* srcModel = LoadModel(srcPath);
	if (!srcModel)
	{
		std::cout << "Copy Model Fail Src Model Is Empty! Src Path:" << srcPath << "Dst Path:" << dstPath << std::endl;
		return false;
	}

	Model* dstModel = new Model(*srcModel);
	if (models.count(dstPath) > 0)
	{
		delete models[dstPath];
		models[dstPath] = nullptr;
		models.erase(dstPath);
	}
	models.emplace(dstPath, dstModel);

	return true;
}
bool Renderer::ResourceManager::InitResource()
{
	if (!LoadTexture(BOX_TEXTURE_PATH, Renderer::TextureType::Diffuse, BOX_TEXTURE_NAME,true)						||
		!LoadTexture(IRONY_TEXTURE_PATH, Renderer::TextureType::Specular, IRONY_TEXTURE_NAME, true)					||
		!LoadTexture(WALL_TEXTURE_PATH, Renderer::TextureType::Diffuse, WALL_TEXTURE_NAME, true)					||
		!LoadTexture(WOOD_TEXTURE_PATH, Renderer::TextureType::Diffuse, WOOD_TEXTURE_NAME, true)					||
		!LoadTexture(WHITE_TEXTURE_PATH, Renderer::TextureType::Specular, WHITE_TEXTURE_NAME, true)					||
		!LoadTexture(HUOLG_TEXTURE_PATH, Renderer::TextureType::Diffuse, HUOLG_TEXTURE_NAME, true)					||
		!LoadTexture(GRASS_CUTOUT_TEXTURE_PATH, Renderer::TextureType::Diffuse, GRASS_CUTOUT_TEXTURE_NAME, true)	||
		!LoadTexture(FACE_CUTOUT_TEXTURE_PATH, Renderer::TextureType::Diffuse, GRASS_CUTOUT_TEXTURE_NAME, true)		||
		!LoadTexture(WINDOW_TRANSPARENT_TEXTURE_PATH, Renderer::TextureType::Diffuse, GRASS_CUTOUT_TEXTURE_NAME, true))
	{
		return false;
	}
	if (!MmapRGBTexture(GetTexture(BOX_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(IRONY_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(WALL_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(WOOD_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(WHITE_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(HUOLG_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(GRASS_CUTOUT_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(FACE_CUTOUT_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(WINDOW_TRANSPARENT_TEXTURE_PATH)))
	{
		return false;
	}
	if (!LoadModel(CUBE_MODEL_PATH)   || 
		!LoadModel(PANEL_MODEL_PATH)  ||
		!LoadModel(PLANET_MODEL_PATH) ||
		!LoadModel(ROCK_MODEL_PATH))
	{
		return false;
	}
	if (!CopyModel(PLANET_MODEL_PATH, SPHERE_MODEL_PATH))
	{
		return false;
	}
	if (!LoadCubemap(CUBEMAP_GLACIER_SKYBOX_PATH, CUBEMAP_GLACIER_SKYBOX_NAME,true) ||
		!LoadCubemap(CUBEMAP_PALACE_SKYBOX_PATH, CUBEMAP_PALACE_SKYBOX_NAME,true))
	{
		return false;
	}
	return true;
}
Renderer::ResourceManager& Renderer::ResourceManager::Instance = instance;
Renderer::ResourceManager Renderer::ResourceManager::instance;