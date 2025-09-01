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
		unsigned int id = 0;
		TextureType type = TextureType::Diffuse;
		std::string name;
		std::string path;
		bool isSRGB = false;
	};

	struct Cubemap
	{
		unsigned int id = 0;
		std::string name;
	};

	struct Material
	{
		glm::vec3 diffuseColor = glm::vec3(0.0f);
		glm::vec3 specularColor = glm::vec3(0.0f);
		glm::vec3 ambientColor = glm::vec3(0.0f);
		std::unordered_map<TextureType, Texture*> textures;
		float shininess = 0.0f;

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

		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
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