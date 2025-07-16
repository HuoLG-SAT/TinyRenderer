#pragma once
#include"mesh.h"
#include"model.h"
#include<unordered_map>
#include<unordered_set>
#include<string>

namespace Renderer
{
#define BOX_TEXTURE_PATH ("resource\\texture\\box.jpg")
#define IRONY_TEXTURE_PATH ("resource\\texture\\irony.png")
#define WALL_TEXTURE_PATH ("resource\\texture\\wall.jpg")
#define WHITE_TEXTURE_PATH ("resource\\texture\\white.png")
#define WOOD_TEXTURE_PATH ("resource\\texture\\wood.png")
#define HUOLG_TEXTURE_PATH ("resource\\texture\\Huolg.png")
#define GRASS_CUTOUT_TEXTURE_PATH ("resource\\texture\\gress.png")
#define FACE_CUTOUT_TEXTURE_PATH ("resource\\texture\\face.png")
#define WINDOW_TRANSPARENT_TEXTURE_PATH ("resource\\texture\\trasnform_window.png")
#define HUOLG_TEXTURE_NAME ("Huolg.png")
#define BOX_TEXTURE_NAME ("box.jpg")
#define IRONY_TEXTURE_NAME ("irony.jpg")
#define WALL_TEXTURE_NAME ("wall.jpg")
#define WHITE_TEXTURE_NAME ("white.jpg")
#define WOOD_TEXTURE_NAME ("wood.jpg")
#define GRASS_CUTOUT_TEXTURE_NAME ("gress.png")
#define FACE_CUTOUT_TEXTURE_NAME ("face.png")
#define WINDOW_TRANSPARENT_TEXTURE_NAME ("trasnform_window.png")
#define CUBE_MODEL_PATH ("resource\\model\\cube\\cube.obj")
#define SPHERE_MODEL_PATH ("resource\\model\\sphere\\sphere.obj")
#define PANEL_MODEL_PATH ("resource\\model\\panel\\panel.obj")
#define PLANET_MODEL_PATH ("resource\\model\\planet\\planet.obj")
#define ROCK_MODEL_PATH ("resource\\model\\rock\\rock.obj")
#define CUBEMAP_GLACIER_SKYBOX_PATH {"resource/skybox/glacier/right.jpg","resource/skybox/glacier/left.jpg","resource/skybox/glacier/top.jpg","resource/skybox/glacier/bottom.jpg","resource/skybox/glacier/front.jpg","resource/skybox/glacier/back.jpg"}
#define CUBEMAP_PALACE_SKYBOX_PATH {"resource/skybox/palace/right.jpg","resource/skybox/palace/left.jpg","resource/skybox/palace/top.jpg","resource/skybox/palace/bottom.jpg","resource/skybox/palace/front.jpg","resource/skybox/palace/back.jpg"}
#define CUBEMAP_GLACIER_SKYBOX_NAME "Glacier"
#define CUBEMAP_PALACE_SKYBOX_NAME "Palace"

	class ResourceManager
	{
	public:
		Texture* LoadTexture(const std::string& path, TextureType type, const std::string& name, bool isSRGB);
		Texture* MmapRGBTexture(Texture* texture);
		Texture* GetTexture(unsigned int id);
		Texture* GetTexture(const std::string& path);
		Cubemap* LoadCubemap(const std::vector<const char*>& paths, const std::string& name, bool isSRGB);
		Cubemap* GetCubemap(const std::string& name);
		Model* LoadModel(const std::string& path);
		bool InitResource();

		static ResourceManager& Instance;

	private:
		ResourceManager();
		~ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		bool CopyModel(const char* src, const char* dst);

	private:
		std::unordered_map<std::string, Texture*> textures;
		std::unordered_map<Texture*, Texture*> mmapRGBTetxures;
		std::unordered_map<std::string, Model*> models;
		std::unordered_map<std::string, Cubemap*> cubemaps;

		std::unordered_map<unsigned int, Texture*> texturesUnsignedInt;

		static ResourceManager instance;
	};
}
