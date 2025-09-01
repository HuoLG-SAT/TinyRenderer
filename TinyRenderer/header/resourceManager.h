#pragma once
#include"mesh.h"
#include"model.h"
#include<unordered_map>
#include<unordered_set>
#include<string>

namespace Renderer
{
	namespace TexturePath
	{
		constexpr const char* BOX_TEXTURE_PATH = "resource\\texture\\box.jpg";
		constexpr const char* IRONY_TEXTURE_PATH = "resource\\texture\\irony.png";
		constexpr const char* WALL_TEXTURE_PATH = "resource\\texture\\wall.jpg";
		constexpr const char* WHITE_TEXTURE_PATH = "resource\\texture\\white.png";
		constexpr const char* WOOD_TEXTURE_PATH = "resource\\texture\\wood.png";
		constexpr const char* HUOLG_TEXTURE_PATH = "resource\\texture\\Huolg.png";
		constexpr const char* GRASS_CUTOUT_TEXTURE_PATH = "resource\\texture\\gress.png";
		constexpr const char* FACE_CUTOUT_TEXTURE_PATH = "resource\\texture\\face.png";
		constexpr const char* WINDOW_TRANSPARENT_TEXTURE_PATH = "resource\\texture\\trasnform_window.png";

#define CUBEMAP_GLACIER_SKYBOX_PATH { "resource/skybox/glacier/right.jpg","resource/skybox/glacier/left.jpg","resource/skybox/glacier/top.jpg","resource/skybox/glacier/bottom.jpg","resource/skybox/glacier/front.jpg","resource/skybox/glacier/back.jpg" }
#define CUBEMAP_PALACE_SKYBOX_PATH {"resource/skybox/palace/right.jpg","resource/skybox/palace/left.jpg","resource/skybox/palace/top.jpg","resource/skybox/palace/bottom.jpg","resource/skybox/palace/front.jpg","resource/skybox/palace/back.jpg"}
	}
	namespace TextureName
	{
		constexpr const char* HUOLG_TEXTURE_NAME = "Huolg.png";
		constexpr const char* BOX_TEXTURE_NAME = "box.jpg";
		constexpr const char* IRONY_TEXTURE_NAME = "irony.jpg";
		constexpr const char* WALL_TEXTURE_NAME = "wall.jpg";
		constexpr const char* WHITE_TEXTURE_NAME = "white.jpg";
		constexpr const char* WOOD_TEXTURE_NAME = "wood.jpg";
		constexpr const char* GRASS_CUTOUT_TEXTURE_NAME = "gress.png";
		constexpr const char* FACE_CUTOUT_TEXTURE_NAME = "face.png";
		constexpr const char* WINDOW_TRANSPARENT_TEXTURE_NAME = "trasnform_window.png";
	}
	namespace SkyboxName
	{
		constexpr const char* CUBEMAP_GLACIER_SKYBOX_NAME = "Glacier";
		constexpr const char* CUBEMAP_PALACE_SKYBOX_NAME = "Palace";
	}
	namespace ModelPath
	{
		constexpr const char* CUBE_MODEL_PATH = "resource\\model\\cube\\cube.obj";
		constexpr const char* SPHERE_MODEL_PATH = "resource\\model\\sphere\\sphere.obj";
		constexpr const char* PANEL_MODEL_PATH = "resource\\model\\panel\\panel.obj";
		constexpr const char* PLANET_MODEL_PATH = "resource\\model\\planet\\planet.obj";
		constexpr const char* ROCK_MODEL_PATH = "resource\\model\\rock\\rock.obj";
	}

	class ResourceManager
	{
	public:
		Texture* LoadTexture(const std::string& path, TextureType type, const std::string& name, bool isSRGB);
		Texture* MmapRGBTexture(Texture* texture);
		Cubemap* LoadCubemap(const std::vector<const char*>& paths, const std::string& name, bool isSRGB);
		Model* LoadModel(const std::string& path);
		bool InitResource();

		inline Texture* GetTexture(unsigned int id)
		{
			return (texturesUnsignedInt.count(id) > 0) ? texturesUnsignedInt[id] : nullptr;
		}
		inline Texture* GetTexture(const std::string& path)
		{
			return (textures.count(path) > 0) ? textures[path] : nullptr;
		}
		inline Cubemap* GetCubemap(const std::string& name)
		{
			return (cubemaps.count(name) > 0) ? cubemaps[name] : nullptr;
		}

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
