#include"../header/resourcemanager.h"
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
	if (!LoadTexture(TexturePath::BOX_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::BOX_TEXTURE_NAME,true)							||
		!LoadTexture(TexturePath::IRONY_TEXTURE_PATH, Renderer::TextureType::Specular, TextureName::IRONY_TEXTURE_NAME, true)					||
		!LoadTexture(TexturePath::WALL_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::WALL_TEXTURE_NAME, true)						||
		!LoadTexture(TexturePath::WOOD_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::WOOD_TEXTURE_NAME, true)						||
		!LoadTexture(TexturePath::WHITE_TEXTURE_PATH, Renderer::TextureType::Specular, TextureName::WHITE_TEXTURE_NAME, true)					||
		!LoadTexture(TexturePath::HUOLG_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::HUOLG_TEXTURE_NAME, true)					||
		!LoadTexture(TexturePath::GRASS_CUTOUT_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::GRASS_CUTOUT_TEXTURE_NAME, true)		||
		!LoadTexture(TexturePath::FACE_CUTOUT_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::GRASS_CUTOUT_TEXTURE_NAME, true)		||
		!LoadTexture(TexturePath::WINDOW_TRANSPARENT_TEXTURE_PATH, Renderer::TextureType::Diffuse, TextureName::GRASS_CUTOUT_TEXTURE_NAME, true))
	{
		return false;
	}
	if (!MmapRGBTexture(GetTexture(TexturePath::BOX_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::IRONY_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::WALL_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::WOOD_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::WHITE_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::HUOLG_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::GRASS_CUTOUT_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::FACE_CUTOUT_TEXTURE_PATH)) ||
		!MmapRGBTexture(GetTexture(TexturePath::WINDOW_TRANSPARENT_TEXTURE_PATH)))
	{
		return false;
	}
	if (!LoadModel(ModelPath::CUBE_MODEL_PATH)   || 
		!LoadModel(ModelPath::PANEL_MODEL_PATH)  ||
		!LoadModel(ModelPath::PLANET_MODEL_PATH) ||
		!LoadModel(ModelPath::ROCK_MODEL_PATH))
	{
		return false;
	}
	if (!CopyModel(ModelPath::PLANET_MODEL_PATH, ModelPath::SPHERE_MODEL_PATH))
	{
		return false;
	}
	if (!LoadCubemap(CUBEMAP_GLACIER_SKYBOX_PATH, SkyboxName::CUBEMAP_GLACIER_SKYBOX_NAME,true) ||
		!LoadCubemap(CUBEMAP_PALACE_SKYBOX_PATH, SkyboxName::CUBEMAP_PALACE_SKYBOX_NAME,true))
	{
		return false;
	}
	return true;
}
Renderer::ResourceManager& Renderer::ResourceManager::Instance = instance;
Renderer::ResourceManager Renderer::ResourceManager::instance;