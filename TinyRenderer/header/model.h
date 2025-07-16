#pragma once
#include"mesh.h"
#include"shader.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<unordered_map>
namespace Renderer
{
	class Model
	{
		friend class ResourceManager;
		friend class GPUInstanceManager;
	public:
		Model();
		~Model();

		bool Load(const std::string& path);
		void Draw(Shader& shader, ShaderType shaderType);

		void SetModelMap(TextureType type, Texture* texture);
		void SetModelMap(TextureType type, const std::string& path);
		void SetModelMap(TextureType type, unsigned int id);
		Texture* GetModeMap(TextureType type);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Material ProcessMaterial(aiMaterial* _aiMaterial);
		Texture* ProcessTexture(aiMaterial* _aiMaterial, aiTextureType type, TextureType textureType, bool isSRGB);

		void Dispose();

	private:
		std::vector<Mesh> meshs;
		std::string directory;
	};
}