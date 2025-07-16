#pragma once
#include"renderer.h"
#include"../depend/glm/glm.hpp"
#include"../depend/glm/gtc/matrix_transform.hpp"
#include"../depend/glm/gtc/type_ptr.hpp"
#include"shader.h"
#include<vector>
#include<string>

namespace Renderer
{
#define MATERIAL_SHINESS "material.shininess"
#define MATERIAL_DIFFUSE_COLOR "material.diffuseColor"
#define MATERIAL_SPECULAR_COLOR "material.specularColor"
#define MATERIAL_AMBIENT_COLOR "material.ambientColor"
#define MATERIAL_DIFFUSE_MAP "material.diffuseMap"
#define MATERIAL_SPECULAR_MAP "material.specularMap"
#define CUTOUT_TEXTURE "cutoutTexture"
#define TRANSPARENT_TEXTURE	"transparentTexture"
#define EXPLANDE_TEXTURE "_texture"
#define PLANET_TEXTURE "_texture"
#define ROCK_TETXURE "_texture"

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	enum class TextureType
	{
		Diffuse,
		Specular
	};

	struct Texture
	{
		unsigned int id;
		TextureType type;
		std::string name;
		std::string path;
		bool isSRGB;
	};

	struct Cubemap
	{
		unsigned int id;
		std::string name;
	};

	struct Material
	{
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		glm::vec3 ambientColor;
		std::unordered_map<TextureType, Texture*> textures;
		float shininess;

		void Apply(Shader& shader,ShaderType shaderType);
		void ApplyDefaultMaterial(Shader& shader);
		void ApplyCutoutMaterila(Shader& shader);
		void ApplyTransparentMaterial(Shader& shader);
		void ApplyExplandMaterial(Shader& shader);
		void ApplyPlanetMaterial(Shader& shader);
		void ApplyRockMaterial(Shader& shader);
	};

	class Mesh
	{
		friend class Model;
		friend class GPUInstanceManager;

	public:
		Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> indexs, Material material);
		~Mesh();

		void Draw(Shader& shader, ShaderType shaderType);

	private:
		void SetupMesh();
		void Dispose();

	private:
		std::vector<Vertex> vertexs;
		std::vector<unsigned int> indexs;
		Material material;

		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
	};

	namespace TextureUtility
	{
		unsigned int LoadTexture(const char* path, bool isSRGB);
	}

	namespace CubemapUtility
	{
		unsigned int LoadCubemap(const std::vector<const char*>& path, bool isSRGB);
	}
}